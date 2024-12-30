#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_BOOKS 100

typedef struct {
    char username[50];
    char password[50];
    char role[10]; // Admin or Member
} User;

typedef struct {
    int id;
    char title[50];
    char author[50];
    float price;
    int quantity;
} Book;

// Function prototypes
void registerUser();
int loginUser(char *role);
void viewBooks();
void borrowBook();

int main() {
    int choice;
    char role[10];

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(role)) {
                    if (strcmp(role, "Member") == 0) {
                        int memberChoice;
                        do {
                            printf("\nMember Menu\n");
                            printf("1. View Books\n");
                            printf("2. Borrow Book\n");
                            printf("3. Logout\n");
                            printf("Enter your choice: ");
                            scanf("%d", &memberChoice);

                            switch (memberChoice) {
                                case 1:
                                    viewBooks();
                                    break;
                                case 2:
                                    borrowBook();
                                    break;
                                case 3:
                                    printf("Logging out...\n");
                                    break;
                                default:
                                    printf("Invalid choice. Try again.\n");
                            }
                        } while (memberChoice != 3);
                    }
                }
                break;
            case 3:
                printf("Exiting the system...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

// Function to register a new user
void registerUser() {
    FILE *file = fopen("users.txt", "a+");
    if (!file) {
        perror("Error opening users.txt");
        return;
    }

    User user;
    printf("Enter username: ");
    scanf("%s", user.username);

    // Check for unique username
    User temp;
    while (fscanf(file, "%s %s %s\n", temp.username, temp.password, temp.role) != EOF) {
        if (strcmp(temp.username, user.username) == 0) {
            printf("Username already exists. Try again.\n");
            fclose(file);
            return;
        }
    }

    printf("Enter password: ");
    scanf("%s", user.password);
    printf("Enter role (Admin/Member): ");
    scanf("%s", user.role);

    fprintf(file, "%s %s %s\n", user.username, user.password, user.role);
    fclose(file);
    printf("Registration successful!\n");
}

// Function to log in a user
int loginUser(char *role) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        perror("Error opening users.txt");
        return 0;
    }

    char username[50], password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    User user;
    while (fscanf(file, "%s %s %s\n", user.username, user.password, user.role) != EOF) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            strcpy(role, user.role);
            fclose(file);
            printf("Login successful! Welcome, %s.\n", role);
            return 1;
        }
    }

    fclose(file);
    printf("Invalid username or password. Try again.\n");
    return 0;
}

// Function to view books
void viewBooks() {
    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("No books available yet. Please check back later!\n");
        return;
    }

    Book book;
    printf("\nAvailable Books:\n");
    printf("ID\tTitle\t\t\tAuthor\t\tPrice\tQuantity\n");
    while (fscanf(file, "%d %49s %49s %f %d\n", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        printf("%d\t%-20s %-15s %.2f\t%d\n", book.id, book.title, book.author, book.price, book.quantity);
    }

    fclose(file);
}

// Function to borrow a book
void borrowBook() {
    FILE *file = fopen("books.txt", "r+");
    if (!file) {
        printf("No books available yet. Please check back later!\n");
        return;
    }

    int bookId;
    printf("Enter the ID of the book you want to borrow: ");
    scanf("%d", &bookId);

    Book book;
    long pos = -1;
    int found = 0;

    while (!found && (pos = ftell(file)) != -1 &&
           fscanf(file, "%d %49s %49s %f %d\n", &book.id, book.title, book.author, &book.price, &book.quantity) != EOF) {
        if (book.id == bookId) {
            found = 1;
            if (book.quantity > 0) {
                book.quantity--;
                fseek(file, pos, SEEK_SET);
                fprintf(file, "%d %s %s %.2f %d\n", book.id, book.title, book.author, book.price, book.quantity);
                fclose(file);
                printf("You successfully borrowed the book: %s\n", book.title);
            } else {
                printf("Book out of stock!\n");
            }
        }
    }

    if (!found) {
        printf("Book ID not found.\n");
    }

    fclose(file);
}
