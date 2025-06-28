#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_BORROWERS 50
#define FILENAME_BOOKS "books.dat"
#define FILENAME_BORROWERS "borrowers.dat"

typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
    int is_available; // 1 for available, 0 for borrowed
} Book;

typedef struct {
    int book_id;
    int borrower_id;
    char borrower_name[100];
    time_t due_date;
} Borrower;

Book books[MAX_BOOKS];
Borrower borrowers[MAX_BORROWERS];
int book_count = 0;
int borrower_count = 0;

// Function prototypes
void loadData();
void saveData();
void displayMenu();
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

int main() {
    loadData();
    
    int choice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3: searchBook(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: displayBorrowedBooks(); break;
            case 7: displayOverdueBooks(); break;
            case 8: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 8);
    
    saveData();
    return 0;
}

void loadData() {
    FILE *file;
    
    // Load books
    file = fopen(FILENAME_BOOKS, "rb");
    if (file != NULL) {
        book_count = fread(books, sizeof(Book), MAX_BOOKS, file);
        fclose(file);
    }
    
    // Load borrowers
    file = fopen(FILENAME_BORROWERS, "rb");
    if (file != NULL) {
        borrower_count = fread(borrowers, sizeof(Borrower), MAX_BORROWERS, file);
        fclose(file);
    }
}

void saveData() {
    FILE *file;
    
    // Save books
    file = fopen(FILENAME_BOOKS, "wb");
    if (file != NULL) {
        fwrite(books, sizeof(Book), book_count, file);
        fclose(file);
    }
    
    // Save borrowers
    file = fopen(FILENAME_BORROWERS, "wb");
    if (file != NULL) {
        fwrite(borrowers, sizeof(Borrower), borrower_count, file);
        fclose(file);
    }
}

void displayMenu() {
    printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
    printf("1. Add a new book\n");
    printf("2. Display all books\n");
    printf("3. Search for a book\n");
    printf("4. Borrow a book\n");
    printf("5. Return a book\n");
    printf("6. Display borrowed books\n");
    printf("7. Display overdue books\n");
    printf("8. Exit\n");
    printf("====================================\n");
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
    
    printf("Enter your borrower ID: ");
    scanf("%d", &newBorrower.borrower_id);
    clearInputBuffer();
    
    printf("Enter your name: ");
    fgets(newBorrower.borrower_name, sizeof(newBorrower.borrower_name), stdin);
    newBorrower.borrower_name[strcspn(newBorrower.borrower_name, "\n")] = '\0';
    
    // Set due date to 14 days from now
    time_t now = time(NULL);
    newBorrower.due_date = now + (14 * 24 * 60 * 60); // 14 days in seconds
    
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
    
    // Remove borrower record by shifting array
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
