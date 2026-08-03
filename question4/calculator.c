#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculator.h"
#define HISTORY_FILE "history.txt"

ConversionRecord *history = NULL;
int record_count = 0;
int history_capacity = 0;

void clear_input() {
    int character;
    while ((character = getchar()) != '\n' &&
           character != EOF) {
    }
}


double c_to_f(double value) {
    return (value * 9.0 / 5.0) + 32.0;
}
double f_to_c(double value) {
    return (value - 32.0) * 5.0 / 9.0;
}
double km_to_mi(double value) {
    return value * 0.621371;
}
double mi_to_km(double value) {
    return value / 0.621371;
}
double kg_to_lb(double value) {
    return value * 2.20462;
}
double lb_to_kg(double value) {
    return value / 2.20462;
}
double cm_to_in(double value) {
    return value * 0.393701;
}
double in_to_cm(double value) {
    return value / 0.393701;
}


void add_record(const char *type, double input, double output) {
    ConversionRecord *temp;
    if (record_count == history_capacity) {
        if (history_capacity == 0) {
            history_capacity = 5;
        } else {
            history_capacity *= 2;
        }
        temp = realloc(
            history,
            history_capacity * sizeof(ConversionRecord)
        );
        if (temp == NULL) {
            printf("Error: Conversion not saved.\n");
            return;
        }
        history = temp;
    }
    strcpy(history[record_count].type, type);
    history[record_count].input = input;
    history[record_count].output = output;
    record_count++;
}

void perform_conversion() {
    const char *names[8] = {
        "Celsius to Fahrenheit",
        "Fahrenheit to Celsius",
        "km to miles",
        "miles to km",
        "kg to pounds",
        "pounds to kg",
        "cm to inches",
        "inches to cm"
    };

    ConversionFunction conversions[8] = {c_to_f, f_to_c, km_to_mi, mi_to_km, kg_to_lb, lb_to_kg, cm_to_in, in_to_cm};
    
    int choice;
    double input;
    double output;
    
    printf("\nChoose a conversion\n");
    for (int i = 0; i < 8; i++) {
        printf("%d) %s\n", i + 1, names[i]);
    }
    printf("Choose an option (1-8): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input();
        return;
    }
    if (choice < 1 || choice > 8) {
        printf("Invalid choice.\n");
        clear_input();
        return;
    }
    printf("Enter value: ");
    if (scanf("%lf", &input) != 1) {
        printf("Invalid value.\n");
        clear_input();
        return;
    }
    clear_input();
    // funct pointer to convert
    output = conversions[choice - 1](input);
    printf("Result: %.4f\n", output);
    add_record(
        names[choice - 1],
        input,
        output
    );
}

void view_history() {
    printf("\nConversion History\n");
    if (record_count == 0) {
        printf("No conversions saved.\n");
        return;
    }
    for (int i = 0; i < record_count; i++) {
        printf("%d) %s\n", i + 1, history[i].type);
        printf("   Input: %.4f\n", history[i].input);
        printf("   Output: %.4f\n", history[i].output);
    }
}

void search_records() {
    int choice;
    if (record_count == 0) {
        printf("No records available.\n");
        return;
    }
    printf("\nSearch History\n");
    printf("1) Search by conversion type\n2) Search by converted value\n");
    printf("Choose an option: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input();
        return;
    }
    clear_input();
    if (choice == 1) {
        char search[50];
        int found = 0;
        printf("Enter conversion type: ");
        if (fgets(search, sizeof(search), stdin) == NULL) {
            return;
        }
        search[strcspn(search, "\n")] = '\0';
        for (int i = 0; i < record_count; i++) {
            if (strstr(history[i].type, search) != NULL) {
                printf("%d) %s | Input: %.4f | Output: %.4f\n", i + 1, history[i].type, history[i].input, history[i].output);
                found = 1;
            }
        }
        if (!found) {
            printf("No records match.\n");
        }
    }
    else if (choice == 2) {
        double value;
        int found = 0;
        printf("Enter converted value: ");
        if (scanf("%lf", &value) != 1) {
            printf("Invalid value.\n");
            clear_input();
            return;
        }
        clear_input();
        for (int i = 0; i < record_count; i++) {
            if (fabs(history[i].output - value) < 0.01) {
                printf("%d) %s | Input: %.4f | Output: %.4f\n", i + 1, history[i].type, history[i].input, history[i].output);
                found = 1;
            }
        }
        if (!found) {
            printf("No records match.\n");
        }
    }
    else {
        printf("Invalid choice.\n");
    }
}

// sorting callbacks
int compare_by_type(
    const ConversionRecord *first,
    const ConversionRecord *second
) {
    return strcmp(
        first->type,
        second->type
    );
}
int compare_by_value(
    const ConversionRecord *first,
    const ConversionRecord *second
) {
    if (first->output > second->output) {
        return 1;
    }
    if (first->output < second->output) {
        return -1;
    }
    return 0;
}

// sort using comparison
void sort_records(CompareCallback compare) {
    if (record_count < 2) {
        printf("Not enough records to sort.\n");
        return;
    }
    for (int i = 0; i < record_count - 1; i++) {
        for (int j = 0; j < record_count - i - 1; j++) {
            if (compare(&history[j], &history[j + 1]) > 0) {
                ConversionRecord temp = history[j];
                history[j] = history[j + 1];
                history[j + 1] = temp;
            }
        }
    }
    printf("History sorted.\n");
}

void sort_history() {
    int choice;
    printf("\nSort History\n");
    printf("1) Sort by conversion type\n2) Sort by converted value\n");
    printf("Choose an option: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input();
        return;
    }
    clear_input();
    if (choice == 1) {
        sort_records(compare_by_type);
    }
    else if (choice == 2) {
        sort_records(compare_by_value);
    }
    else {
        printf("Invalid choice.\n");
    }
}

void change_precision(
    ConversionRecord *record,
    int precision
) {
    double multiplier = pow(10, precision);
    record->output = round(record->output * multiplier) / multiplier;
}

int filter_by_value(
    const ConversionRecord *record,
    double value
) {
    return record->output > value;
}

void apply_callbacks() {
    int choice;
    if (record_count == 0) {
        printf("No records available.\n");
        return;
    }
    printf("\nCallback Operations\n");
    printf("1) Change output precision\n2) Show outputs above value\n");
    printf("Choose an option: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clear_input();
        return;
    }
    if (choice == 1) {
        int precision;
        printf("Enter decimal places (0-6): ");
        if (scanf("%d", &precision) != 1) {
            printf("Invalid precision.\n");
            clear_input();
            return;
        }
        clear_input();
        if (precision < 0 || precision > 6) {
            printf(
                "Precision must be 0-6.\n"
            );
            return;
        }
        // apply to all
        for (int i = 0; i < record_count; i++) {
            change_precision(
                &history[i],
                precision
            );
        }
        printf("Output values updated.\n");
    }
    else if (choice == 2) {
        double value;
        int found = 0;
        printf("Enter minimum output value: ");
        if (scanf("%lf", &value) != 1) {
            printf("Invalid value.\n");
            clear_input();
            return;
        }
        clear_input();
        // filter
        for (int i = 0; i < record_count; i++) {
            if (filter_by_value(&history[i], value)) {
                printf("%d) %s | Output: %.4f\n", i + 1, history[i].type, history[i].output);
                found = 1;
            }
        }
        if (!found) {
            printf("No records match.\n");
        }
    }
    else {
        printf("Invalid choice.\n");
    }
}

int save_history() {
    FILE *file;
    file = fopen(HISTORY_FILE, "w");
    if (file == NULL) {
        printf("Error: History not saved.\n");
        return 0;
    }
    for (int i = 0; i < record_count; i++) {
        fprintf(file, "%s|%.10f|%.10f\n", history[i].type, history[i].input, history[i].output);
    }
    fclose(file);
    return 1;
}

int load_history() {
    FILE *file;
    char type[50];
    double input;
    double output;
    file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return 0;
    }
    free_history();
    while (fscanf(file, "%49[^|]|%lf|%lf\n", type, &input, &output) == 3) {
        add_record(type, input, output);
    }
    fclose(file);
    return 1;
}

void free_history() {
    free(history);
    history = NULL;
    record_count = 0;
    history_capacity = 0;
}

// main menu
void menu() {
    int choice;
    while (1) {
        printf("\n\nSMART CALCULATOR\n\n");
        printf("1) Perform conversion\n2) View conversion history\n3) Search records\n");
        printf("4) Sort records\n5) Apply callbacks\n6) Save history\n7) Load history\n");
        printf("8) Exit\n\nChoose an option (1-8): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input();
            continue;
        }
        clear_input();
        switch (choice) {
            case 1:
                perform_conversion();
                break;
            case 2:
                view_history();
                break;
            case 3:
                search_records();
                break;
            case 4:
                sort_history();
                break;
            case 5:
                apply_callbacks();
                break;
            case 6:
                if (save_history()) {printf("History saved.\n");}
                break;
            case 7:
                if (load_history()) {printf("History loaded.\n");} else {
                    printf("No history saved.\n"
                    );
                }
                break;
            case 8:
                save_history();
                free_history();
                printf("Goodbye!\n");
                return;
            default:
                printf("Invalid choice.\nEnter a number (1-8).\n");
        }
    }
}

