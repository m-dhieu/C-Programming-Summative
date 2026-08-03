#ifndef LIBRARY_H
#define LIBRARY_H

// book info
typedef struct {
    int book_id;
    char title[100];
    char author[100];
    char category[50];
    int copies;
} Book;

// start program menu
void menu(void);

// file func
void load_books_from_file(void);
int save_books_to_file(void);

// inventory func
void add_book(void);
void display_books(void);
void update_book(void);
void delete_book(void);

// search func
void search_menu(void);
void search_by_id(void);
void search_by_title(void);

// sort/report func
void sort_menu(void);
void generate_reports(void);

void free_inventory(void);

#endif

