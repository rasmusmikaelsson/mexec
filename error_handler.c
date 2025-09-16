/**
 * Header file for error handling functions.
 * Provides declarations for functions that validate file operations
 * and memory allocations.
 * 
 * Version: 2025-09-15
 * Author:  Rasmus Mikaelsson (et24rmn)
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "error_handler.h"

#define WRITE_END 1
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

/**
 * Validates if a file was opened successfully.
 * If the file is NULL, prints an error message and exits.
 * 
 * @param file Pointer to the FILE object to validate.
 * @return void
 */
void validate_file(FILE *file) {
    if (file == NULL) {
        perror(RED "Error opening file" RESET);
        exit(EXIT_FAILURE);
    }
}

/**
 * Validates if memory allocation for commands was successful.
 * If the commands pointer is NULL, prints an error message and exits.
 * 
 * @param commands Pointer to the allocated commands array.
 * @return void
 */
void validate_commands_allocation(char **commands) {
    if (!commands) {
        perror(RED "Memory allocation failed" RESET);
        exit(EXIT_FAILURE);
    }
}

/**
 * Validates if memory reallocation was successful.
 * If the tmp pointer is NULL, prints an error message, frees previously
 * allocated commands, and exits.
 * 
 * @param tmp Pointer to the reallocated memory.
 * @param commands Pointer to the original commands array.
 * @param cmd_count Number of commands currently allocated.
 * @return void
 */
void validate_reallocation(char **tmp) {
    if (!tmp) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
}

/**
 * Validates if a return value is non-negative.
 * If the return value is negative, prints an error message and exits.
 * 
 * @param return_value The return value to validate.
 * @return void
 */
void validate_non_negative(int return_value) {
    if (return_value < 0) {
        perror("System call failed");
        exit(EXIT_FAILURE);
    }
}

/**
 * Validates if dup2 was successful.
 * If the result is negative, prints an error message and exits.
 * 
 * @param result The result of the dup2 call to validate.
 * @param child_to_child The pipe array to close on error.
 * @return void
 */
void validate_dup2(int result, int child_to_child[]) {
    if (result < 0) {
        perror(RED "dup2 failed" RESET);
        close(child_to_child[WRITE_END]);
        exit(EXIT_FAILURE);
    }
}
