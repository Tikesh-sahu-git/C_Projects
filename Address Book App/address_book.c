#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CONTACTS 100
#define FILENAME "address_book.dat"

typedef struct {
    char name[50];
    char phone[15];
    char email[50];
    char address[100];
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

// Function prototypes
void loadContacts();
void saveContacts();
void displayMenu();
void addContact();
void viewAllContacts();
void searchContact();
void deleteContact();
void editContact();
void clearInputBuffer();

int main() {
    loadContacts();
    
    int choice;
    do {
        displayMenu();
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addContact(); break;
            case 2: viewAllContacts(); break;
            case 3: searchContact(); break;
            case 4: editContact(); break;
            case 5: deleteContact(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
    
    saveContacts();
    return 0;
}

void loadContacts() {
    FILE *file = fopen(FILENAME, "rb");
    if (file != NULL) {
        contactCount = fread(contacts, sizeof(Contact), MAX_CONTACTS, file);
        fclose(file);
    }
}

void saveContacts() {
    FILE *file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(contacts, sizeof(Contact), contactCount, file);
        fclose(file);
    }
}

void displayMenu() {
    printf("\n===== ADDRESS BOOK MENU =====\n");
    printf("1. Add New Contact\n");
    printf("2. View All Contacts\n");
    printf("3. Search Contact\n");
    printf("4. Edit Contact\n");
    printf("5. Delete Contact\n");
    printf("6. Exit\n");
    printf("=============================\n");
}

void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("Address book is full!\n");
        return;
    }
    
    Contact newContact;
    
    printf("\nEnter contact details:\n");
    
    printf("Name: ");
    fgets(newContact.name, sizeof(newContact.name), stdin);
    newContact.name[strcspn(newContact.name, "\n")] = '\0';
    
    printf("Phone: ");
    fgets(newContact.phone, sizeof(newContact.phone), stdin);
    newContact.phone[strcspn(newContact.phone, "\n")] = '\0';
    
    printf("Email: ");
    fgets(newContact.email, sizeof(newContact.email), stdin);
    newContact.email[strcspn(newContact.email, "\n")] = '\0';
    
    printf("Address: ");
    fgets(newContact.address, sizeof(newContact.address), stdin);
    newContact.address[strcspn(newContact.address, "\n")] = '\0';
    
    contacts[contactCount++] = newContact;
    printf("\nContact added successfully!\n");
}

void viewAllContacts() {
    if (contactCount == 0) {
        printf("\nNo contacts found in the address book.\n");
        return;
    }
    
    printf("\n===== ALL CONTACTS =====\n");
    printf("%-20s %-15s %-30s %s\n", "Name", "Phone", "Email", "Address");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < contactCount; i++) {
        printf("%-20s %-15s %-30s %s\n", 
               contacts[i].name,
               contacts[i].phone,
               contacts[i].email,
               contacts[i].address);
    }
}

void searchContact() {
    if (contactCount == 0) {
        printf("\nNo contacts to search.\n");
        return;
    }
    
    char searchTerm[50];
    printf("\nEnter name or phone number to search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\n===== SEARCH RESULTS =====\n");
    printf("%-20s %-15s %-30s %s\n", "Name", "Phone", "Email", "Address");
    printf("--------------------------------------------------------------------------------\n");
    
    bool found = false;
    for (int i = 0; i < contactCount; i++) {
        if (strstr(contacts[i].name, searchTerm) || 
            strstr(contacts[i].phone, searchTerm)) {
            printf("%-20s %-15s %-30s %s\n", 
                   contacts[i].name,
                   contacts[i].phone,
                   contacts[i].email,
                   contacts[i].address);
            found = true;
        }
    }
    
    if (!found) {
        printf("No contacts found matching your search.\n");
    }
}

void editContact() {
    if (contactCount == 0) {
        printf("\nNo contacts to edit.\n");
        return;
    }
    
    char searchTerm[50];
    printf("\nEnter name or phone number of contact to edit: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    int foundIndex = -1;
    for (int i = 0; i < contactCount; i++) {
        if (strstr(contacts[i].name, searchTerm) || 
            strstr(contacts[i].phone, searchTerm)) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        printf("Contact not found.\n");
        return;
    }
    
    printf("\nCurrent contact details:\n");
    printf("Name: %s\n", contacts[foundIndex].name);
    printf("Phone: %s\n", contacts[foundIndex].phone);
    printf("Email: %s\n", contacts[foundIndex].email);
    printf("Address: %s\n", contacts[foundIndex].address);
    
    printf("\nEnter new details (leave blank to keep current):\n");
    
    char input[100];
    
    printf("Name [%s]: ", contacts[foundIndex].name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(contacts[foundIndex].name, input);
    }
    
    printf("Phone [%s]: ", contacts[foundIndex].phone);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(contacts[foundIndex].phone, input);
    }
    
    printf("Email [%s]: ", contacts[foundIndex].email);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(contacts[foundIndex].email, input);
    }
    
    printf("Address [%s]: ", contacts[foundIndex].address);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0) {
        strcpy(contacts[foundIndex].address, input);
    }
    
    printf("\nContact updated successfully!\n");
}

void deleteContact() {
    if (contactCount == 0) {
        printf("\nNo contacts to delete.\n");
        return;
    }
    
    char searchTerm[50];
    printf("\nEnter name or phone number of contact to delete: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    int foundIndex = -1;
    for (int i = 0; i < contactCount; i++) {
        if (strstr(contacts[i].name, searchTerm) || 
            strstr(contacts[i].phone, searchTerm)) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        printf("Contact not found.\n");
        return;
    }
    
    printf("\nDeleting contact:\n");
    printf("Name: %s\n", contacts[foundIndex].name);
    printf("Phone: %s\n", contacts[foundIndex].phone);
    
    // Shift all contacts after this one forward
    for (int i = foundIndex; i < contactCount - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contactCount--;
    
    printf("\nContact deleted successfully!\n");
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
