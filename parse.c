/** 
 * parse.c
 * 
 * This file contains the implementation of the parse_line function, which
 * parses a given input line into an array of arguments.
 * 
 * Version: 2025-09-15
 *          2025-09-16 (Final version)
 *
 * Author:  Rasmus Mikaelsson (et24rmn)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"

#define MAX_LINE_LENGTH 1024

/**
 * parse_line - Parses a given input line into an array of arguments.
 * @line: Input string containing a command and its arguments.
 *
 * Returns a pointer to a static array of strings representing the arguments.
 * Note: The returned array points to static memory and is overwritten on each call.
 */
char **parse_line(char *line) {
    // Remove newline character if present
    line[strcspn(line, "\n")] = 0;

    // Tokenize line into arguments separated by spaces, store in tmp
    static char *tmp[MAX_LINE_LENGTH];
    int count = 0;
    char *token = strtok(line, " ");
    while (token != NULL) {
        tmp[count++] = token;
        token = strtok(NULL, " ");
    }
    tmp[count] = NULL;
    return tmp;
}
