#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include "analyzer.h"

// process and save
void *process_file(void *arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    printf(
        "Thread %d: Processing %s\n",
        data->thread_id,
        data->input_file
    );
    FILE *input = fopen(data->input_file, "r");
    if (input == NULL) {
        printf(
            "Thread %d: Could not open %s\n",
            data->thread_id,
            data->input_file
        );
        return NULL;
    }

    int lines = 0;
    int words = 0;
    int characters = 0;
    int in_word = 0;
    int ch;
    int last_character = '\0';

    // read file in characters
    while ((ch = fgetc(input)) != EOF) {
        characters++;
        last_character = ch;
        if (ch == '\n') {lines++;}
        if (isspace((unsigned char)ch)) {in_word = 0;}
	else if (in_word == 0) {words++; in_word = 1;}
    }

    // count last line if there's no \n
    if (characters > 0 && last_character != '\n') {lines++;}

    fclose(input);

    // create report
    FILE *output = fopen(data->output_file, "w");

    if (output == NULL) {
        printf(
            "Thread %d: Could not create %s\n",
            data->thread_id,
            data->output_file
        );
        return NULL;
    }

    fprintf(output, "File Analysis\n");
    fprintf(output, "File: %s\n", data->input_file);
    fprintf(output, "Lines: %d\n", lines);
    fprintf(output, "Words: %d\n", words);
    fprintf(output, "Characters: %d\n", characters);

    fclose(output);

    printf(
        "Thread %d: Finished. Report saved to %s\n",
        data->thread_id,
        data->output_file
    );
    return NULL;
}

