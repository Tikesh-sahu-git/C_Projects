#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// Define maximum capacities for patients, doctors, and appointments
#define MAX_PATIENTS 100
#define MAX_DOCTORS 20
#define MAX_APPOINTMENTS 200

// Define filenames for data persistence
#define FILENAME_PATIENTS "patients.dat"
#define FILENAME_DOCTORS "doctors.dat"
#define FILENAME_APPOINTMENTS "appointments.dat"

// Admin credentials for system login
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// Structure to hold patient information
typedef struct {
    int id;
    char name[50];
    char address[100];
    char phone[15];
    char email[50];
    int age;
    char gender; // 'M' or 'F'
    char bloodGroup[5];
    char medicalHistory[200];
} Patient;

// Structure to hold doctor information
typedef struct {
    int id;
    char name[50];
    char specialization[50];
    char phone[15];
    char email[50];
    char availableDays[50];
    char availableHours[50];
    int fee;
} Doctor;

// Structure to hold appointment information
typedef struct {
    int id;
    int patientId;
    int doctorId;
    char date[20];
    char time[10];
    char purpose[100];
    char status[20]; // "Scheduled", "Completed", "Cancelled"
} Appointment;

// Global arrays to store data in memory
Patient patients[MAX_PATIENTS];
Doctor doctors[MAX_DOCTORS];
Appointment appointments[MAX_APPOINTMENTS];

// Global counters for the number of records
int patientCount = 0;
int doctorCount = 0;
int appointmentCount = 0;

// --- Function Prototypes ---

// Data management functions
void loadData(); // Loads data from binary files into memory
void saveData(); // Saves data from memory to binary files

// Authentication and menu functions
int authenticateAdmin(); // Authenticates the admin user
void mainMenu();        // Displays the main menu for different user roles
void adminMenu();       // Displays the admin functionalities menu
void doctorMenu(int doctorId); // Displays the doctor specific functionalities menu
void patientMenu(int patientId); // Displays the patient specific functionalities menu

// Patient management functions
void addPatient();         // Adds a new patient record
void displayAllPatients(); // Displays all registered patients
void searchPatient();      // Searches for a patient by name or ID
void updatePatient();      // Updates an existing patient record
void deletePatient();      // Deletes a patient record

// Doctor management functions
void addDoctor();         // Adds a new doctor record
void displayAllDoctors(); // Displays all registered doctors
void searchDoctor();      // Searches for a doctor by name, specialization, or ID
void updateDoctor();      // Updates an existing doctor record
void deleteDoctor();      // Deletes a doctor record

// Appointment management functions
void scheduleAppointment();      // Schedules a new appointment
void viewAppointments();         // Displays all appointments
void updateAppointmentStatus();  // Updates the status of an appointment
void cancelAppointment();        // Cancels an existing appointment

// Specific view functions for doctor and patient roles
void viewDoctorSchedule(int doctorId); // Displays a doctor's scheduled appointments
void viewPatientHistory(int patientId); // Displays a patient's medical history and past appointments

// Helper functions for finding records by ID
int findPatientById(int id);    // Finds a patient's index by their ID
int findDoctorById(int id);     // Finds a doctor's index by their ID
int findAppointmentById(int id); // Finds an appointment's index by its ID

// Utility functions
void clearInputBuffer(); // Clears the standard input buffer
void printWelcomeArt();  // Prints ASCII art for welcome message

// --- Helper Menu Functions (for better menu navigation) ---
void patientManagementMenu();
void doctorManagementMenu();
void voidappointmentManagementMenu();


int main() {
    // Load existing data at the start of the program
    loadData();
    // Print a welcome message with ASCII art
    printWelcomeArt();
    
    // Authenticate the admin before proceeding to the main menu
    if (authenticateAdmin()) {
        mainMenu(); // If authenticated, display the main menu
    } else {
        printf("Authentication failed. Exiting...\n"); // If authentication fails, exit
    }
    
    // Save all current data before the program exits
    saveData();
    return 0;
}

// Function to print ASCII art welcome message
void printWelcomeArt() {
    printf("\n");
    printf("  _    _          _      _          _____          _      _\n");
    printf(" | |  | |        | |    | |        / ____|        | |    | |\n");
    printf(" | |__| | ___  ___| |_ ___| |_ _ __ ___| (___  ___ _ __| |_ ___| |_\n");
    printf(" |  __  |/ _ \\/ __| __/ _ \\ __| '__/ _ \\\\___ \\ / _ \\ '__| __/ __| __|\n");
    printf(" | |  | | (_) \\__ \\ ||  __/ |_| | | (_) |___) |  __/ |  | |_\\__ \\ |_\n");
    printf(" |_|  |_|\\___/|___/\\__\\___|\\__|_|_\\___/_____/ \\___|_|   \\__|___/\\__|\n");
    printf("\n");
}

// Function to load data from binary files
void loadData() {
    FILE *file;
    
    // Load patients from file
    file = fopen(FILENAME_PATIENTS, "rb");
    if (file != NULL) {
        patientCount = fread(patients, sizeof(Patient), MAX_PATIENTS, file);
        fclose(file);
    }
    
    // Load doctors from file
    file = fopen(FILENAME_DOCTORS, "rb");
    if (file != NULL) {
        doctorCount = fread(doctors, sizeof(Doctor), MAX_DOCTORS, file);
        fclose(file);
    }
    
    // Load appointments from file
    file = fopen(FILENAME_APPOINTMENTS, "rb");
    if (file != NULL) {
        appointmentCount = fread(appointments, sizeof(Appointment), MAX_APPOINTMENTS, file);
        fclose(file);
    }
}

// Function to save data to binary files
void saveData() {
    FILE *file;
    
    // Save patients to file
    file = fopen(FILENAME_PATIENTS, "wb");
    if (file != NULL) {
        fwrite(patients, sizeof(Patient), patientCount, file);
        fclose(file);
    }
    
    // Save doctors to file
    file = fopen(FILENAME_DOCTORS, "wb");
    if (file != NULL) {
        fwrite(doctors, sizeof(Doctor), doctorCount, file);
        fclose(file);
    }
    
    // Save appointments to file
    file = fopen(FILENAME_APPOINTMENTS, "wb");
    if (file != NULL) {
        fwrite(appointments, sizeof(Appointment), appointmentCount, file);
        fclose(file);
    }
}

// Function to authenticate admin user
int authenticateAdmin() {
    char username[50];
    char password[50];
    
    printf("\n===== ADMIN LOGIN =====\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove newline character
    
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove newline character
    
    // Compare entered credentials with predefined admin credentials
    if (strcmp(username, ADMIN_USERNAME) == 0 && 
        strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("\nLogin successful! Welcome, Admin.\n");
        return 1; // Authentication successful
    }
    
    return 0; // Authentication failed
}

// Main menu for navigating different user roles
void mainMenu() {
    int choice;
    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Admin Functions\n");
        printf("2. Doctor Login\n");
        printf("3. Patient Login\n");
        printf("4. Exit\n");
        printf("====================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer(); // Clear input buffer after reading integer
        
        switch(choice) {
            case 1: adminMenu(); break; // Go to admin menu
            case 2: {
                int doctorId;
                printf("Enter your doctor ID: ");
                scanf("%d", &doctorId);
                clearInputBuffer();
                
                int doctorIndex = findDoctorById(doctorId);
                if (doctorIndex != -1) {
                    doctorMenu(doctorId); // Go to doctor menu if ID found
                } else {
                    printf("Doctor not found!\n");
                }
                break;
            }
            case 3: {
                int patientId;
                printf("Enter your patient ID: ");
                scanf("%d", &patientId);
                clearInputBuffer();
                
                int patientIndex = findPatientById(patientId);
                if (patientIndex != -1) {
                    patientMenu(patientId); // Go to patient menu if ID found
                } else {
                    printf("Patient not found!\n");
                }
                break;
            }
            case 4: printf("Exiting...\n"); break; // Exit the program
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

// Admin menu functionalities
void adminMenu() {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Patient Management\n");
        printf("2. Doctor Management\n");
        printf("3. Appointment Management\n");
        printf("4. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: patientManagementMenu(); break;      // Go to patient management
            case 2: doctorManagementMenu(); break;       // Go to doctor management
            case 3: appointmentManagementMenu(); break;  // Go to appointment management
            case 4: printf("Returning to main menu...\n"); break; // Return to main menu
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

// Patient management menu (accessed by admin)
void patientManagementMenu() {
    int choice;
    do {
        printf("\n===== PATIENT MANAGEMENT =====\n");
        printf("1. Add New Patient\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Update Patient Record\n");
        printf("5. Delete Patient Record\n");
        printf("6. Back to Admin Menu\n");
        printf("=============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addPatient(); break;
            case 2: displayAllPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: printf("Returning to admin menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
}

// Doctor management menu (accessed by admin)
void doctorManagementMenu() {
    int choice;
    do {
        printf("\n===== DOCTOR MANAGEMENT =====\n");
        printf("1. Add New Doctor\n");
        printf("2. View All Doctors\n");
        printf("3. Search Doctor\n");
        printf("4. Update Doctor Record\n");
        printf("5. Delete Doctor Record\n");
        printf("6. Back to Admin Menu\n");
        printf("============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addDoctor(); break;
            case 2: displayAllDoctors(); break;
            case 3: searchDoctor(); break;
            case 4: updateDoctor(); break;
            case 5: deleteDoctor(); break;
            case 6: printf("Returning to admin menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
}

// Appointment management menu (accessed by admin)
void appointmentManagementMenu() {
    int choice;
    do {
        printf("\n===== APPOINTMENT MANAGEMENT =====\n");
        printf("1. Schedule Appointment\n");
        printf("2. View All Appointments\n");
        printf("3. Update Appointment Status\n");
        printf("4. Cancel Appointment\n");
        printf("5. Back to Admin Menu\n");
        printf("=================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: scheduleAppointment(); break;
            case 2: viewAppointments(); break;
            case 3: updateAppointmentStatus(); break;
            case 4: cancelAppointment(); break;
            case 5: printf("Returning to admin menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 5);
}

// Doctor specific menu
void doctorMenu(int doctorId) {
    int doctorIndex = findDoctorById(doctorId);
    if (doctorIndex == -1) {
        printf("Doctor not found!\n");
        return;
    }
    
    printf("\nWelcome, Dr. %s!\n", doctors[doctorIndex].name);
    
    int choice;
    do {
        printf("\n===== DOCTOR MENU =====\n");
        printf("1. View My Schedule\n");
        printf("2. Update Appointment Status\n");
        printf("3. View Patient History\n");
        printf("4. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: viewDoctorSchedule(doctorId); break;
            case 2: updateAppointmentStatus(); break;
            case 3: {
                int patientId;
                printf("Enter patient ID: ");
                scanf("%d", &patientId);
                clearInputBuffer();
                viewPatientHistory(patientId); // Allows doctor to view specific patient's history
                break;
            }
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

// Patient specific menu
void patientMenu(int patientId) {
    int patientIndex = findPatientById(patientId);
    if (patientIndex == -1) {
        printf("Patient not found!\n");
        return;
    }
    
    printf("\nWelcome, %s!\n", patients[patientIndex].name);
    
    int choice;
    do {
        printf("\n===== PATIENT MENU =====\n");
        printf("1. View My Appointments\n");
        printf("2. Schedule Appointment\n");
        printf("3. Cancel Appointment\n");
        printf("4. View My Medical History\n");
        printf("5. Back to Main Menu\n");
        printf("=======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: {
                printf("\n===== YOUR APPOINTMENTS =====\n");
                bool found = false;
                for (int i = 0; i < appointmentCount; i++) {
                    if (appointments[i].patientId == patientId) {
                        int doctorIndex = findDoctorById(appointments[i].doctorId);
                        char doctorName[50] = "Unknown"; // Default to "Unknown"
                        if (doctorIndex != -1) {
                            strcpy(doctorName, doctors[doctorIndex].name);
                        }
                        printf("ID: %d, Date: %s, Time: %s\n", appointments[i].id, 
                               appointments[i].date, appointments[i].time);
                        printf("Doctor: %s, Purpose: %s, Status: %s\n\n", 
                               doctorName, 
                               appointments[i].purpose, 
                               appointments[i].status);
                        found = true;
                    }
                }
                if (!found) {
                    printf("No appointments found.\n");
                }
                break;
            }
            case 2: scheduleAppointment(); break;
            case 3: cancelAppointment(); break;
            case 4: viewPatientHistory(patientId); break; // Patient can view their own medical history
            case 5: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 5);
}

// Function to add a new patient
void addPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("Maximum number of patients reached!\n");
        return;
    }
    
    Patient newPatient;
    
    printf("\nEnter patient details:\n");
    
    // Generate a simple ID (starts from 1000)
    newPatient.id = 1000 + patientCount; 
    
    printf("Name: ");
    fgets(newPatient.name, sizeof(newPatient.name), stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = '\0'; // Remove newline
    
    printf("Address: ");
    fgets(newPatient.address, sizeof(newPatient.address), stdin);
    newPatient.address[strcspn(newPatient.address, "\n")] = '\0';
    
    printf("Phone: ");
    fgets(newPatient.phone, sizeof(newPatient.phone), stdin);
    newPatient.phone[strcspn(newPatient.phone, "\n")] = '\0';
    
    printf("Email: ");
    fgets(newPatient.email, sizeof(newPatient.email), stdin);
    newPatient.email[strcspn(newPatient.email, "\n")] = '\0';
    
    printf("Age: ");
    scanf("%d", &newPatient.age);
    clearInputBuffer();
    
    printf("Gender (M/F): ");
    scanf("%c", &newPatient.gender);
    clearInputBuffer();
    newPatient.gender = toupper(newPatient.gender); // Convert to uppercase
    
    printf("Blood Group: ");
    fgets(newPatient.bloodGroup, sizeof(newPatient.bloodGroup), stdin);
    newPatient.bloodGroup[strcspn(newPatient.bloodGroup, "\n")] = '\0';
    
    printf("Medical History (brief): ");
    fgets(newPatient.medicalHistory, sizeof(newPatient.medicalHistory), stdin);
    newPatient.medicalHistory[strcspn(newPatient.medicalHistory, "\n")] = '\0';
    
    // Add the new patient to the array and increment count
    patients[patientCount++] = newPatient;
    
    printf("\nPatient added successfully!\n");
    printf("Patient ID: %d\n", newPatient.id);
}

// Function to display all patients
void displayAllPatients() {
    if (patientCount == 0) {
        printf("\nNo patients found.\n");
        return;
    }
    
    printf("\n===== ALL PATIENTS =====\n");
    printf("%-6s %-20s %-5s %-5s %-15s %-30s\n", 
           "ID", "Name", "Age", "Gen", "Phone", "Email");
    printf("------------------------------------------------------------------------\n");
    
    for (int i = 0; i < patientCount; i++) {
        printf("%-6d %-20s %-5d %-5c %-15s %-30s\n", 
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].gender,
               patients[i].phone,
               patients[i].email);
    }
}

// Function to search for a patient
void searchPatient() {
    if (patientCount == 0) {
        printf("\nNo patients to search.\n");
        return;
    }
    
    char searchTerm[50];
    printf("\nEnter patient name or ID to search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\n===== SEARCH RESULTS =====\n");
    printf("%-6s %-20s %-5s %-5s %-15s %-30s\n", 
           "ID", "Name", "Age", "Gen", "Phone", "Email");
    printf("------------------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < patientCount; i++) {
        // Search by name (substring) or by ID (if input is numeric)
        if (strstr(patients[i].name, searchTerm) != NULL || 
            (isdigit(searchTerm[0]) && patients[i].id == atoi(searchTerm))) {
            printf("%-6d %-20s %-5d %-5c %-15s %-30s\n", 
                   patients[i].id,
                   patients[i].name,
                   patients[i].age,
                   patients[i].gender,
                   patients[i].phone,
                   patients[i].email);
            found = true;
        }
    }
    
    if (!found) {
        printf("No patients found matching your search.\n");
    }
}

// Function to update patient details
void updatePatient() {
    if (patientCount == 0) {
        printf("\nNo patients to update.\n");
        return;
    }
    
    int patientId;
    printf("\nEnter patient ID to update: ");
    scanf("%d", &patientId);
    clearInputBuffer();
    
    int index = findPatientById(patientId);
    if (index == -1) {
        printf("Patient not found!\n");
        return;
    }
    
    printf("\nCurrent patient details:\n");
    printf("Name: %s\n", patients[index].name);
    printf("Address: %s\n", patients[index].address);
    printf("Phone: %s\n", patients[index].phone);
    printf("Email: %s\n", patients[index].email);
    printf("Age: %d\n", patients[index].age);
    printf("Gender: %c\n", patients[index].gender);
    printf("Blood Group: %s\n", patients[index].bloodGroup);
    printf("Medical History: %s\n", patients[index].medicalHistory);
    
    printf("\nEnter new details (leave blank to keep current):\n");
    
    char input[100];
    int intInput;
    
    printf("Name [%s]: ", patients[index].name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].name, input);
    }
    
    printf("Address [%s]: ", patients[index].address);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].address, input);
    }
    
    printf("Phone [%s]: ", patients[index].phone);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].phone, input);
    }
    
    printf("Email [%s]: ", patients[index].email);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].email, input);
    }
    
    printf("Age [%d]: ", patients[index].age);
    // Check if scanf successfully read an integer
    if (scanf("%d", &intInput) == 1) { 
        patients[index].age = intInput;
    }
    clearInputBuffer();
    
    printf("Gender [%c]: ", patients[index].gender);
    fgets(input, sizeof(input), stdin);
    // Check for at least one character + newline before trying to assign
    if (strlen(input) > 1) { 
        patients[index].gender = toupper(input[0]);
    }
    
    printf("Blood Group [%s]: ", patients[index].bloodGroup);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].bloodGroup, input);
    }
    
    printf("Medical History [%s]: ", patients[index].medicalHistory);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].medicalHistory, input);
    }
    
    printf("\nPatient record updated successfully!\n");
}

// Function to delete a patient record
void deletePatient() {
    if (patientCount == 0) {
        printf("\nNo patients to delete.\n");
        return;
    }
    
    int patientId;
    printf("\nEnter patient ID to delete: ");
    scanf("%d", &patientId);
    clearInputBuffer();
    
    int index = findPatientById(patientId);
    if (index == -1) {
        printf("Patient not found!\n");
        return;
    }
    
    printf("\nDeleting patient:\n");
    printf("Name: %s\n", patients[index].name);
    printf("ID: %d\n", patients[index].id);
    
    // Shift elements to the left to overwrite the deleted patient
    for (int i = index; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }
    patientCount--; // Decrement patient count
    
    printf("\nPatient deleted successfully!\n");
}

// Function to add a new doctor
void addDoctor() {
    if (doctorCount >= MAX_DOCTORS) {
        printf("Maximum number of doctors reached!\n");
        return;
    }
    
    Doctor newDoctor;
    
    printf("\nEnter doctor details:\n");
    
    // Generate a simple ID (starts from 2000)
    newDoctor.id = 2000 + doctorCount; 
    
    printf("Name: ");
    fgets(newDoctor.name, sizeof(newDoctor.name), stdin);
    newDoctor.name[strcspn(newDoctor.name, "\n")] = '\0';
    
    printf("Specialization: ");
    fgets(newDoctor.specialization, sizeof(newDoctor.specialization), stdin);
    newDoctor.specialization[strcspn(newDoctor.specialization, "\n")] = '\0';
    
    printf("Phone: ");
    fgets(newDoctor.phone, sizeof(newDoctor.phone), stdin);
    newDoctor.phone[strcspn(newDoctor.phone, "\n")] = '\0';
    
    printf("Email: ");
    fgets(newDoctor.email, sizeof(newDoctor.email), stdin);
    newDoctor.email[strcspn(newDoctor.email, "\n")] = '\0';
    
    printf("Available Days (e.g., Mon-Fri): ");
    fgets(newDoctor.availableDays, sizeof(newDoctor.availableDays), stdin);
    newDoctor.availableDays[strcspn(newDoctor.availableDays, "\n")] = '\0';
    
    printf("Available Hours (e.g., 9AM-5PM): ");
    fgets(newDoctor.availableHours, sizeof(newDoctor.availableHours), stdin);
    newDoctor.availableHours[strcspn(newDoctor.availableHours, "\n")] = '\0';
    
    printf("Consultation Fee: ");
    scanf("%d", &newDoctor.fee);
    clearInputBuffer();
    
    doctors[doctorCount++] = newDoctor; // Add new doctor and increment count
    
    printf("\nDoctor added successfully!\n");
    printf("Doctor ID: %d\n", newDoctor.id);
}

// Function to display all doctors
void displayAllDoctors() {
    if (doctorCount == 0) {
        printf("\nNo doctors found.\n");
        return;
    }
    
    printf("\n===== ALL DOCTORS =====\n");
    printf("%-6s %-20s %-20s %-15s %-10s %-15s %s\n", 
           "ID", "Name", "Specialization", "Phone", "Fee", "Days", "Hours");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < doctorCount; i++) {
        printf("%-6d %-20s %-20s %-15s $%-9d %-15s %s\n", 
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].phone,
               doctors[i].fee,
               doctors[i].availableDays,
               doctors[i].availableHours);
    }
}

// Function to search for a doctor
void searchDoctor() {
    if (doctorCount == 0) {
        printf("\nNo doctors to search.\n");
        return;
    }
    
    char searchTerm[50];
    printf("\nEnter doctor name, specialization or ID to search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\n===== SEARCH RESULTS =====\n");
    printf("%-6s %-20s %-20s %-15s %-10s %-15s %s\n", 
           "ID", "Name", "Specialization", "Phone", "Fee", "Days", "Hours");
    printf("--------------------------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < doctorCount; i++) {
        // Search by name, specialization (substrings), or by ID
        if (strstr(doctors[i].name, searchTerm) != NULL || 
            strstr(doctors[i].specialization, searchTerm) != NULL ||
            (isdigit(searchTerm[0]) && doctors[i].id == atoi(searchTerm))) {
            printf("%-6d %-20s %-20s %-15s $%-9d %-15s %s\n", 
                   doctors[i].id,
                   doctors[i].name,
                   doctors[i].specialization,
                   doctors[i].phone,
                   doctors[i].fee,
                   doctors[i].availableDays,
                   doctors[i].availableHours);
            found = true;
        }
    }
    
    if (!found) {
        printf("No doctors found matching your search.\n");
    }
}

// Function to update doctor details
void updateDoctor() {
    if (doctorCount == 0) {
        printf("\nNo doctors to update.\n");
        return;
    }
    
    int doctorId;
    printf("\nEnter doctor ID to update: ");
    scanf("%d", &doctorId);
    clearInputBuffer();
    
    int index = findDoctorById(doctorId);
    if (index == -1) {
        printf("Doctor not found!\n");
        return;
    }
    
    printf("\nCurrent doctor details:\n");
    printf("Name: %s\n", doctors[index].name);
    printf("Specialization: %s\n", doctors[index].specialization);
    printf("Phone: %s\n", doctors[index].phone);
    printf("Email: %s\n", doctors[index].email);
    printf("Available Days: %s\n", doctors[index].availableDays);
    printf("Available Hours: %s\n", doctors[index].availableHours);
    printf("Consultation Fee: %d\n", doctors[index].fee);
    
    printf("\nEnter new details (leave blank to keep current):\n");
    
    char input[100];
    int intInput;
    
    printf("Name [%s]: ", doctors[index].name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].name, input);
    }
    
    printf("Specialization [%s]: ", doctors[index].specialization);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].specialization, input);
    }
    
    printf("Phone [%s]: ", doctors[index].phone);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].phone, input);
    }
    
    printf("Email [%s]: ", doctors[index].email);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].email, input);
    }
    
    printf("Available Days [%s]: ", doctors[index].availableDays);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].availableDays, input);
    }
    
    printf("Available Hours [%s]: ", doctors[index].availableHours);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].availableHours, input);
    }
    
    printf("Consultation Fee [%d]: ", doctors[index].fee);
    // Check if scanf successfully read an integer
    if (scanf("%d", &intInput) == 1) {
        doctors[index].fee = intInput;
    }
    clearInputBuffer();
    
    printf("\nDoctor record updated successfully!\n");
}

// Function to delete a doctor record
void deleteDoctor() {
    if (doctorCount == 0) {
        printf("\nNo doctors to delete.\n");
        return;
    }
    
    int doctorId;
    printf("\nEnter doctor ID to delete: ");
    scanf("%d", &doctorId);
    clearInputBuffer();
    
    int index = findDoctorById(doctorId);
    if (index == -1) {
        printf("Doctor not found!\n");
        return;
    }
    
    printf("\nDeleting doctor:\n");
    printf("Name: %s\n", doctors[index].name);
    printf("ID: %d\n", doctors[index].id);
    
    // Shift elements to the left to overwrite the deleted doctor
    for (int i = index; i < doctorCount - 1; i++) {
        doctors[i] = doctors[i + 1];
    }
    doctorCount--; // Decrement doctor count
    
    printf("\nDoctor deleted successfully!\n");
}

// Function to schedule a new appointment
void scheduleAppointment() {
    if (patientCount == 0 || doctorCount == 0) {
        printf("\nNot enough data to schedule an appointment. Please add patients and doctors first.\n");
        return;
    }
    
    if (appointmentCount >= MAX_APPOINTMENTS) {
        printf("Maximum number of appointments reached!\n");
        return;
    }
    
    Appointment newAppointment;
    
    printf("\nEnter appointment details:\n");
    
    // Generate a simple ID (starts from 3000)
    newAppointment.id = 3000 + appointmentCount; 
    
    printf("Patient ID: ");
    scanf("%d", &newAppointment.patientId);
    clearInputBuffer();
    
    // Validate patient ID
    if (findPatientById(newAppointment.patientId) == -1) {
        printf("Patient not found! Please enter a valid patient ID.\n");
        return;
    }
    
    printf("Doctor ID: ");
    scanf("%d", &newAppointment.doctorId);
    clearInputBuffer();
    
    // Validate doctor ID
    if (findDoctorById(newAppointment.doctorId) == -1) {
        printf("Doctor not found! Please enter a valid doctor ID.\n");
        return;
    }
    
    printf("Date (DD/MM/YYYY): ");
    fgets(newAppointment.date, sizeof(newAppointment.date), stdin);
    newAppointment.date[strcspn(newAppointment.date, "\n")] = '\0';
    
    printf("Time (HH:MM): ");
    fgets(newAppointment.time, sizeof(newAppointment.time), stdin);
    newAppointment.time[strcspn(newAppointment.time, "\n")] = '\0';
    
    printf("Purpose of visit: ");
    fgets(newAppointment.purpose, sizeof(newAppointment.purpose), stdin);
    newAppointment.purpose[strcspn(newAppointment.purpose, "\n")] = '\0';
    
    strcpy(newAppointment.status, "Scheduled"); // Default status for new appointments
    
    appointments[appointmentCount++] = newAppointment; // Add new appointment and increment count
    
    printf("\nAppointment scheduled successfully!\n");
    printf("Appointment ID: %d\n", newAppointment.id);
}

// Function to view all appointments
void viewAppointments() {
    if (appointmentCount == 0) {
        printf("\nNo appointments found.\n");
        return;
    }
    
    printf("\n===== ALL APPOINTMENTS =====\n");
    printf("%-6s %-8s %-20s %-20s %-12s %-8s %-20s %s\n", 
           "ID", "Pat.ID", "Patient Name", "Doctor Name", "Date", "Time", "Purpose", "Status");
    printf("------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < appointmentCount; i++) {
        int patientIndex = findPatientById(appointments[i].patientId);
        int doctorIndex = findDoctorById(appointments[i].doctorId);
        
        // Get patient and doctor names, default to "Unknown" if not found (e.g., if deleted)
        char patientName[50] = "Unknown";
        char doctorName[50] = "Unknown";
        
        if (patientIndex != -1) {
            strcpy(patientName, patients[patientIndex].name);
        }
        if (doctorIndex != -1) {
            strcpy(doctorName, doctors[doctorIndex].name);
        }

        printf("%-6d %-8d %-20s %-20s %-12s %-8s %-20s %s\n", 
               appointments[i].id,
               appointments[i].patientId,
               patientName,
               doctorName,
               appointments[i].date,
               appointments[i].time,
               appointments[i].purpose,
               appointments[i].status);
    }
}

// Function to update the status of an appointment
void updateAppointmentStatus() {
    if (appointmentCount == 0) {
        printf("\nNo appointments to update.\n");
        return;
    }
    
    int appointmentId;
    printf("\nEnter appointment ID to update: ");
    scanf("%d", &appointmentId);
    clearInputBuffer();
    
    int index = findAppointmentById(appointmentId);
    if (index == -1) {
        printf("Appointment not found!\n");
        return;
    }
    
    printf("\nCurrent appointment status: %s\n", appointments[index].status);
    printf("Enter new status (Scheduled/Completed/Cancelled): ");
    
    char newStatus[20];
    fgets(newStatus, sizeof(newStatus), stdin);
    newStatus[strcspn(newStatus, "\n")] = '\0';
    
    // Normalize input status (e.g., "scheduled" to "Scheduled")
    if (strlen(newStatus) > 0) {
        newStatus[0] = toupper(newStatus[0]);
        for (int i = 1; newStatus[i] != '\0'; i++) {
            newStatus[i] = tolower(newStatus[i]);
        }
    }

    // Validate the new status
    if (strcmp(newStatus, "Scheduled") == 0 || 
        strcmp(newStatus, "Completed") == 0 || 
        strcmp(newStatus, "Cancelled") == 0) {
        strcpy(appointments[index].status, newStatus);
        printf("\nAppointment status updated successfully!\n");
    } else {
        printf("\nInvalid status. No changes made. Please use 'Scheduled', 'Completed', or 'Cancelled'.\n");
    }
}

// Function to cancel an appointment
void cancelAppointment() {
    if (appointmentCount == 0) {
        printf("\nNo appointments to cancel.\n");
        return;
    }
    
    int appointmentId;
    printf("\nEnter appointment ID to cancel: ");
    scanf("%d", &appointmentId);
    clearInputBuffer();
    
    int index = findAppointmentById(appointmentId);
    if (index == -1) {
        printf("Appointment not found!\n");
        return;
    }
    
    printf("\nCanceling appointment:\n");
    printf("ID: %d, Date: %s, Time: %s\n", 
           appointments[index].id, 
           appointments[index].date, 
           appointments[index].time);
    
    // Shift elements to the left to remove the cancelled appointment
    for (int i = index; i < appointmentCount - 1; i++) {
        appointments[i] = appointments[i + 1];
    }
    appointmentCount--; // Decrement appointment count
    
    printf("\nAppointment cancelled successfully!\n");
}

// Function to view a specific doctor's schedule
void viewDoctorSchedule(int doctorId) {
    int doctorIndex = findDoctorById(doctorId);
    if (doctorIndex == -1) {
        printf("Doctor not found!\n");
        return;
    }
    
    printf("\n===== DR. %s'S SCHEDULE =====\n", doctors[doctorIndex].name);
    printf("%-6s %-20s %-12s %-8s %-20s %s\n", 
           "ID", "Patient Name", "Date", "Time", "Purpose", "Status");
    printf("----------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].doctorId == doctorId) {
            int patientIndex = findPatientById(appointments[i].patientId);
            char patientName[50] = "Unknown"; // Default to "Unknown"
            if (patientIndex != -1) {
                strcpy(patientName, patients[patientIndex].name);
            }
            printf("%-6d %-20s %-12s %-8s %-20s %s\n", 
                   appointments[i].id,
                   patientName,
                   appointments[i].date,
                   appointments[i].time,
                   appointments[i].purpose,
                   appointments[i].status);
            found = true;
        }
    }
    if (!found) {
        printf("No appointments found for this doctor.\n");
    }
}

// Function to view a patient's medical history (including their details and completed appointments)
void viewPatientHistory(int patientId) {
    int patientIndex = findPatientById(patientId);
    if (patientIndex == -1) {
        printf("Patient not found!\n");
        return;
    }

    printf("\n===== MEDICAL HISTORY for %s (ID: %d) =====\n", patients[patientIndex].name, patients[patientIndex].id);
    printf("Age: %d\n", patients[patientIndex].age);
    printf("Gender: %c\n", patients[patientIndex].gender);
    printf("Blood Group: %s\n", patients[patientIndex].bloodGroup);
    printf("Known Medical History: %s\n", patients[patientIndex].medicalHistory);

    printf("\n--- Past Appointments ---\n");
    printf("%-6s %-20s %-12s %-8s %-20s %s\n", 
           "ID", "Doctor Name", "Date", "Time", "Purpose", "Status");
    printf("----------------------------------------------------------------\n");

    bool foundAppointments = false;
    for (int i = 0; i < appointmentCount; i++) {
        // Display only completed appointments for medical history
        if (appointments[i].patientId == patientId && strcmp(appointments[i].status, "Completed") == 0) {
            int doctorIndex = findDoctorById(appointments[i].doctorId);
            char doctorName[50] = "Unknown"; // Default to "Unknown"
            if (doctorIndex != -1) {
                strcpy(doctorName, doctors[doctorIndex].name);
            }
            printf("%-6d %-20s %-12s %-8s %-20s %s\n", 
                   appointments[i].id,
                   doctorName,
                   appointments[i].date,
                   appointments[i].time,
                   appointments[i].purpose,
                   appointments[i].status);
            foundAppointments = true;
        }
    }
    if (!foundAppointments) {
        printf("No completed appointments found for this patient.\n");
    }
}

// Helper function to find a patient by ID and return their index
int findPatientById(int id) {
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

// Helper function to find a doctor by ID and return their index
int findDoctorById(int id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

// Helper function to find an appointment by ID and return its index
int findAppointmentById(int id) {
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].id == id) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

// Utility function to clear the input buffer (important after scanf)
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
