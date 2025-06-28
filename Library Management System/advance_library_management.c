#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_BOOKS 100
#define MAX_BORROWERS 50
#define MAX_USERS 20
#define FILENAME_BOOKS "books.dat"
#define FILENAME_BORROWERS "borrowers.dat"
#define FILENAME_USERS "users.dat"

typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
    int is_available;
} Book;

typedef struct {
    int book_id;
    int borrower_id;
    char borrower_name[100];
    time_t due_date;
} Borrower;

typedef struct {
    char username[50];
    char password[50];
    int is_librarian;
} User;

Book books[MAX_BOOKS];
Borrower borrowers[MAX_BORROWERS];
User users[MAX_USERS];
int book_count = 0;
int borrower_count = 0;
int user_count = 0;
User current_user;

void loadData();
void saveData();
int authenticateUser();
void registerUser();
void librarianMenu();
void userMenu();
void addBook();
void displayAllBooks();
void searchBook();
void borrowBook();
void returnBook();
void displayBorrowedBooks();
void displayOverdueBooks();
int findBookById(int id);
int findBookByTitle(const char *title);
int findBorrowerByBookId(int book_id);
void clearInputBuffer();
void changePassword();
void viewUserDetails();

int main() {
    loadData();
    
    int choice;
    do {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: 
                if (authenticateUser()) {
                    if (current_user.is_librarian) {
                        librarianMenu();
                    } else {
                        userMenu();
                    }
                }
                break;
            case 2: 
                registerUser();
                break;
            case 3: 
                printf("Exiting...\n");
                break;
            default: 
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 3);
    
    saveData();
    return 0;
}

void loadData() {
    FILE *file;
    
    file = fopen(FILENAME_BOOKS, "rb");
    if (file != NULL) {
        book_count = fread(books, sizeof(Book), MAX_BOOKS, file);
        fclose(file);
    }
    
    file = fopen(FILENAME_BORROWERS, "rb");
    if (file != NULL) {
        borrower_count = fread(borrowers, sizeof(Borrower), MAX_BORROWERS, file);
        fclose(file);
    }
    
    file = fopen(FILENAME_USERS, "rb");
    if (file != NULL) {
        user_count = fread(users, sizeof(User), MAX_USERS, file);
        fclose(file);
        
        if (user_count == 0) {
            strcpy(users[0].username, "admin");
            strcpy(users[0].password, "admin123");
            users[0].is_librarian = 1;
            user_count = 1;
        }
    } else {
        strcpy(users[0].username, "admin");
        strcpy(users[0].password, "admin123");
        users[0].is_librarian = 1;
        user_count = 1;
    }
}

void saveData() {
    FILE *file;
    
    file = fopen(FILENAME_BOOKS, "wb");
    if (file != NULL) {
        fwrite(books, sizeof(Book), book_count, file);
        fclose(file);
    }
    
    file = fopen(FILENAME_BORROWERS, "wb");
    if (file != NULL) {
        fwrite(borrowers, sizeof(Borrower), borrower_count, file);
        fclose(file);
    }
    
    file = fopen(FILENAME_USERS, "wb");
    if (file != NULL) {
        fwrite(users, sizeof(User), user_count, file);
        fclose(file);
    }
}

int authenticateUser() {
    char username[50];
    char password[50];
    
    printf("\nEnter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            current_user = users[i];
            printf("\nLogin successful! Welcome, %s.\n", username);
            return 1;
        }
    }
    
    printf("\nInvalid username or password.\n");
    return 0;
}

void registerUser() {
    if (user_count >= MAX_USERS) {
        printf("Maximum number of users reached!\n");
        return;
    }
    
    User newUser;
    printf("\nEnter registration details:\n");
    
    printf("Username: ");
    fgets(newUser.username, sizeof(newUser.username), stdin);
    newUser.username[strcspn(newUser.username, "\n")] = '\0';
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("Username already exists. Please choose another.\n");
            return;
        }
    }
    
    printf("Password: ");
    fgets(newUser.password, sizeof(newUser.password), stdin);
    newUser.password[strcspn(newUser.password, "\n")] = '\0';
    
    newUser.is_librarian = 0;
    
    users[user_count++] = newUser;
    printf("Registration successful! You can now login.\n");
}

void librarianMenu() {
    int choice;
    do {
        printf("\n===== LIBRARIAN MENU =====\n");
        printf("1. Add a new book\n");
        printf("2. Display all books\n");
        printf("3. Search for a book\n");
        printf("4. View borrowed books\n");
        printf("5. View overdue books\n");
        printf("6. View user details\n");
        printf("7. Change password\n");
        printf("8. Logout\n");
        printf("==========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3: searchBook(); break;
            case 4: displayBorrowedBooks(); break;
            case 5: displayOverdueBooks(); break;
            case 6: viewUserDetails(); break;
            case 7: changePassword(); break;
            case 8: printf("Logging out...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 8);
}

void userMenu() {
    int choice;
    do {
        printf("\n===== USER MENU =====\n");
        printf("1. Display all books\n");
        printf("2. Search for a book\n");
        printf("3. Borrow a book\n");
        printf("4. Return a book\n");
        printf("5. View my details\n");
        printf("6. Change password\n");
        printf("7. Logout\n");
        printf("=====================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: displayAllBooks(); break;
            case 2: searchBook(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: viewUserDetails(); break;
            case 6: changePassword(); break;
            case 7: printf("Logging out...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 7);
}

void addBook() {
    if (book_count >= MAX_BOOKS) {
        printf("Maximum number of books reached!\n");
        return;
    }
    
    Book newBook;
    printf("\nEnter book details:\n");
    
    newBook.id = book_count + 1;
    
    printf("Title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';
    
    printf("Author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';
    
    printf("Year: ");
    scanf("%d", &newBook.year);
    clearInputBuffer();
    
    newBook.is_available = 1;
    
    books[book_count++] = newBook;
    printf("Book added successfully with ID: %d\n", newBook.id);
}

void displayAllBooks() {
    printf("\n===== ALL BOOKS =====\n");
    printf("%-5s %-30s %-20s %-6s %s\n", "ID", "Title", "Author", "Year", "Status");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < book_count; i++) {
        printf("%-5d %-30s %-20s %-6d %s\n", 
               books[i].id, 
               books[i].title, 
               books[i].author, 
               books[i].year,
               books[i].is_available ? "Available" : "Borrowed");
    }
}

void searchBook() {
    char searchTerm[100];
    printf("\nEnter title or author to search: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\n===== SEARCH RESULTS =====\n");
    printf("%-5s %-30s %-20s %-6s %s\n", "ID", "Title", "Author", "Year", "Status");
    printf("------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].title, searchTerm) || strstr(books[i].author, searchTerm)) {
            printf("%-5d %-30s %-20s %-6d %s\n", 
                   books[i].id, 
                   books[i].title, 
                   books[i].author, 
                   books[i].year,
                   books[i].is_available ? "Available" : "Borrowed");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No books found matching your search.\n");
    }
}

void borrowBook() {
    int book_id;
    printf("\nEnter book ID to borrow: ");
    scanf("%d", &book_id);
    clearInputBuffer();
    
    int book_index = findBookById(book_id);
    if (book_index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    if (!books[book_index].is_available) {
        printf("Book is already borrowed!\n");
        return;
    }
    
    if (borrower_count >= MAX_BORROWERS) {
        printf("Maximum number of borrowers reached!\n");
        return;
    }
    
    Borrower newBorrower;
    newBorrower.book_id = book_id;
    newBorrower.borrower_id = current_user.is_librarian ? 0 : 1; // Simple user type flag
    
    printf("Enter your name: ");
    fgets(newBorrower.borrower_name, sizeof(newBorrower.borrower_name), stdin);
    newBorrower.borrower_name[strcspn(newBorrower.borrower_name, "\n")] = '\0';
    
    time_t now = time(NULL);
    newBorrower.due_date = now + (14 * 24 * 60 * 60);
    
    borrowers[borrower_count++] = newBorrower;
    books[book_index].is_available = 0;
    
    printf("Book borrowed successfully!\n");
    printf("Due date: %s", ctime(&newBorrower.due_date));
}

void returnBook() {
    int book_id;
    printf("\nEnter book ID to return: ");
    scanf("%d", &book_id);
    clearInputBuffer();
    
    int book_index = findBookById(book_id);
    if (book_index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    if (books[book_index].is_available) {
        printf("Book is not currently borrowed!\n");
        return;
    }
    
    int borrower_index = findBorrowerByBookId(book_id);
    if (borrower_index == -1) {
        printf("Borrower record not found!\n");
        return;
    }
    
    for (int i = borrower_index; i < borrower_count - 1; i++) {
        borrowers[i] = borrowers[i + 1];
    }
    borrower_count--;
    
    books[book_index].is_available = 1;
    printf("Book returned successfully!\n");
}

void displayBorrowedBooks() {
    printf("\n===== BORROWED BOOKS =====\n");
    printf("%-5s %-30s %-20s %-15s %s\n", "ID", "Title", "Borrower", "Borrower ID", "Due Date");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < borrower_count; i++) {
        int book_index = findBookById(borrowers[i].book_id);
        if (book_index != -1) {
            printf("%-5d %-30s %-20s %-15d %s", 
                   books[book_index].id,
                   books[book_index].title,
                   borrowers[i].borrower_name,
                   borrowers[i].borrower_id,
                   ctime(&borrowers[i].due_date));
        }
    }
    
    if (borrower_count == 0) {
        printf("No books are currently borrowed.\n");
    }
}

void displayOverdueBooks() {
    time_t now = time(NULL);
    int overdue_count = 0;
    
    printf("\n===== OVERDUE BOOKS =====\n");
    printf("%-5s %-30s %-20s %-15s %s\n", "ID", "Title", "Borrower", "Borrower ID", "Due Date");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < borrower_count; i++) {
        if (borrowers[i].due_date < now) {
            int book_index = findBookById(borrowers[i].book_id);
            if (book_index != -1) {
                printf("%-5d %-30s %-20s %-15d %s", 
                       books[book_index].id,
                       books[book_index].title,
                       borrowers[i].borrower_name,
                       borrowers[i].borrower_id,
                       ctime(&borrowers[i].due_date));
                overdue_count++;
            }
        }
    }
    
    if (overdue_count == 0) {
        printf("No books are currently overdue.\n");
    }
}

void changePassword() {
    char current_password[50];
    char new_password[50];
    
    printf("\nEnter current password: ");
    fgets(current_password, sizeof(current_password), stdin);
    current_password[strcspn(current_password, "\n")] = '\0';
    
    if (strcmp(current_user.password, current_password) != 0) {
        printf("Incorrect current password.\n");
        return;
    }
    
    printf("Enter new password: ");
    fgets(new_password, sizeof(new_password), stdin);
    new_password[strcspn(new_password, "\n")] = '\0';
    
    strcpy(current_user.password, new_password);
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, current_user.username) == 0) {
            strcpy(users[i].password, new_password);
            break;
        }
    }
    
    printf("Password changed successfully!\n");
}

void viewUserDetails() {
    printf("\n===== USER DETAILS =====\n");
    printf("Username: %s\n", current_user.username);
    printf("Account Type: %s\n", current_user.is_librarian ? "Librarian" : "Regular User");
    
    if (!current_user.is_librarian) {
        printf("\nYour Borrowed Books:\n");
        printf("--------------------\n");
        
        int count = 0;
        for (int i = 0; i < borrower_count; i++) {
            if (strcmp(borrowers[i].borrower_name, current_user.username) == 0) {
                int book_index = findBookById(borrowers[i].book_id);
                if (book_index != -1) {
                    printf("Book: %s (ID: %d)\n", books[book_index].title, books[book_index].id);
                    printf("Due Date: %s", ctime(&borrowers[i].due_date));
                    count++;
                }
            }
        }
        
        if (count == 0) {
            printf("You have no borrowed books.\n");
        }
    }
}

int findBookById(int id) {
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findBookByTitle(const char *title) {
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            return i;
        }
    }
    return -1;
}

int findBorrowerByBookId(int book_id) {
    for (int i = 0; i < borrower_count; i++) {
        if (borrowers[i].book_id == book_id) {
            return i;
        }
    }
    return -1;
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
