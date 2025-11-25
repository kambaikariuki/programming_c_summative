#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>

struct CURLResponse {
    char *html;
    size_t size;
};

// Write callback for libcurl
static size_t WriteHTMLCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct CURLResponse *mem = (struct CURLResponse *) userp;

    char *ptr = realloc(mem->html, mem->size + realsize + 1);
    if (!ptr) {
        printf("Not enough memory available (realloc returned NULL)\n");
        return 0;
    }

    mem->html = ptr;
    memcpy(&(mem->html[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->html[mem->size] = '\0';

    return realsize;
};

// Struct to hold thread parameters
typedef struct {
    int id;
    const char *url;
} ThreadArgs;

// Performs a GET request (each thread has its own CURL handle)
struct CURLResponse GetRequest(const char *url) {
    struct CURLResponse response;
    response.html = malloc(1);
    response.size = 0;

    if (!response.html) {
        fprintf(stderr, "Memory allocation failed\n");
        return response;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "CURL init failed\n");
        free(response.html);
        response.html = NULL;
        return response;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteHTMLCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "[Thread] GET request failed: %s\n", curl_easy_strerror(res));
        free(response.html);
        response.html = NULL;
    }

    curl_easy_cleanup(curl);
    return response;
}

// Thread function
void *ThreadEntry(void *arg) {
    ThreadArgs *t = (ThreadArgs *)arg;

    printf("[Thread %d] Fetching %s\n", t->id, t->url);

    struct CURLResponse resp = GetRequest(t->url);

    if (!resp.html) {
        printf("[Thread %d] Failed to scrape %s\n", t->id, t->url);
        return NULL;
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "output_%d.html", t->id);

    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        free(resp.html);
        return NULL;
    }

    fwrite(resp.html, 1, resp.size, f);
    fclose(f);
    free(resp.html);

    printf("[Thread %d] Saved → %s\n", t->id, filename);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <url1> <url2> ...\n", argv[0]);
        return 1;
    }

    int num_threads = argc - 1;

    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];

    curl_global_init(CURL_GLOBAL_ALL);

    for (int i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].url = argv[i + 1];

        pthread_create(&threads[i], NULL, ThreadEntry, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    curl_global_cleanup();

    printf("All downloads finished.\n");
    return 0;
}
