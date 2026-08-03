#ifndef ANALYZER_H
#define ANALYZER_H

typedef struct {
    char input_file[256];
    char output_file[256];
    int thread_id;
} ThreadArgs;

void *process_file(void *arg);

#endif

