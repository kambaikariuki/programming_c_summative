#include <stdio.h>
#include <stdlib.h>

/* ============================
   Function Pointer Typedefs
   ============================ */
typedef void (*operation_func)(int *, int);

/* ============================
   Utility Function Prototypes
   ============================ */
void print_menu();
void print_operations_menu();

/* Dataset operations */
void sum(int *data, int size);
void average(int *data, int size);
void find_min(int *data, int size);
void find_max(int *data, int size);
void sort_asc(int *data, int size);
void sort_desc(int *data, int size);
void search_value(int *data, int size);

/* File operations */
void load_from_file(int **data, int *size);
void save_to_file(int *data, int size);

/* Dataset functions */
void view_dataset(int *data, int size);
void add_value(int **data, int *size);
void modify_value(int *data, int size);
void delete_value(int **data, int *size);

   // Main Program

int main() {
    int *data = NULL;
    int size = 0;
    int choice;

    operation_func operations[] = {
        sum,
        average,
        find_min,
        find_max,
        sort_asc,
        sort_desc,
        search_value
    };

    int num_operations = sizeof(operations) / sizeof(operations[0]);

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_value(&data, &size);
                break;
            case 2:
                modify_value(data, size);
                break;
            case 3:
                delete_value(&data, &size);
                break;
            case 4:
                view_dataset(data, size);
                break;
            case 5: {
                print_operations_menu();
                int op;
                scanf("%d", &op);

                if (op >= 1 && op <= num_operations) {
                    operations[op - 1](data, size);
                } else {
                    printf("Invalid operation\n");
                }
                break;
            }
            case 6:
                load_from_file(&data, &size);
                break;
            case 7:
                save_to_file(data, size);
                break;
            case 8:
                free(data);
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid menu option.\n");
        }
    }
}



//   Menu Printing

void print_menu() {
    printf("\n===== Dynamic Math & Data Engine =====\n");
    printf("1. Add value to dataset\n");
    printf("2. Modify value in dataset\n");
    printf("3. Delete value from dataset\n");
    printf("4. View current dataset\n");
    printf("5. Perform operation\n");
    printf("6. Load data from file\n");
    printf("7. Save data to file\n");
    printf("8. Exit\n");
    printf("Choose an option: ");
}

void print_operations_menu() {
    printf("\n------ Operations ------\n");
    printf("1. Sum of elements\n");
    printf("2. Average of elements\n");
    printf("3. Find minimum\n");
    printf("4. Find maximum\n");
    printf("5. Sort ascending\n");
    printf("6. Sort descending\n");
    printf("7. Search for a value\n");
    printf("Choose an operation: ");
}

//     Dataset Operations
void view_dataset(int *data, int size) {
    if (size == 0) {
        printf("Dataset is empty.\n");
        return;
    }

    printf("\n--- Current Dataset (%d values) ---\n", size);
    for (int i = 0; i < size; i++) {
        printf("[%d] = %d\n", i, data[i]);
    }
    printf("-----------------------------------\n");
}

void sum(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    int sum = 0;
    for (int i = 0; i < size; i++) sum += data[i];
    printf("Sum = %d\n", sum);
}

void average(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    int sum = 0;
    for (int i = 0; i < size; i++) sum += data[i];
    printf("Average = %.2f\n", (float)sum / size);
}

void find_min(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    int min = data[0];
    for (int i = 1; i < size; i++)
        if (data[i] < min) min = data[i];
    printf("Minimum value = %d\n", min);
}

void find_max(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    int max = data[0];
    for (int i = 1; i < size; i++)
        if (data[i] > max) max = data[i];
    printf("Maximum value = %d\n", max);
}

void sort_asc(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (data[j] > data[j + 1]) {
                int tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
    printf("Sorted ascending.\n");
}

void sort_desc(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (data[j] < data[j + 1]) {
                int tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
    printf("Sorted descending.\n");
}

void search_value(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    int val;
    printf("Enter value to search: ");
    scanf("%d", &val);

    for (int i = 0; i < size; i++) {
        if (data[i] == val) {
            printf("Value %d found at index %d.\n", val, i);
            return;
        }
    }
    printf("Value not found.\n");
}

//   Dataset Editing

void add_value(int **data, int *size) {
    int new_val;
    printf("Enter value to add: ");
    scanf("%d", &new_val);

    *data = realloc(*data, (*size + 1) * sizeof(int));
    (*data)[*size] = new_val;
    (*size)++;

    printf("Value added.\n");
}

void modify_value(int *data, int size) {
    if (size == 0) { printf("Dataset empty.\n"); return; }
    int index, new_val;

    printf("Enter index to modify (0-%d): ", size - 1);
    scanf("%d", &index);

    if (index < 0 || index >= size) {
        printf("Invalid index.\n");
        return;
    }

    printf("Enter new value: ");
    scanf("%d", &new_val);
    data[index] = new_val;

    printf("Value updated.\n");
}

void delete_value(int **data, int *size) {
    if (*size == 0) { printf("Dataset empty.\n"); return; }

    int index;
    printf("Enter index to delete (0-%d): ", *size - 1);
    scanf("%d", &index);

    if (index < 0 || index >= *size) {
        printf("Invalid index.\n");
        return;
    }

    for (int i = index; i < *size - 1; i++)
        (*data)[i] = (*data)[i + 1];

    *data = realloc(*data, (*size - 1) * sizeof(int));
    (*size)--;

    printf("Value deleted.\n");
}

/* ============================
   File I/O
   ============================ */

void load_from_file(int **data, int *size) {
    char filename[100];
    printf("Enter filename to load: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }

    int temp, count = 0;
    int *buffer = NULL;

    while (fscanf(fp, "%d", &temp) == 1) {
        buffer = realloc(buffer, (count + 1) * sizeof(int));
        buffer[count++] = temp;
    }

    fclose(fp);

    free(*data);
    *data = buffer;
    *size = count;

    printf("Loaded %d values.\n", count);
}

void save_to_file(int *data, int size) {
    char filename[100];
    printf("Enter filename to save: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to save file.\n");
        return;
    }

    for (int i = 0; i < size; i++)
        fprintf(fp, "%d\n", data[i]);

    fclose(fp);
    printf("Saved %d values.\n", size);
}
