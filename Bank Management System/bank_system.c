#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define FILENAME "bank_data.dat"
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

typedef struct {
    int accountNumber;
    char name[100];
    char address[100];
    char phone[15];
    double balance;
    char accountType[20]; // "savings" or "current"
    time_t lastTransaction;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function prototypes
void loadData();
void saveData();
int authenticateAdmin();
void mainMenu();
void adminMenu();
void customerMenu(int accountIndex);
void createAccount();
void displayAllAccounts();
void searchAccount();
void deposit(int accountIndex);
void withdraw(int accountIndex);
void transfer(int accountIndex);
void viewBalance(int accountIndex);
void viewTransactionHistory(int accountIndex);
void deleteAccount();
void modifyAccount();
int findAccountByNumber(int accountNumber);
void clearInputBuffer();
void printAccountDetails(int index);
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
    printf("  ____              _        _   _           _\n");
    printf(" |  _ \\            | |      | | | |         | |\n");
    printf(" | |_) | __ _ _ __ | | __   | |_| | __ _ ___| |__\n");
    printf(" |  _ < / _` | '_ \\| |/ /   |  _  |/ _` / __| '_ \\\n");
    printf(" | |_) | (_| | | | |   <    | | | | (_| \\__ \\ | | |\n");
    printf(" |____/ \\__,_|_| |_|_|\\_\\   \\_| |_/\\__,_|___/_| |_|\n");
    printf("\n");
}

void loadData() {
    FILE *file = fopen(FILENAME, "rb");
    if (file != NULL) {
        accountCount = fread(accounts, sizeof(Account), MAX_ACCOUNTS, file);
        fclose(file);
    }
}

void saveData() {
    FILE *file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(accounts, sizeof(Account), accountCount, file);
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
        printf("2. Customer Login\n");
        printf("3. Exit\n");
        printf("====================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: adminMenu(); break;
            case 2: {
                int accNumber;
                printf("Enter your account number: ");
                scanf("%d", &accNumber);
                clearInputBuffer();
                
                int accountIndex = findAccountByNumber(accNumber);
                if (accountIndex != -1) {
                    customerMenu(accountIndex);
                } else {
                    printf("Account not found!\n");
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
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Modify Account\n");
        printf("5. Delete Account\n");
        printf("6. Back to Main Menu\n");
        printf("======================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: createAccount(); break;
            case 2: displayAllAccounts(); break;
            case 3: searchAccount(); break;
            case 4: modifyAccount(); break;
            case 5: deleteAccount(); break;
            case 6: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
}

void customerMenu(int accountIndex) {
    printf("\nWelcome, %s!\n", accounts[accountIndex].name);
    
    int choice;
    do {
        printf("\n===== CUSTOMER MENU =====\n");
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Transfer Money\n");
        printf("4. View Balance\n");
        printf("5. View Transaction History\n");
        printf("6. Back to Main Menu\n");
        printf("=========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: deposit(accountIndex); break;
            case 2: withdraw(accountIndex); break;
            case 3: transfer(accountIndex); break;
            case 4: viewBalance(accountIndex); break;
            case 5: viewTransactionHistory(accountIndex); break;
            case 6: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6);
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached!\n");
        return;
    }
    
    Account newAccount;
    
    // Generate account number (1000 + current count)
    newAccount.accountNumber = 1000 + accountCount;
    
    printf("\nEnter account details:\n");
    
    printf("Customer Name: ");
    fgets(newAccount.name, sizeof(newAccount.name), stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = '\0';
    
    printf("Address: ");
    fgets(newAccount.address, sizeof(newAccount.address), stdin);
    newAccount.address[strcspn(newAccount.address, "\n")] = '\0';
    
    printf("Phone Number: ");
    fgets(newAccount.phone, sizeof(newAccount.phone), stdin);
    newAccount.phone[strcspn(newAccount.phone, "\n")] = '\0';
    
    printf("Initial Deposit: ");
    scanf("%lf", &newAccount.balance);
    clearInputBuffer();
    
    printf("Account Type (savings/current): ");
    fgets(newAccount.accountType, sizeof(newAccount.accountType), stdin);
    newAccount.accountType[strcspn(newAccount.accountType, "\n")] = '\0';
    
    // Set last transaction time to now
    newAccount.lastTransaction = time(NULL);
    
    accounts[accountCount++] = newAccount;
    
    printf("\nAccount created successfully!\n");
    printf("Account Number: %d\n", newAccount.accountNumber);
}

void displayAllAccounts() {
    printf("\n===== ALL ACCOUNTS =====\n");
    printf("%-15s %-20s %-15s %-10s %-15s %s\n", 
           "Account No.", "Name", "Phone", "Type", "Balance", "Last Transaction");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < accountCount; i++) {
        printf("%-15d %-20s %-15s %-10s $%-14.2f %s", 
               accounts[i].accountNumber,
               accounts[i].name,
               accounts[i].phone,
               accounts[i].accountType,
               accounts[i].balance,
               ctime(&accounts[i].lastTransaction));
    }
}

void searchAccount() {
    int accNumber;
    printf("\nEnter account number to search: ");
    scanf("%d", &accNumber);
    clearInputBuffer();
    
    int index = findAccountByNumber(accNumber);
    if (index != -1) {
        printAccountDetails(index);
    } else {
        printf("Account not found!\n");
    }
}

void deposit(int accountIndex) {
    double amount;
    printf("\nEnter amount to deposit: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    
    accounts[accountIndex].balance += amount;
    accounts[accountIndex].lastTransaction = time(NULL);
    
    printf("Deposit successful. New balance: $%.2f\n", accounts[accountIndex].balance);
}

void withdraw(int accountIndex) {
    double amount;
    printf("\nEnter amount to withdraw: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    
    if (amount > accounts[accountIndex].balance) {
        printf("Insufficient balance!\n");
        return;
    }
    
    accounts[accountIndex].balance -= amount;
    accounts[accountIndex].lastTransaction = time(NULL);
    
    printf("Withdrawal successful. New balance: $%.2f\n", accounts[accountIndex].balance);
}

void transfer(int accountIndex) {
    int targetAccNumber;
    double amount;
    
    printf("\nEnter recipient account number: ");
    scanf("%d", &targetAccNumber);
    clearInputBuffer();
    
    int targetIndex = findAccountByNumber(targetAccNumber);
    if (targetIndex == -1) {
        printf("Recipient account not found!\n");
        return;
    }
    
    if (targetIndex == accountIndex) {
        printf("Cannot transfer to the same account!\n");
        return;
    }
    
    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    
    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    
    if (amount > accounts[accountIndex].balance) {
        printf("Insufficient balance!\n");
        return;
    }
    
    accounts[accountIndex].balance -= amount;
    accounts[targetIndex].balance += amount;
    
    time_t now = time(NULL);
    accounts[accountIndex].lastTransaction = now;
    accounts[targetIndex].lastTransaction = now;
    
    printf("Transfer successful!\n");
    printf("Your new balance: $%.2f\n", accounts[accountIndex].balance);
}

void viewBalance(int accountIndex) {
    printf("\nAccount Balance: $%.2f\n", accounts[accountIndex].balance);
}

void viewTransactionHistory(int accountIndex) {
    printf("\n===== TRANSACTION HISTORY =====\n");
    printAccountDetails(accountIndex);
    printf("Last Transaction: %s", ctime(&accounts[accountIndex].lastTransaction));
}

void deleteAccount() {
    int accNumber;
    printf("\nEnter account number to delete: ");
    scanf("%d", &accNumber);
    clearInputBuffer();
    
    int index = findAccountByNumber(accNumber);
    if (index != -1) {
        // Shift all accounts after this one forward
        for (int i = index; i < accountCount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        accountCount--;
        printf("Account deleted successfully!\n");
    } else {
        printf("Account not found!\n");
    }
}

void modifyAccount() {
    int accNumber;
    printf("\nEnter account number to modify: ");
    scanf("%d", &accNumber);
    clearInputBuffer();
    
    int index = findAccountByNumber(accNumber);
    if (index != -1) {
        printf("\nCurrent account details:\n");
        printAccountDetails(index);
        
        printf("\nEnter new details (leave blank to keep current):\n");
        
        char input[100];
        
        printf("Name [%s]: ", accounts[index].name);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strcpy(accounts[index].name, input);
        }
        
        printf("Address [%s]: ", accounts[index].address);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strcpy(accounts[index].address, input);
        }
        
        printf("Phone [%s]: ", accounts[index].phone);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strcpy(accounts[index].phone, input);
        }
        
        printf("Account Type [%s]: ", accounts[index].accountType);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            strcpy(accounts[index].accountType, input);
        }
        
        accounts[index].lastTransaction = time(NULL);
        printf("\nAccount updated successfully!\n");
    } else {
        printf("Account not found!\n");
    }
}

int findAccountByNumber(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

void printAccountDetails(int index) {
    printf("\n===== ACCOUNT DETAILS =====\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Customer Name: %s\n", accounts[index].name);
    printf("Address: %s\n", accounts[index].address);
    printf("Phone Number: %s\n", accounts[index].phone);
    printf("Account Type: %s\n", accounts[index].accountType);
    printf("Current Balance: $%.2f\n", accounts[index].balance);
    printf("Last Transaction: %s", ctime(&accounts[index].lastTransaction));
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
