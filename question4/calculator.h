#ifndef CALCULATOR_H
#define CALCULATOR_H

// conversion record
typedef struct {
    char type[50];
    double input;
    double output;
} ConversionRecord;

// func pointer for conversions
typedef double (*ConversionFunction)(double);

// sorting callback
typedef int (*CompareCallback)(
    const ConversionRecord *first,
    const ConversionRecord *second
);

// main program func
void menu();
void free_history();

// conversion func
double c_to_f(double value);
double f_to_c(double value);
double km_to_mi(double value);
double mi_to_km(double value);
double kg_to_lb(double value);
double lb_to_kg(double value);
double cm_to_in(double value);
double in_to_cm(double value);

// history func
void add_record(const char *type, double input, double output);
void view_history();
void search_records();

// sorting func
void sort_records(CompareCallback compare);
int compare_by_type(
    const ConversionRecord *first,
    const ConversionRecord *second
);
int compare_by_value(
    const ConversionRecord *first,
    const ConversionRecord *second
);

// callback func
void apply_callbacks();
void change_precision(ConversionRecord *record, int precision);
int filter_by_value(const ConversionRecord *record, double value);

// file func
int save_history();
int load_history();

#endif

