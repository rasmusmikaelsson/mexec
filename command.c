/**
 * command.c
 * 
 * Command handling functions for inter-process communication.
 * Includes functions to execute commands and read commands from a file.
 * Uses pipes for communication between child processes.
 * 
 * Version: 2025-09-15
 * Author:  Rasmus Mikaelsson (et24rmn)
 */

#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define READ_END 0
#define WRITE_END 1
#define MAX_FILE_CMD_LENGTH 1024

/**
 * Free the allocated memory for commands
 * 
 * @param commands The array of commands
 * @param cmd_count The number of commands
 */
void free_commands(char **commands, int cmd_count) {
    for (int i = 0; i < cmd_count; i++) {
        free(commands[i]);
    }
    free(commands);
}

/**
 * Execute a command in a child process
 * 
 * @param child_to_child The pipe to write to
 * @param resultingLine The command to execute
 * @return nothing, exits on failure
 */
void execute_command(int child_to_child[], char **resultingLine) {
    close(child_to_child[READ_END]);
    if (dup2(child_to_child[WRITE_END], STDOUT_FILENO) == -1) {
        perror("dup2 failed");
        exit(EXIT_FAILURE);
    }
    close(child_to_child[WRITE_END]);
    if(execvp(resultingLine[0], resultingLine) == -1){
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

/**
 * Read commands from a file
 * 
 * @param file The file to read from
 * @param commands_ptr Pointer to store the array of commands
 * @return The number of commands read
 */
int read_commands(FILE *file, char ***commands_ptr) {
    size_t capacity = 10;
    char **commands = malloc(capacity * sizeof(char *));
    if (!commands) {
        perror("malloc failed");
        return -1;
    }

    size_t cmd_count = 0;
    char line[MAX_FILE_CMD_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        if (cmd_count == capacity) {
            capacity *= 2;
            char **tmp = realloc(commands, capacity * sizeof(char *));
            if (!tmp) {
                perror("realloc failed");
                free_commands(commands, cmd_count);
                return -1;
            }
            commands = tmp;
        }
        commands[cmd_count] = strdup(line);
        cmd_count++;
    }
    *commands_ptr = commands;
    return (int)cmd_count;
}
