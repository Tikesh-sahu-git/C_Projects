#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 5
#define FILENAME "student_data.dat"
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

typedef struct {
    int rollNumber;
    char name[50];
    int age;
    char gender;
    char course[50];
    int semester;
    float marks[MAX_SUBJECTS];
    int attendance;
    char grade;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;
const char *subjects[MAX_SUBJECTS] = {"Math", "Science", "English", "History", "Programming"};

// Function prototypes
void loadData();
void saveData();
int authenticateAdmin();
void mainMenu();
void adminMenu();
void studentMenu(int studentIndex);
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void addMarks(int studentIndex);
void viewReportCard(int studentIndex);
void updateAttendance(int studentIndex);
void calculateGrade(int studentIndex);
int findStudentByRollNumber(int rollNumber);
void clearInputBuffer();
void printStudentDetails(int index);
void printWelcomeArt();

int main() {
    loadData();
    printWelcomeArt();
    
    if (authenticateAdmin()) {
        mainMenu();
    } else {
        printf("Authentication failed. Exiting...\n");
    }
    
    saveData();
    return 0;
}

void printWelcomeArt() {
    printf("\n");
    printf("  _____ _           _            _   _             _____           _       _\n");
    printf(" / ____| |         | |          | | (_)           / ____|         (_)     | |\n");
    printf("| (___ | |_ _   _  | | ___   ___| |_ _  ___  _ __| (___   ___ _ __ _ _ __ | |_\n");
    printf(" \\___ \\| __| | | | | |/ _ \\ / __| __| |/ _ \\| '__|\\___ \\ / __| '__| | '_ \\| __|\n");
    printf(" ____) | |_| |_| | | | (_) | (__| |_| | (_) | |  ____) | (__| |  | | |_) | |_\n");
    printf("|_____/ \\__|\\__, | |_|\\___/ \\___|\\__|_|\\___/|_| |_____/ \\___|_|  |_| .__/ \\__|\n");
    printf("             __/ |                                                  | |\n");
    printf("            |___/                                                   |_|\n");
    printf("\n");
}

void loadData() {
    FILE *file = fopen(FILENAME, "rb");
    if (file != NULL) {
        studentCount = fread(students, sizeof(Student), MAX_STUDENTS, file);
        fclose(file);
    }
}

void saveData() {
    FILE *file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(students, sizeof(Student), studentCount, file);
        fclose(file);
    }
}

int authenticateAdmin() {
    char username[50];
    char password[50];
    
    printf("\n===== ADMIN LOGIN =====\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
    
    if (strcmp(username, ADMIN_USERNAME) == 0 && 
        strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("\nLogin successful! Welcome, Admin.\n");
        return 1;
    }
    
    return 0;
}

void mainMenu() {
    int choice;
    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Admin Functions\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");
        printf("====================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: adminMenu(); break;
            case 2: {
                int rollNumber;
                printf("Enter your roll number: ");
                scanf("%d", &rollNumber);
                clearInputBuffer();
                
                int studentIndex = findStudentByRollNumber(rollNumber);
                if (studentIndex != -1) {
                    studentMenu(studentIndex);
                } else {
                    printf("Student not found!\n");
                }
                break;
            }
            case 3: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 3);
}

void adminMenu() {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student Record\n");
        printf("5. Delete Student Record\n");
        printf("6. Add/Update Marks\n");
        printf("7. Update Attendance\n");
        printf("8. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: {
                int rollNumber;
                printf("Enter student roll number: ");
                scanf("%d", &rollNumber);
                clearInputBuffer();
                
                int index = findStudentByRollNumber(rollNumber);
                if (index != -1) {
                    addMarks(index);
                } else {
                    printf("Student not found!\n");
                }
                break;
            }
            case 7: {
                int rollNumber;
                printf("Enter student roll number: ");
                scanf("%d", &rollNumber);
                clearInputBuffer();
                
                int index = findStudentByRollNumber(rollNumber);
                if (index != -1) {
                    updateAttendance(index);
                } else {
                    printf("Student not found!\n");
                }
                break;
            }
            case 8: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 8);
}

void studentMenu(int studentIndex) {
    printf("\nWelcome, %s!\n", students[studentIndex].name);
    
    int choice;
    do {
        printf("\n===== STUDENT MENU =====\n");
        printf("1. View My Details\n");
        printf("2. View Report Card\n");
        printf("3. View Attendance\n");
        printf("4. Back to Main Menu\n");
        printf("========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: printStudentDetails(studentIndex); break;
            case 2: viewReportCard(studentIndex); break;
            case 3: 
                printf("\nAttendance: %d%%\n", students[studentIndex].attendance); 
                break;
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Maximum number of students reached!\n");
        return;
    }
    
    Student newStudent;
    
    printf("\nEnter student details:\n");
    
    printf("Roll Number: ");
    scanf("%d", &newStudent.rollNumber);
    clearInputBuffer();
    
    // Check if roll number already exists
    if (findStudentByRollNumber(newStudent.rollNumber) != -1) {
        printf("Student with this roll number already exists!\n");
        return;
    }
    
    printf("Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
    
    printf("Age: ");
    scanf("%d", &newStudent.age);
    clearInputBuffer();
    
    printf("Gender (M/F): ");
    scanf("%c", &newStudent.gender);
    clearInputBuffer();
    newStudent.gender = toupper(newStudent.gender);
    
    printf("Course: ");
    fgets(newStudent.course, sizeof(newStudent.course), stdin);
    newStudent.course[strcspn(newStudent.course, "\n")] = '\0';
    
    printf("Semester: ");
    scanf("%d", &newStudent.semester);
    clearInputBuffer();
    
    // Initialize marks to 0
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        newStudent.marks[i] = 0;
    }
    
    newStudent.attendance = 0;
    newStudent.grade = 'N'; // 'N' for Not Available
    
    students[studentCount++] = newStudent;
    
    printf("\nStudent added successfully!\n");
}

void displayAllStudents() {
    printf("\n===== ALL STUDENTS =====\n");
    printf("%-10s %-20s %-5s %-5s %-15s %-5s %-5s %s\n", 
           "Roll No.", "Name", "Age", "Gen", "Course", "Sem", "Att%", "Grade");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-20s %-5d %-5c %-15s %-5d %-5d %c\n", 
               students[i].rollNumber,
               students[i].name,
               students[i].age,
               students[i].gender,
               students[i].course,
               students[i].semester,
               students[i].attendance,
               students[i].grade);
    }
}

void searchStudent() {
    int rollNumber;
    printf("\nEnter roll number to search: ");
    scanf("%d", &rollNumber);
    clearInputBuffer();
    
    int index = findStudentByRollNumber(rollNumber);
    if (index != -1) {
        printStudentDetails(index);
    } else {
        printf("Student not found!\n");
    }
}

void updateStudent() {
    int rollNumber;
    printf("\nEnter roll number to update: ");
    scanf("%d", &rollNumber);
    clearInputBuffer();
    
    int index = findStudentByRollNumber(rollNumber);
    if (index != -1) {
        printf("\nCurrent student details:\n");
        printStudentDetails(index);
        
        printf("\nEnter new details (leave blank to keep current):\n");
        
        char input[100];
        int intInput;
        float floatInput;
        
        printf("Name [%s]: ", students[index].name);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strcpy(students[index].name, input);
        }
        
        printf("Age [%d]: ", students[index].age);
        if (scanf("%d", &intInput) == 1) {
            students[index].age = intInput;
        }
        clearInputBuffer();
        
        printf("Gender [%c]: ", students[index].gender);
        fgets(input, sizeof(input), stdin);
        if (strlen(input) > 1) { // More than just the newline
            students[index].gender = toupper(input[0]);
        }
        
        printf("Course [%s]: ", students[index].course);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strcpy(students[index].course, input);
        }
        
        printf("Semester [%d]: ", students[index].semester);
        if (scanf("%d", &intInput) == 1) {
            students[index].semester = intInput;
        }
        clearInputBuffer();
        
        printf("\nStudent record updated successfully!\n");
    } else {
        printf("Student not found!\n");
    }
}

void deleteStudent() {
    int rollNumber;
    printf("\nEnter roll number to delete: ");
    scanf("%d", &rollNumber);
    clearInputBuffer();
    
    int index = findStudentByRollNumber(rollNumber);
    if (index != -1) {
        // Shift all students after this one forward
        for (int i = index; i < studentCount - 1; i++) {
            students[i] = students[i + 1];
        }
        studentCount--;
        printf("Student deleted successfully!\n");
    } else {
        printf("Student not found!\n");
    }
}

void addMarks(int studentIndex) {
    printf("\n===== ENTER MARKS FOR %s =====\n", students[studentIndex].name);
    
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("%s: ", subjects[i]);
        scanf("%f", &students[studentIndex].marks[i]);
        clearInputBuffer();
        
        // Validate marks (0-100)
        while (students[studentIndex].marks[i] < 0 || students[studentIndex].marks[i] > 100) {
            printf("Invalid marks! Enter again (0-100): ");
            scanf("%f", &students[studentIndex].marks[i]);
            clearInputBuffer();
        }
    }
    
    calculateGrade(studentIndex);
    printf("\nMarks added successfully!\n");
}

void viewReportCard(int studentIndex) {
    printf("\n===== REPORT CARD =====\n");
    printf("Name: %s\n", students[studentIndex].name);
    printf("Roll Number: %d\n", students[studentIndex].rollNumber);
    printf("Course: %s, Semester: %d\n", students[studentIndex].course, students[studentIndex].semester);
    printf("\n%-15s %s\n", "Subject", "Marks");
    printf("----------------------\n");
    
    float total = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("%-15s %.2f\n", subjects[i], students[studentIndex].marks[i]);
        total += students[studentIndex].marks[i];
    }
    
    float percentage = total / MAX_SUBJECTS;
    printf("\nTotal Marks: %.2f/500\n", total);
    printf("Percentage: %.2f%%\n", percentage);
    printf("Grade: %c\n", students[studentIndex].grade);
}

void updateAttendance(int studentIndex) {
    printf("\nCurrent attendance for %s: %d%%\n", 
           students[studentIndex].name, students[studentIndex].attendance);
    printf("Enter new attendance percentage (0-100): ");
    
    int attendance;
    scanf("%d", &attendance);
    clearInputBuffer();
    
    while (attendance < 0 || attendance > 100) {
        printf("Invalid attendance! Enter again (0-100): ");
        scanf("%d", &attendance);
        clearInputBuffer();
    }
    
    students[studentIndex].attendance = attendance;
    printf("Attendance updated successfully!\n");
}

void calculateGrade(int studentIndex) {
    float total = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        total += students[studentIndex].marks[i];
    }
    float percentage = total / MAX_SUBJECTS;
    
    if (percentage >= 90) students[studentIndex].grade = 'A';
    else if (percentage >= 80) students[studentIndex].grade = 'B';
    else if (percentage >= 70) students[studentIndex].grade = 'C';
    else if (percentage >= 60) students[studentIndex].grade = 'D';
    else if (percentage >= 50) students[studentIndex].grade = 'E';
    else students[studentIndex].grade = 'F';
}

int findStudentByRollNumber(int rollNumber) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].rollNumber == rollNumber) {
            return i;
        }
    }
    return -1;
}

void printStudentDetails(int index) {
    printf("\n===== STUDENT DETAILS =====\n");
    printf("Roll Number: %d\n", students[index].rollNumber);
    printf("Name: %s\n", students[index].name);
    printf("Age: %d, Gender: %c\n", students[index].age, students[index].gender);
    printf("Course: %s, Semester: %d\n", students[index].course, students[index].semester);
    printf("Attendance: %d%%\n", students[index].attendance);
    printf("Grade: %c\n", students[index].grade);
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
