#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_COURSE 50
#define MAX_GRADES 10
#define FILE_NAME "students.dat"

// Struct
typedef struct {
  int id;
  char name[MAX_NAME];
  int age;
  char course[MAX_COURSE];
  float grade[MAX_GRADES];
  int gradeCount;
  float gpa;
} Student;

// global dynamic array
Student *students = NULL;
int studentCount = 0;
int autoID = 1;

void loadFromFile(){
  printf("Welcome to the Student Management System\n");
  printf("Loading data from existing file...\n");
  FILE *fp = fopen(FILE_NAME, "rb");
  if (!fp) {
    printf("No existing file. No data to be loaded.\n");
    return;
  }
  fread(&studentCount, sizeof(int), 1, fp);
  if (studentCount > 0) {
      students = malloc(studentCount * sizeof(Student));
      fread(students, sizeof(Student), studentCount, fp);
      autoID = students[studentCount - 1].id + 1;
  }

  fclose(fp);
  printf("Data loaded successfully.\n");
}

void saveToFile(){
  FILE *fp = fopen(FILE_NAME, "wb");
  if (!fp) {
    printf("Error saving to file.\n");
    return;
  }

  fwrite(&studentCount, sizeof(int), 1, fp);
  fwrite(students, sizeof(Student), studentCount, fp);

  fclose(fp);
  printf("Data saved succesfully.\n");
}

// calculate student gpa
void computeGPA(Student *s) {
    if (s->gradeCount == 0) { s->gpa = 0; return; }

    float sum = 0;
    for (int i = 0; i < s->gradeCount; i++)
        sum += s->grade[i];

    s->gpa = sum / s->gradeCount;
}

int searchByID(int id) {
    for (int i = 0; i < studentCount; i++)
        if (students[i].id == id)
            return i;
    return -1;
}

void addStudent() {
    students = realloc(students, (studentCount + 1) * sizeof(Student));
    Student *s = &students[studentCount];

    s->id = autoID++;
    printf("Assigned Auto ID: %d\n", s->id);

    printf("Enter Name: ");
    getchar();
    fgets(s->name, MAX_NAME, stdin);
    s->name[strcspn(s->name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &s->age);

    printf("Enter Course: ");
    getchar();
    fgets(s->course, MAX_COURSE, stdin);
    s->course[strcspn(s->course, "\n")] = 0;

    printf("Enter number of grades: ");
    scanf("%d", &s->gradeCount);

    for (int i = 0; i < s->gradeCount; i++) {
        printf("Grade %d: ", i + 1);
        scanf("%f", &s->grade[i]);
    }

    computeGPA(s);

    studentCount++;
    saveToFile();
}


// Display all student records
void displayStudents() {
    if (studentCount == 0) {
        printf("No students to display.\n");
        return;
    }

    printf("\n--- Student List ---\n");
    for (int i = 0; i < studentCount; i++) {
        Student s = students[i];
        printf("\nID: %d\nName: %s\nAge: %d\nCourse: %s\nGPA: %.2f\n",
               s.id, s.name, s.age, s.course, s.gpa);
    }
}


int searchByName(char *name) {
    for (int i = 0; i < studentCount; i++) {

        // case-insensitive compare
        if (strcasestr(students[i].name, name) != NULL) {

            // Print full student details
            printf("\n--- Student Found ---\n");
            printf("ID: %d\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("Age: %d\n", students[i].age);
            printf("Course: %s\n", students[i].course);
            printf("GPA: %.2f\n", students[i].gpa);
            return i;   // student index
        }
    }

    printf("\nNo student found with name: %s\n", name);
    return -1;
}

// input function to handle search by name
void search(){

  char nameSearch[50];

  printf("Enter name to search: ");
  getchar();
  fgets(nameSearch, 50, stdin);
  nameSearch[strcspn(nameSearch, "\n")] = 0;

  searchByName(nameSearch);

}



// UPDATE STUDENT
void updateStudent() {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);

    int pos = searchByID(id);
    if (pos == -1) {
        printf("Student not found.\n");
        return;
    }

    Student *s = &students[pos];

    printf("Enter new age: ");
    scanf("%d", &s->age);

    printf("Updating grades...\n");
    printf("Number of grades: ");
    scanf("%d", &s->gradeCount);

    for (int i = 0; i < s->gradeCount; i++) {
        printf("Grade %d: ", i + 1);
        scanf("%f", &s->grade[i]);
    }

  // calculates new gpa and updates it in the file
    computeGPA(s);
    saveToFile();
}

// DELETE STUDENT
void deleteStudent() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    int pos = searchByID(id);
    if (pos == -1) {
        printf("Student not found.\n");
        return;
    }

    for (int i = pos; i < studentCount - 1; i++)
        students[i] = students[i + 1];

    studentCount--;
    students = realloc(students, studentCount * sizeof(Student));

    saveToFile();
}

void computeStatistics() {
    if (studentCount == 0) return;

    float sum = 0, max = 0, min = 999;
    float gp[studentCount];

    for (int i = 0; i < studentCount; i++) {
        float g = students[i].gpa;
        sum += g;
        if (g > max) max = g;
        if (g < min) min = g;
        gp[i] = g;
    }

    // Median (simple sort)
    for (int i = 0; i < studentCount - 1; i++)
        for (int j = 0; j < studentCount - i - 1; j++)
            if (gp[j] > gp[j+1]) {
                float temp = gp[j];
                gp[j] = gp[j+1];
                gp[j+1] = temp;
            }

    float median = gp[studentCount / 2];
    float avg = sum / studentCount;

    printf("\n--- STATISTICS ---\n");
    printf("Class Average GPA: %.2f\n", avg);
    printf("Median GPA: %.2f\n", median);
    printf("Highest GPA: %.2f\n", max);
    printf("Lowest GPA: %.2f\n", min);
}

// MAIN MENU

int main() {
    loadFromFile();

    int choice;
    do {
        printf("\n--- Student Management System ---\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Search Student by name\n");
        printf("6. Statistics (Avg, Median, High/Low)\n");
        printf("7. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: search(); break;
            case 6: computeStatistics(); break;
            case 7: saveToFile(); printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (choice != 7);

    free(students);
    return 0;
}
