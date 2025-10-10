/**
 * error_handler.h
 * 
 * Header file for error handling functions.
 * Provides declarations for functions that validate file operations
 * and memory allocations.
 * 
 * Version: 2025-09-15
 * 
 * Author:  Rasmus Mikaelsson (et24rmn)
 */

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

/**
 * Validates if a file was opened successfully.
 * If the file is NULL, prints an error message and exits.
 * 
 * @param file Pointer to the FILE object to validate.
 * @return void
 */
void validate_file(FILE *file);

/**
 * Validates if memory allocation for commands was successful.
 * If the commands pointer is NULL, prints an error message and exits.
 * 
 * @param commands Pointer to the allocated commands array.
 * @return void
 */
void validate_commands_allocation(char **commands);

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
void validate_reallocation(char **tmp);

/**
 * Validates if a return value is non-negative.
 * If the return value is negative, prints an error message and exits.
 * 
 * @param return_value The return value to validate.
 * @return void
 */
void validate_non_negative(int return_value);

/**
 * Validates if dup2 was successful.
 * If the result is negative, prints an error message and exits.
 * 
 * @param result The result of the dup2 call to validate.
 * @param child_to_child The pipe array to close on error.
 * @return void
 */
void validate_dup2(int result, int child_to_child[]);

#endif // ERROR_HANDLER_H
