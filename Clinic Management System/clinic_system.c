#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// Define maximum capacities for patients, doctors, appointments, and medicines
#define MAX_PATIENTS 100
#define MAX_DOCTORS 20
#define MAX_APPOINTMENTS 200
#define MAX_MEDICINES 50

// Define filenames for data persistence
#define FILENAME_PATIENTS "patients.dat"
#define FILENAME_DOCTORS "doctors.dat"
#define FILENAME_APPOINTMENTS "appointments.dat"
#define FILENAME_MEDICINES "medicines.dat"

// Admin credentials for system login
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// Structure to hold patient information
typedef struct {
    int id;
    char name[50];
    char address[100];
    char phone[15];
    int age;
    char gender; // 'M' or 'F'
    char bloodGroup[5];
    char allergies[200];      // New field
    char medicalHistory[500]; // Increased size
} Patient;

// Structure to hold doctor information
typedef struct {
    int id;
    char name[50];
    char specialization[50];
    char phone[15];
    char schedule[100];      // Changed from availableDays/Hours to single schedule string
    int consultationFee;     // Changed name from fee
} Doctor;

// Structure to hold appointment information
typedef struct {
    int id;
    int patientId;
    int doctorId;
    char date[20]; // DD/MM/YYYY format
    char time[10]; // HH:MM format
    char diagnosis[200];    // New field
    char prescription[500]; // New field, increased size
    float fee;              // Changed name from purpose, now stores total fee
    char status[20];        // "Scheduled", "Completed", "Cancelled"
} Appointment;

// Structure to hold medicine information
typedef struct {
    int id;
    char name[50];
    char manufacturer[50];
    float price;
    int quantity;
    char expiryDate[20]; // DD/MM/YYYY format
} Medicine;

// Global arrays to store data in memory
Patient patients[MAX_PATIENTS];
Doctor doctors[MAX_DOCTORS];
Appointment appointments[MAX_APPOINTMENTS];
Medicine medicines[MAX_MEDICINES]; // New global array

// Global counters for the number of records
int patientCount = 0;
int doctorCount = 0;
int appointmentCount = 0;
int medicineCount = 0; // New counter

// --- Function Prototypes ---

// Data management functions
void loadData(); // Loads data from binary files into memory
void saveData(); // Saves data from memory to binary files

// Authentication and menu functions
int authenticateAdmin(); // Authenticates the admin user
void mainMenu();         // Displays the main menu for different user roles
void adminMenu();        // Displays the admin functionalities menu
void doctorMenu(int doctorId); // Displays the doctor specific functionalities menu
void receptionistMenu(); // New menu for receptionist functionalities

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

// Medicine management functions (new)
void addMedicine();         // Adds a new medicine record
void displayAllMedicines(); // Displays all registered medicines
void searchMedicine();      // Searches for a medicine by name or ID
void updateMedicine();      // Updates an existing medicine record
void deleteMedicine();      // Deletes a medicine record

// Appointment management functions
void scheduleAppointment();      // Schedules a new appointment
void viewAppointments();         // Displays all appointments
void completeAppointment(int appointmentId); // New function to complete an appointment with diagnosis/prescription
void cancelAppointment();        // Cancels an existing appointment
void generateBill(int appointmentId); // New function to generate a bill for an appointment

// Specific view functions for doctor and patient roles
void viewPatientHistory(int patientId); // Displays a patient's medical history and past appointments
void viewDoctorSchedule(int doctorId);  // Displays a doctor's scheduled appointments

// Helper functions for finding records by ID
int findPatientById(int id);    // Finds a patient's index by their ID
int findDoctorById(int id);     // Finds a doctor's index by their ID
int findAppointmentById(int id); // Finds an appointment's index by its ID
int findMedicineById(int id);   // New: Finds a medicine's index by its ID

// Utility functions
void clearInputBuffer(); // Clears the standard input buffer
void printWelcomeArt();  // Prints ASCII art for welcome message

// --- Helper Menu Functions (for better menu navigation) ---
void patientManagementMenu();
void doctorManagementMenu();
void medicineManagementMenu(); // New menu for medicine management


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
    printf("   ____ _      _      _       _____           _      _\n");
    printf("  / ___| | ___ (_) ___| | __  / ____|         | |    | |\n");
    printf(" | |   | |/ _ \\| |/ __| |/ / | |     ___ _ __| |_ ___| |_\n");
    printf(" | |___| | (_) | | (__|  <  | |    / _ \\ '__| __/ __| __|\n");
    printf("  \\____|_|\\___/|_|\\___|_|\\_\\ | |___|  __/ |  | |_\\__ \\ |_\n");
    printf("                                      \\_____\\___|_|   \\__|___/\\__|\n");
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
    
    // Load medicines from file (new)
    file = fopen(FILENAME_MEDICINES, "rb");
    if (file != NULL) {
        medicineCount = fread(medicines, sizeof(Medicine), MAX_MEDICINES, file);
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
    
    // Save medicines to file (new)
    file = fopen(FILENAME_MEDICINES, "wb");
    if (file != NULL) {
        fwrite(medicines, sizeof(Medicine), medicineCount, file);
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
        printf("3. Receptionist Functions\n"); // New menu option
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
                
                if (findDoctorById(doctorId) != -1) {
                    doctorMenu(doctorId); // Go to doctor menu if ID found
                } else {
                    printf("Doctor not found!\n");
                }
                break;
            }
            case 3: receptionistMenu(); break; // Go to receptionist menu
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
        printf("1. Manage Patients\n");
        printf("2. Manage Doctors\n");
        printf("3. Manage Medicines\n");     // New menu option
        printf("4. View All Appointments\n");
        printf("5. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: patientManagementMenu(); break;      // Go to patient management
            case 2: doctorManagementMenu(); break;       // Go to doctor management
            case 3: medicineManagementMenu(); break;     // Go to medicine management
            case 4: viewAppointments(); break;           // View all appointments (admin can see all)
            case 5: printf("Returning to main menu...\n"); break; // Return to main menu
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 5);
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
        printf("6. View Doctor Schedule\n"); // Moved from doctor menu to admin for oversight
        printf("7. Back to Admin Menu\n");
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
            case 6: {
                int doctorId;
                printf("Enter doctor ID to view schedule: ");
                scanf("%d", &doctorId);
                clearInputBuffer();
                viewDoctorSchedule(doctorId);
                break;
            }
            case 7: printf("Returning to admin menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 7);
}

// Medicine management menu (new)
void medicineManagementMenu() {
    int choice;
    do {
        printf("\n===== MEDICINE MANAGEMENT =====\n");
        printf("1. Add New Medicine\n");
        printf("2. View All Medicines\n");
        printf("3. Search Medicine\n");
        printf("4. Update Medicine Record\n");
        printf("5. Delete Medicine Record\n");
        printf("6. Back to Admin Menu\n");
        printf("==============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addMedicine(); break;
            case 2: displayAllMedicines(); break;
            case 3: searchMedicine(); break;
            case 4: updateMedicine(); break;
            case 5: deleteMedicine(); break;
            case 6: printf("Returning to admin menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
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
        printf("2. Complete Appointment\n"); // Doctor fills diagnosis and prescription
        printf("3. View Patient History\n");
        printf("4. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: viewDoctorSchedule(doctorId); break;
            case 2: {
                int appointmentId;
                printf("Enter appointment ID to complete: ");
                scanf("%d", &appointmentId);
                clearInputBuffer();
                completeAppointment(appointmentId); // Call new complete appointment function
                break;
            }
            case 3: {
                int patientId;
                printf("Enter patient ID to view history: ");
                scanf("%d", &patientId);
                clearInputBuffer();
                viewPatientHistory(patientId);
                break;
            }
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

// Receptionist specific menu (new role)
void receptionistMenu() {
    int choice;
    do {
        printf("\n===== RECEPTIONIST MENU =====\n");
        printf("1. Schedule Appointment\n");
        printf("2. View All Appointments\n");
        printf("3. Cancel Appointment\n");
        printf("4. Generate Bill\n");       // New billing function
        printf("5. Search Patient\n");      // Receptionist might need to search patients
        printf("6. Back to Main Menu\n");
        printf("============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: scheduleAppointment(); break;
            case 2: viewAppointments(); break;
            case 3: cancelAppointment(); break;
            case 4: {
                int appointmentId;
                printf("Enter appointment ID to generate bill: ");
                scanf("%d", &appointmentId);
                clearInputBuffer();
                generateBill(appointmentId);
                break;
            }
            case 5: searchPatient(); break;
            case 6: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
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
    
    printf("Allergies (comma-separated, or 'None'): "); // New field input
    fgets(newPatient.allergies, sizeof(newPatient.allergies), stdin);
    newPatient.allergies[strcspn(newPatient.allergies, "\n")] = '\0';
    
    printf("Medical History (brief notes): "); // Increased size
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
    // Updated header for Address (was Email)
    printf("%-6s %-20s %-5s %-5s %-15s %-30s\n", 
           "ID", "Name", "Age", "Gen", "Phone", "Address");
    printf("------------------------------------------------------------------------\n");
    
    for (int i = 0; i < patientCount; i++) {
        printf("%-6d %-20s %-5d %-5c %-15s %-30s\n", 
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].gender,
               patients[i].phone,
               patients[i].address); // Displaying address
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
           "ID", "Name", "Age", "Gen", "Phone", "Address"); // Updated header
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
                   patients[i].address); // Displaying address
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
    printf("Age: %d\n", patients[index].age);
    printf("Gender: %c\n", patients[index].gender);
    printf("Blood Group: %s\n", patients[index].bloodGroup);
    printf("Allergies: %s\n", patients[index].allergies);       // New field
    printf("Medical History: %s\n", patients[index].medicalHistory); // Updated size
    
    printf("\nEnter new details (leave blank to keep current):\n");
    
    char input[MAX_MEDICINES]; // Reusing a define for buffer size, MAX_MEDICINES is 50, which is enough
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
    
    printf("Allergies [%s]: ", patients[index].allergies); // Update allergies
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(patients[index].allergies, input);
    }

    printf("Medical History [%s]: ", patients[index].medicalHistory); // Update medical history
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
    
    printf("Schedule (e.g., Mon-Fri 9AM-5PM): "); // Updated field
    fgets(newDoctor.schedule, sizeof(newDoctor.schedule), stdin);
    newDoctor.schedule[strcspn(newDoctor.schedule, "\n")] = '\0';
    
    printf("Consultation Fee: "); // Updated field name
    scanf("%d", &newDoctor.consultationFee);
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
    printf("%-6s %-20s %-20s %-15s %-10s %s\n", 
           "ID", "Name", "Specialization", "Phone", "Fee", "Schedule"); // Updated header
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < doctorCount; i++) {
        printf("%-6d %-20s %-20s %-15s $%-9d %s\n", 
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].phone,
               doctors[i].consultationFee, // Updated field name
               doctors[i].schedule);      // Updated field name
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
    printf("%-6s %-20s %-20s %-15s %-10s %s\n", 
           "ID", "Name", "Specialization", "Phone", "Fee", "Schedule"); // Updated header
    printf("----------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < doctorCount; i++) {
        // Search by name, specialization (substrings), or by ID
        if (strstr(doctors[i].name, searchTerm) != NULL || 
            strstr(doctors[i].specialization, searchTerm) != NULL ||
            (isdigit(searchTerm[0]) && doctors[i].id == atoi(searchTerm))) {
            printf("%-6d %-20s %-20s %-15s $%-9d %s\n", 
                   doctors[i].id,
                   doctors[i].name,
                   doctors[i].specialization,
                   doctors[i].phone,
                   doctors[i].consultationFee, // Updated field name
                   doctors[i].schedule);      // Updated field name
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
    printf("Schedule: %s\n", doctors[index].schedule);           // Updated field
    printf("Consultation Fee: %d\n", doctors[index].consultationFee); // Updated field name
    
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
    
    printf("Schedule [%s]: ", doctors[index].schedule); // Update schedule
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(doctors[index].schedule, input);
    }
    
    printf("Consultation Fee [%d]: ", doctors[index].consultationFee); // Update fee
    // Check if scanf successfully read an integer
    if (scanf("%d", &intInput) == 1) {
        doctors[index].consultationFee = intInput;
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

// Function to add a new medicine (new)
void addMedicine() {
    if (medicineCount >= MAX_MEDICINES) {
        printf("Maximum number of medicines reached!\n");
        return;
    }
    
    Medicine newMedicine;
    
    printf("\nEnter medicine details:\n");
    
    newMedicine.id = 3000 + medicineCount; // Simple ID generation for medicines
    
    printf("Name: ");
    fgets(newMedicine.name, sizeof(newMedicine.name), stdin);
    newMedicine.name[strcspn(newMedicine.name, "\n")] = '\0';
    
    printf("Manufacturer: ");
    fgets(newMedicine.manufacturer, sizeof(newMedicine.manufacturer), stdin);
    newMedicine.manufacturer[strcspn(newMedicine.manufacturer, "\n")] = '\0';
    
    printf("Price: ");
    scanf("%f", &newMedicine.price);
    clearInputBuffer();
    
    printf("Quantity: ");
    scanf("%d", &newMedicine.quantity);
    clearInputBuffer();
    
    printf("Expiry Date (DD/MM/YYYY): ");
    fgets(newMedicine.expiryDate, sizeof(newMedicine.expiryDate), stdin);
    newMedicine.expiryDate[strcspn(newMedicine.expiryDate, "\n")] = '\0';
    
    medicines[medicineCount++] = newMedicine; // Add new medicine and increment count
    
    printf("\nMedicine added successfully!\n");
    printf("Medicine ID: %d\n", newMedicine.id);
}

// Function to display all medicines (new)
void displayAllMedicines() {
    if (medicineCount == 0) {
        printf("\nNo medicines found.\n");
        return;
    }
    
    printf("\n===== ALL MEDICINES =====\n");
    printf("%-6s %-20s %-20s %-10s %-8s %s\n", 
           "ID", "Name", "Manufacturer", "Price", "Qty", "Expiry Date");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < medicineCount; i++) {
        printf("%-6d %-20s %-20s $%-9.2f %-8d %s\n", 
               medicines[i].id,
               medicines[i].name,
               medicines[i].manufacturer,
               medicines[i].price,
               medicines[i].quantity,
               medicines[i].expiryDate);
    }
}

// Function to search for a medicine (new)
void searchMedicine() {
    if (medicineCount == 0) {
        printf("\nNo medicines to search.\n");
        return;
    }
    
    char searchTerm[50];
    printf("\nEnter medicine name or ID to search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\n===== SEARCH RESULTS =====\n");
    printf("%-6s %-20s %-20s %-10s %-8s %s\n", 
           "ID", "Name", "Manufacturer", "Price", "Qty", "Expiry Date");
    printf("------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < medicineCount; i++) {
        if (strstr(medicines[i].name, searchTerm) != NULL || 
            (isdigit(searchTerm[0]) && medicines[i].id == atoi(searchTerm))) {
            printf("%-6d %-20s %-20s $%-9.2f %-8d %s\n", 
                   medicines[i].id,
                   medicines[i].name,
                   medicines[i].manufacturer,
                   medicines[i].price,
                   medicines[i].quantity,
                   medicines[i].expiryDate);
            found = true;
        }
    }
    
    if (!found) {
        printf("No medicines found matching your search.\n");
    }
}

// Function to update medicine details (new)
void updateMedicine() {
    if (medicineCount == 0) {
        printf("\nNo medicines to update.\n");
        return;
    }
    
    int medicineId;
    printf("\nEnter medicine ID to update: ");
    scanf("%d", &medicineId);
    clearInputBuffer();
    
    int index = findMedicineById(medicineId);
    if (index == -1) {
        printf("Medicine not found!\n");
        return;
    }
    
    printf("\nCurrent medicine details:\n");
    printf("Name: %s\n", medicines[index].name);
    printf("Manufacturer: %s\n", medicines[index].manufacturer);
    printf("Price: %.2f\n", medicines[index].price);
    printf("Quantity: %d\n", medicines[index].quantity);
    printf("Expiry Date: %s\n", medicines[index].expiryDate);
    
    printf("\nEnter new details (leave blank to keep current):\n");
    
    char input[100];
    float floatInput;
    int intInput;
    
    printf("Name [%s]: ", medicines[index].name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(medicines[index].name, input);
    }
    
    printf("Manufacturer [%s]: ", medicines[index].manufacturer);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(medicines[index].manufacturer, input);
    }
    
    printf("Price [%.2f]: ", medicines[index].price);
    if (scanf("%f", &floatInput) == 1) {
        medicines[index].price = floatInput;
    }
    clearInputBuffer();
    
    printf("Quantity [%d]: ", medicines[index].quantity);
    if (scanf("%d", &intInput) == 1) {
        medicines[index].quantity = intInput;
    }
    clearInputBuffer();
    
    printf("Expiry Date [%s]: ", medicines[index].expiryDate);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(medicines[index].expiryDate, input);
    }
    
    printf("\nMedicine record updated successfully!\n");
}

// Function to delete a medicine record (new)
void deleteMedicine() {
    if (medicineCount == 0) {
        printf("\nNo medicines to delete.\n");
        return;
    }
    
    int medicineId;
    printf("\nEnter medicine ID to delete: ");
    scanf("%d", &medicineId);
    clearInputBuffer();
    
    int index = findMedicineById(medicineId);
    if (index == -1) {
        printf("Medicine not found!\n");
        return;
    }
    
    printf("\nDeleting medicine:\n");
    printf("Name: %s\n", medicines[index].name);
    printf("ID: %d\n", medicines[index].id);
    printf("Manufacturer: %s\n", medicines[index].manufacturer);
    printf("Quantity: %d\n", medicines[index].quantity);
    
    char confirm;
    printf("\nAre you sure you want to delete this medicine? (y/n): ");
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (tolower(confirm) == 'y') {
        // Shift all medicines after this one forward
        for (int i = index; i < medicineCount - 1; i++) {
            medicines[i] = medicines[i + 1];
        }
        medicineCount--; // Decrement medicine count
        printf("\nMedicine deleted successfully!\n");
    } else {
        printf("\nMedicine deletion cancelled.\n");
    }
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
    
    // Generate a simple ID (starts from 4000 for appointments now, to avoid conflict with medicines)
    newAppointment.id = 4000 + appointmentCount; 
    
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
    int doctorIndex = findDoctorById(newAppointment.doctorId);
    if (doctorIndex == -1) {
        printf("Doctor not found! Please enter a valid doctor ID.\n");
        return;
    }
    
    printf("Date (DD/MM/YYYY): ");
    fgets(newAppointment.date, sizeof(newAppointment.date), stdin);
    newAppointment.date[strcspn(newAppointment.date, "\n")] = '\0';
    
    printf("Time (HH:MM): ");
    fgets(newAppointment.time, sizeof(newAppointment.time), stdin);
    newAppointment.time[strcspn(newAppointment.time, "\n")] = '\0';
    
    // Initialize diagnosis and prescription as empty for scheduled appointments
    strcpy(newAppointment.diagnosis, "N/A");
    strcpy(newAppointment.prescription, "N/A");
    
    newAppointment.fee = doctors[doctorIndex].consultationFee; // Set initial fee from doctor's consultation fee
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
    printf("%-6s %-8s %-20s %-20s %-12s %-8s %-10s %s\n", 
           "ID", "Pat.ID", "Patient Name", "Doctor Name", "Date", "Time", "Fee", "Status"); // Updated header
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

        printf("%-6d %-8d %-20s %-20s %-12s %-8s $%-9.2f %s\n", // Format fee as float
               appointments[i].id,
               appointments[i].patientId,
               patientName,
               doctorName,
               appointments[i].date,
               appointments[i].time,
               appointments[i].fee, // Display appointment fee
               appointments[i].status);
    }
}

// Function for doctor to complete an appointment (new)
void completeAppointment(int appointmentId) {
    int index = findAppointmentById(appointmentId);
    if (index == -1) {
        printf("Appointment not found!\n");
        return;
    }

    if (strcmp(appointments[index].status, "Scheduled") != 0) {
        printf("Appointment is not scheduled. Cannot complete.\n");
        return;
    }

    printf("\nCompleting Appointment ID: %d\n", appointmentId);
    printf("Current status: %s\n", appointments[index].status);
    
    // Get diagnosis input
    printf("Enter Diagnosis (brief notes): ");
    fgets(appointments[index].diagnosis, sizeof(appointments[index].diagnosis), stdin);
    appointments[index].diagnosis[strcspn(appointments[index].diagnosis, "\n")] = '\0';

    // Get prescription input
    printf("Enter Prescription (details, comma-separated medicines/instructions): ");
    fgets(appointments[index].prescription, sizeof(appointments[index].prescription), stdin);
    appointments[index].prescription[strcspn(appointments[index].prescription, "\n")] = '\0';

    // Update status to Completed
    strcpy(appointments[index].status, "Completed");
    printf("\nAppointment ID %d completed successfully!\n", appointmentId);
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
    
    char confirm;
    printf("\nAre you sure you want to cancel this appointment? (y/n): ");
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (tolower(confirm) == 'y') {
        // Change status to cancelled instead of deleting the record entirely
        strcpy(appointments[index].status, "Cancelled"); 
        printf("\nAppointment cancelled successfully!\n");
    } else {
        printf("\nAppointment cancellation aborted.\n");
    }
}

// Function to generate a bill for a completed appointment (new)
void generateBill(int appointmentId) {
    int index = findAppointmentById(appointmentId);
    if (index == -1) {
        printf("Appointment not found!\n");
        return;
    }

    if (strcmp(appointments[index].status, "Completed") != 0) {
        printf("Bill can only be generated for completed appointments. Current status: %s\n", appointments[index].status);
        return;
    }

    int patientIndex = findPatientById(appointments[index].patientId);
    int doctorIndex = findDoctorById(appointments[index].doctorId);

    char patientName[50] = "Unknown";
    char doctorName[50] = "Unknown";

    if (patientIndex != -1) {
        strcpy(patientName, patients[patientIndex].name);
    }
    if (doctorIndex != -1) {
        strcpy(doctorName, doctors[doctorIndex].name);
    }

    printf("\n===== BILL FOR APPOINTMENT ID: %d =====\n", appointmentId);
    printf("Date: %s, Time: %s\n", appointments[index].date, appointments[index].time);
    printf("Patient Name: %s (ID: %d)\n", patientName, appointments[index].patientId);
    printf("Doctor Name: %s (ID: %d)\n", doctorName, appointments[index].doctorId);
    printf("----------------------------------------\n");
    printf("Consultation Fee: $%.2f\n", doctors[doctorIndex].consultationFee); // Get fee from doctor record

    // Here you could add logic to parse the prescription and add medicine costs
    // For simplicity, we'll just display a placeholder total for now.
    float totalBill = doctors[doctorIndex].consultationFee;
    
    // Basic medicine cost estimation (you would typically parse the prescription for actual medicines)
    // For now, let's assume a fixed "medicine charge" if prescription is not "N/A"
    if (strcmp(appointments[index].prescription, "N/A") != 0 && strlen(appointments[index].prescription) > 0) {
        float medicineCharge = 0.0;
        printf("Prescription: %s\n", appointments[index].prescription);
        printf("Do you want to add medicine charges to the bill? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        clearInputBuffer();
        if (tolower(choice) == 'y') {
            printf("Enter estimated medicine cost: $");
            scanf("%f", &medicineCharge);
            clearInputBuffer();
            totalBill += medicineCharge;
            printf("Medicine Charge: $%.2f\n", medicineCharge);
        }
    }

    printf("----------------------------------------\n");
    printf("Total Amount Due: $%.2f\n", totalBill);
    printf("========================================\n");
    // Update the appointment's stored fee with the final calculated bill amount
    appointments[index].fee = totalBill; 
    printf("Bill generated. Total fee updated in appointment record.\n");
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
           "ID", "Patient Name", "Date", "Time", "Purpose/Status", "Diagnosis/Prescription"); // Updated header
    printf("------------------------------------------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].doctorId == doctorId) {
            int patientIndex = findPatientById(appointments[i].patientId);
            char patientName[50] = "Unknown";
            if (patientIndex != -1) {
                strcpy(patientName, patients[patientIndex].name);
            }
            printf("%-6d %-20s %-12s %-8s %-20s %s / %s\n", 
                   appointments[i].id,
                   patientName,
                   appointments[i].date,
                   appointments[i].time,
                   appointments[i].status, // Now showing status here
                   appointments[i].diagnosis, // Display diagnosis
                   appointments[i].prescription); // Display prescription
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
    printf("Allergies: %s\n", patients[patientIndex].allergies);       // New field
    printf("Known Medical History: %s\n", patients[patientIndex].medicalHistory);

    printf("\n--- Past Appointments ---\n");
    printf("%-6s %-20s %-12s %-8s %-20s %s\n", 
           "ID", "Doctor Name", "Date", "Time", "Diagnosis", "Prescription"); // Updated header
    printf("--------------------------------------------------------------------------------\n");

    bool foundAppointments = false;
    for (int i = 0; i < appointmentCount; i++) {
        // Display only completed appointments for medical history
        if (appointments[i].patientId == patientId && strcmp(appointments[i].status, "Completed") == 0) {
            int doctorIndex = findDoctorById(appointments[i].doctorId);
            char doctorName[50] = "Unknown";
            if (doctorIndex != -1) {
                strcpy(doctorName, doctors[doctorIndex].name);
            }
            printf("%-6d %-20s %-12s %-8s %-20s %s\n", 
                   appointments[i].id,
                   doctorName,
                   appointments[i].date,
                   appointments[i].time,
                   appointments[i].diagnosis,    // Display diagnosis
                   appointments[i].prescription); // Display prescription
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

// Helper function to find a medicine by ID and return its index (new)
int findMedicineById(int id) {
    for (int i = 0; i < medicineCount; i++) {
        if (medicines[i].id == id) {
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
