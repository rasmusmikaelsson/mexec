/**
 * mexec.c - A program that reads commands from a file and executes them in a pipeline.
 * If no file is provided, it reads commands from standard input.
 * Each command's output is piped to the next command's input.
 * The last command outputs to standard output.
 * 
 * Compile with:    make
 * Usage:           ./mexec [filename]
 *                  ./mexec (for stdin input)
 * 
 * Version:         2025-09-11
 *                  2025-09-15 (intermediate version)
 *                  2025-09-16 (final version)
 * 
 * Author:          Rasmus Mikaelsson (et24rmn)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "error_handler.h"
#include "pipeline.h"
#include "command.h"
#include "parse.h"

#define READ_END 0
#define WRITE_END 1
#define MAX_FILE_CMD_LENGTH 1024
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

/**
 * Main program - Reads commands from a file and executes them in a pipeline
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, 1 on failure (EXIT_FAILURE)
 */
int main(int argc, char **argv) {
    FILE *file = stdin;
    
    if (argc == 2) {
        file = fopen(argv[1], "r");
        validate_file(file);
    } else if (argc > 2) {
        fprintf(stderr, RED "Too many arguments. Usage: %s [filename]\n" RESET, argv[0]);
        exit(EXIT_FAILURE);
    }

    char **commands = NULL;
    int cmd_count = read_commands(file, &commands);
    if (file != stdin) fclose(file);

    // Previous file descriptor so that the
    // next child can read from the previous child
    int prev_fd = -1;
    for (int i = 0; i < cmd_count; i++) {
        int child_to_child[2] = {-1, -1};
        pid_t pid;

        // Create a pipe if not the last command
        if (i < cmd_count - 1) {
            if (pipe(child_to_child) == -1) {
                perror("pipe");
                free_commands(commands, cmd_count);
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid < 0) {
            perror("fork");
            if (child_to_child[READ_END] != -1) close(child_to_child[READ_END]);
            if (child_to_child[WRITE_END] != -1) close(child_to_child[WRITE_END]);
            free_commands(commands, cmd_count);
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            char **parsed_line = parse_line(commands[i]);
            if (!parsed_line) {
                fprintf(stderr, RED "Failed to parse command: %s\n" RESET, commands[i]);
                exit(EXIT_FAILURE);
            }

            int is_last = (i == cmd_count - 1);
            redirect_input(prev_fd);

            if (!is_last) {
                execute_command(child_to_child, parsed_line);
            } else {
                validate_non_negative(execvp(parsed_line[0], parsed_line));
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            if (prev_fd != -1) close(prev_fd);
            if (i < cmd_count - 1) {
                close(child_to_child[WRITE_END]);
                prev_fd = child_to_child[READ_END];
            }
        }
    }
    if (prev_fd != -1) close(prev_fd); // Close last read end if open

    int exit_code = 0;
    for (int i = 0; i < cmd_count; i++) {
        int status;
        if (wait(&status) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            exit_code = WEXITSTATUS(status);
        }
    }


    // Free allocated memory at the end
    free_commands(commands, cmd_count);

    return exit_code;
}
