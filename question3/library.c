#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static Book *books = NULL;
static int book_count = 0;
static int capacity = 0;

static const char *file_name = "inventory.txt";

static void clear_input(void) {
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

static void remove_newline(char text[]) {
    text[strcspn(text, "\n")] = '\0';
}

static int get_required_text(const char *message, char text[], int size, const char *field_name) {
    printf("%s", message);
    if (fgets(text, size, stdin) == NULL) {
        printf("Error: Couldn't read %s.\n", field_name);
        return 0;
    }
    if (strchr(text, '\n') == NULL) { // clear extra chars
        clear_input();
    }
    remove_newline(text);
    if (text[0] == '\0') {
        printf("%s can't be empty.\n", field_name);
        return 0;
    }
    return 1;
}

static int find_book(int book_id) { // find by ID
    for (int i = 0; i < book_count; i++) {
        if (books[i].book_id == book_id) {
            return i;
        }
    }
    return -1;
}

static int increase_capacity(void) { // inc array size
    Book *temp;
    if (capacity == 0) {
        capacity = 4;
        temp = malloc(capacity * sizeof(Book));
    } else {
        capacity *= 2;
        temp = realloc(books, capacity * sizeof(Book));
    }
    if (temp == NULL) {
        printf("Error: Mem allocation failed.\n");
        return 0;
    }
    books = temp;
    return 1;
}

void load_books_from_file(void) {
    FILE *file;
    Book book;
    file = fopen(file_name, "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%d\n", &book.book_id) == 1) {
        if (fgets(book.title, sizeof(book.title), file) == NULL) {
            break;
        }
        remove_newline(book.title);
        if (fgets(book.author, sizeof(book.author), file) == NULL) {
            break;
        }
        remove_newline(book.author);
        if (fgets(book.category, sizeof(book.category), file) == NULL) {
            break;
        }
        remove_newline(book.category);
        if (fscanf(file, "%d\n", &book.copies) != 1) {
            break;
        }
        if (book_count == capacity) {
            if (!increase_capacity()) {
                fclose(file);
                return;
            }
        }
        books[book_count] = book;
        book_count++;
    }
    fclose(file);
}

int save_books_to_file(void) {
    FILE *file;
    file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error: Couldn't open inventory.txt.\n");
        return 0;
    }
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%d\n%s\n%s\n%s\n%d\n", books[i].book_id, books[i].title, books[i].author, books[i].category, books[i].copies);
    }
    fclose(file);
    return 1;
}

void add_book(void) {
    Book book;
    printf("\nAdd new book :)\nEnter book ID: ");
    if (scanf("%d", &book.book_id) != 1) {
        printf("Invalid ID.\n");
        clear_input();
        return;
    }
    clear_input();
    if (book.book_id < 0) {
        printf("ID can't be negative.\n");
        return;
    }
     if (find_book(book.book_id) != -1) { // no duplicates
        printf("Error: ID exists.\n");
        return;
    }
    if (!get_required_text("Enter title: ", book.title, sizeof(book.title), "Title")) {
        return;
    }
    if (!get_required_text("Enter author: ", book.author, sizeof(book.author), "Author")) {
        return;
    }
    if (!get_required_text("Enter category: ", book.category, sizeof(book.category), "Category")) {
        return;
    }
    printf("Enter number of Copies: ");
    if (scanf("%d", &book.copies) != 1) {
        printf("Invalid number of copies.\n");
        clear_input();
        return;
    }
    clear_input();
    if (book.copies < 0) {
        printf("Copies can't be negative.\n");
        return;
    }
    if (book_count == capacity) {
        if (!increase_capacity()) {
            return;
        }
    }
    books[book_count] = book;
    book_count++;
    printf("Success: Book added:)\n");
}

void display_books(void) {
    printf("\nCurrent Inventory...\n");
    if (book_count == 0) {
        printf("Inventory is empty.\n");
        return;
    }
    for (int i = 0; i < book_count; i++) {
	printf("\nBook %d\nID: %d\nTitle: %s\nAuthor: %s\nCategory: %s\nCopies: %d\n",
	    i + 1, books[i].book_id, books[i].title, books[i].author, books[i].category, books[i].copies);
    }
}

void update_book(void) {
    int book_id;
    int index;
    char input[100];
    int copies;
    printf("\nUpdate book information...\n");
    if (book_count == 0) {
        printf("No books available.\n");
        return;
    }
    printf("Enter ID to update: ");
    if (scanf("%d", &book_id) != 1) {
        printf("Invalid ID.\n");
        clear_input();
        return;
    }
    clear_input();
    index = find_book(book_id);
    if (index == -1) {
        printf("Error: ID not found.\n");
        return;
    }
    printf("Leave blank to keep current values.\n");
    printf("\nCurrent title: %s\nNew title: ", books[index].title);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        remove_newline(input);
        if (input[0] != '\0') {
            strcpy(books[index].title, input);
        }
    }
    printf("\nCurrent author: %s\nNew author: ", books[index].author);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        remove_newline(input);
        if (input[0] != '\0') {
            strcpy(books[index].author, input);
        }
    }
    printf("\nCurrent Category: %s\nNew Category: ", books[index].category);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        remove_newline(input);
        if (input[0] != '\0') {
            strcpy(books[index].category, input);
        }
    }
    printf("\nCurrent Copies: %d\nNew Copies: ", books[index].copies);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        remove_newline(input);
        if (input[0] != '\0') {
            if (sscanf(input, "%d", &copies) == 1 &&
                copies >= 0) {
                books[index].copies = copies;
            } else {
                printf("Invalid copies. Current value kept.\n");
            }
        }
    }
    printf("Success: Book updated.\n");
}

void delete_book(void) {
    int book_id;
    int index;
    printf("\nDelete existing book?\n");
    if (book_count == 0) {
        printf("No books available.\n");
        return;
    }
    printf("Enter ID to delete: ");
    if (scanf("%d", &book_id) != 1) {
        printf("Invalid ID.\n");
        clear_input();
        return;
    }
    clear_input();
    index = find_book(book_id);
    if (index == -1) {
        printf("Error: ID not found.\n");
        return;
    }
    for (int i = index; i < book_count - 1; i++) {
        books[i] = books[i + 1];
    }
    book_count--;
    printf("Success: Book deleted.\n");
}

void search_menu(void) {
    int choice;
    printf("\nSearch Book...\n1) Search by ID\n2) Search by title\nChoose an option (1-2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input();
        return;
    }
    clear_input();
    switch (choice) {
        case 1:
            search_by_id();
            break;
        case 2:
            search_by_title();
            break;
        default:
            printf("Invalid option.\n");
    }
}

void search_by_id(void) {
    int book_id;
    int index;
    if (book_count == 0) {
        printf("Inventory is empty.\n");
        return;
    }
    printf("Enter ID: ");
    if (scanf("%d", &book_id) != 1) {
        printf("Invalid ID.\n");
        clear_input();
        return;
    }
    clear_input();
    index = find_book(book_id);
    if (index == -1) {
        printf("No book matches ID.\n");
        return;
    }
    printf("\nBook found :)\nID: %d\nTitle: %s\nAuthor: %s\nCategory: %s\nCopies: %d\n",
	books[index].book_id, books[index].title, books[index].author, books[index].category, books[index].copies);
}

void search_by_title(void) {
    char title[100];
    int found = 0;
    if (book_count == 0) {
        printf("Inventory is empty.\n");
        return;
    }
    if (!get_required_text("Enter title: ", title, sizeof(title), "Title")) {
        return;
    }
    for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].title, title) != NULL) {
	    printf("\nBook found:\nID: %d\nTitle: %s\nAuthor: %s\nCategory: %s\nCopies: %d\n",
		books[i].book_id, books[i].title, books[i].author, books[i].category, books[i].copies);
            found = 1;
        }
    }
    if (!found) {
        printf("No book matches '%s'.\n", title);
    }
}

void sort_menu(void) {
    int choice;
    if (book_count < 2) {
        printf("Not enough books to sort.\n");
        return;
    }
    printf("\nSort Inventory...\n1) Sort by book ID\n2) Sort by title\n3) Sort by available copies\n");
    printf("Choose an option (1-3): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input();
        return;
    }
    clear_input();
    if (choice < 1 || choice > 3) {
        printf("Invalid sorting option.\n");
        return;
    }
    for (int i = 0; i < book_count - 1; i++) { // bubble
        for (int j = 0; j < book_count - i - 1; j++) {
            int should_swap = 0;
            if (choice == 1 &&
                books[j].book_id > books[j + 1].book_id) {
                should_swap = 1;
            }
            if (choice == 2 &&
                strcmp(books[j].title,
                       books[j + 1].title) > 0) {
                should_swap = 1;
            }
            if (choice == 3 &&
                books[j].copies > books[j + 1].copies) {
                should_swap = 1;
            }
            if (should_swap) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
    printf("Success: Inventory sorted.\n");
}

void generate_reports(void) {
    int total_copies = 0;
    int highest_index = 0;
    char categories[100][50];
    int category_counts[100] = {0};
    int category_total = 0;
    printf("\nLIBRARY INVENTORY REPORT\n");
    if (book_count == 0) {
        printf("No books available.\n");
        return;
    }
    for (int i = 0; i < book_count; i++) {
        int found = 0;
        total_copies += books[i].copies;
        if (books[i].copies > books[highest_index].copies) {
            highest_index = i;
        }
        for (int j = 0; j < category_total; j++) { // category count
            if (strcmp(categories[j],
                       books[i].category) == 0) {
                category_counts[j]++;
                found = 1;
                break;
            }
        }
        if (!found && category_total < 100) {
            strcpy(categories[category_total],
                   books[i].category);
            category_counts[category_total]++;
            category_total++;
        }
    }
    printf("Total books: %d\nTotal copies: %d\n", book_count, total_copies);
    printf("Book with most copies: %s (%d copies)\n", books[highest_index].title, books[highest_index].copies);
    printf("\nBooks in each category:\n");
    for (int i = 0; i < category_total; i++) {
        printf("- %s: %d\n",
               categories[i],
               category_counts[i]);
    }
}

void free_inventory(void) {
    free(books);
    books = NULL;
    book_count = 0;
    capacity = 0;
}

// main program menu
void menu(void) {
    int choice;
    while (1) {
        printf("\n\nLIBRARY INVENTORY MANAGEMENT SYSTEM\n\n");
        printf("1) Add book\n2) Display all books\n3) Update info\n4) Delete book\n");
	printf("5) Search for book\n6) Sort inventory\n7) Generate report\n");
	printf("8) Save & Exit\n\nPlease select an option (1-8): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input();
            continue;
        }
        clear_input();
        switch (choice) {
            case 1:
                add_book();
                break;
            case 2:
                display_books();
                break;
            case 3:
                update_book();
                break;
            case 4:
                delete_book();
                break;
            case 5:
                search_menu();
                break;
            case 6:
                sort_menu();
                break;
            case 7:
                generate_reports();
                break;
            case 8:
                if (save_books_to_file()) {
                    printf("Inventory saved. Goodbye!\n");
                } else {
                    printf("Error: Inventory not saved.\n");
                }
                free_inventory();
                return;
            default:
                printf("Invalid choice. Please enter a number (1-8).\n");
        }
    }
}

