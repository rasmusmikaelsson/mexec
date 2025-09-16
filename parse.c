#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"

#define MAX_LINE_LENGTH 1024

/**
 * Parse a line into command and arguments
 * Returns a NULL-terminated array of strings
 * 
 * @param line The line to parse
 * @return A NULL-terminated array of strings
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