#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100

typedef struct {
    int id;
    char title[50];
    char author[50];
    float price;
    int quantity;
} Book;

// Function prototypes
void addBook();
void viewBooks();
void updateBook();
void deleteBook();
void adminMenu();

int main() {
    int choice;
    char role[10];

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Admin Login\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // For simplicity, we assume a single admin with username "admin" and password "admin"
                char username[50], password[50];
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);

                if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
                    printf("Login successful! Welcome, Admin.\n");
                    adminMenu();
                } else {
                    printf("Invalid credentials. Try again.\n");
                }
                break;
            }
            case 2:
                printf("Exiting the system...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

// Function for admin menu
void adminMenu() {
    int adminChoice;
    do {
        printf("\nAdmin Menu\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Update Book\n");
        printf("4. Delete Book\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &adminChoice);

        switch (adminChoice) {
            case 1:
                addBook();
                break;
            case 2:
                viewBooks();
                break;
            case 3:
                updateBook();
                break;
            case 4:
                deleteBook();
                break;
            case 5:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (adminChoice != 5);
}

// Function to add a book
void addBook() {
    FILE *file = fopen("books.txt", "a+");
    if (!file) {
        perror("Error opening books.txt");
        return;
    }

    Book book;
    printf("Enter book ID: ");
    scanf("%d", &book.id);
    printf("Enter book title: ");
    scanf("%s", book.title);
    printf("Enter book author: ");
    scanf("%s", book.author);
    printf("Enter book price: ");
    scanf("%f", &book.price);
    printf("Enter book quantity: ");
    scanf("%d", &book.quantity);

    fprintf(file, "%d %s %s %.2f %d\n", book.id, book.title, book.author, book.price, book.quantity);
    fclose(file);
    printf("Book added successfully!\n");
}

// Function to view books
void viewBooks() {
    FILE *file = fopen("books.txt", "r");
    if (!file) {
        perror("Error opening books.txt");
        return;
    }

    Book book;
    printf("\nAvailable Books:\n");
    printf("ID\tTitle\t\tAuthor\t\tPrice\tQuantity\n");
    while (fscanf(file, "%d %49s %49s %f %d\n", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        printf("%d\t%s\t\t%s\t\t%.2f\t%d\n", book.id, book.title, book.author, book.price, book.quantity);
    }

    fclose(file);
}

// Function to update a book
void updateBook() {
    FILE *file = fopen("books.txt", "r+");
    if (!file) {
        perror("Error opening books.txt");
        return;
    }

    int bookId;
    printf("Enter the ID of the book you want to update: ");
    scanf("%d", &bookId);

    Book book;
    long pos;
    while ((pos = ftell(file)) != -1 && fscanf(file, "%d %49s %49s %f %d\n", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        if (book.id == bookId) {
            printf("Enter new title: ");
            scanf("%s", book.title);
            printf("Enter new author: ");
            scanf("%s", book.author);
            printf("Enter new price: ");
            scanf("%f", &book.price);
            printf("Enter new quantity: ");
            scanf("%d", &book.quantity);

            // Ensure all statements are grouped in a block
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%d %s %s %.2f %d\n", book.id, book.title, book.author, book.price, book.quantity);
            fclose(file);
            printf("Book updated successfully!\n");
            return;
        }
    }

    printf("Book ID not found.\n");
    fclose(file);
}

// Function to delete a book
void deleteBook() {
    FILE *file = fopen("books.txt", "r");
    if (!file) {
        perror("Error opening books.txt");
        return;
    }

    int bookId;
    printf("Enter the ID of the book you want to delete: ");
    scanf("%d", &bookId);

    Book book;
    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        perror("Error opening temp.txt");
        fclose(file);
        return;
    }

    while (fscanf(file, "%d %49s %49s %f %d\n", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        if (book.id != bookId) {
            fprintf(tempFile, "%d %s %s %.2f %d\n", book.id, book.title, book.author, book.price, book.quantity);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("books.txt");
    rename("temp.txt", "books.txt");
    printf("Book deleted successfully!\n");
}
