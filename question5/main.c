#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "analyzer.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    int file_count = argc - 1;

    pthread_t *threads = malloc(file_count * sizeof(pthread_t));

    ThreadArgs *thread_data = malloc(file_count * sizeof(ThreadArgs));

    if (threads == NULL || thread_data == NULL) {
        printf("Error: Mem allocation failedy.\n");
        free(threads);
        free(thread_data);
        return 1;
    }

    printf("\nProcessing %d file(s)...\n\n", file_count);

    // one thread for each file
    for (int i = 0; i < file_count; i++) {
        thread_data[i].thread_id = i + 1;
        snprintf(
            thread_data[i].input_file,
            sizeof(thread_data[i].input_file),
            "%s",
            argv[i + 1]
        );
        snprintf(
            thread_data[i].output_file,
            sizeof(thread_data[i].output_file),
            "report_%d.txt",
            i + 1
        );
        int result = pthread_create(
            &threads[i],
            NULL,
            process_file,
            &thread_data[i]
        );
        if (result != 0) {
            printf("Error: Thread not created %d.\n", i + 1);
            threads[i] = 0;
        }
    }

    // wait for created threads
    for (int i = 0; i < file_count; i++) {
        if (threads[i] != 0) {pthread_join(threads[i], NULL);
        }
    }

    printf("\nAll files processed.\n");
    free(threads);
    free(thread_data);
    return 0;
}

