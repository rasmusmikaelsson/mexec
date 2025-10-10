/**
 * error_handler.c
 * 
 * This file contains implementations of error handling functions for file
 * operations and memory allocations.
 * 
 * Version: 2025-09-15
 *          2025-09-16 (Final version)
 * 
 * Author:  Rasmus Mikaelsson (et24rmn)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "error_handler.h"

#define WRITE_END 1
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

void validate_file(FILE *file) {
    if (file == NULL) {
        perror(RED "Error opening file" RESET);
        exit(EXIT_FAILURE);
    }
}

void validate_commands_allocation(char **commands) {
    if (!commands) {
        perror(RED "Memory allocation failed" RESET);
        exit(EXIT_FAILURE);
    }
}

void validate_reallocation(char **tmp) {
    if (!tmp) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
}

void validate_non_negative(int return_value) {
    if (return_value < 0) {
        perror("System call failed");
        exit(EXIT_FAILURE);
    }
}

void validate_dup2(int result, int child_to_child[]) {
    if (result < 0) {
        perror(RED "dup2 failed" RESET);
        close(child_to_child[WRITE_END]);
        exit(EXIT_FAILURE);
    }
}
