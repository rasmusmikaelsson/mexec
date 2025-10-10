/**
 * mexec.c
 * 
 * A program that reads commands from a file and executes them in a pipeline.
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
#include "command.h"

#define READ_END 0
#define WRITE_END 1
#define MAX_LINE_LENGTH 1024
#define MAX_FILE_CMD_LENGTH 1024
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

/* ------------------ Declarations of internal functions ------------------ */

static void redirect_input(int prev_fd);
static char **parse_line(char *line);

/* -------------------------- External functions -------------------------- */

/**
 * Main program
 * Reads commands from a file and executes them in a pipeline
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, 1 on failure (EXIT_FAILURE)
 */
int main(int argc, char *argv[]) {
    char *filename;
    FILE *file = NULL;
   
    if (argc == 2) {
        filename = argv[1];
        file = fopen(filename, "r");
        validate_file(file);
    } else {
        fprintf(stderr, RED "Argument error." RESET " Usage: %s [filename]\n" , argv[0]);
		fclose(file);
        exit(EXIT_FAILURE);
    }

	// Initialize commands array and get number of commands
    char **commands = NULL;
    int cmd_count = read_commands(file, &commands);
	if(cmd_count < 0) {
		exit(EXIT_FAILURE);
	}

	// Loop the given commands
    int prev_fd = -1;
    for (int i = 0; i < cmd_count; i++) {
        int child_to_child[2] = {-1, -1};
        pid_t pid;

        // Create pipe if not last command
        if (i < cmd_count - 1) {
            if (pipe(child_to_child) == -1) {
                perror("pipe");
                free_commands(commands, cmd_count);
                exit(EXIT_FAILURE);
            }
        }

		// Fork a new process
        pid = fork();
        if (pid < 0) {
            perror("fork");
            if (child_to_child[READ_END] != -1) close(child_to_child[READ_END]);
            if (child_to_child[WRITE_END] != -1) close(child_to_child[WRITE_END]);
            free_commands(commands, cmd_count);
			fclose(file);
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            char **parsed_line = parse_line(commands[i]);
            if (!parsed_line) {
                fprintf(stderr, RED "Failed to parse command: %s\n" RESET, commands[i]);
				free_commands(commands, cmd_count);
				fclose(file);
                exit(EXIT_FAILURE);
            }
			
            redirect_input(prev_fd);

            int is_last = (i == cmd_count - 1);
            if (!is_last) {
                if(execute_command(child_to_child, parsed_line, commands, cmd_count) > 0) {
					exit(EXIT_FAILURE);
				}
            } else {
                validate_non_negative(execvp(parsed_line[0], parsed_line));
            }
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
	fclose(file);
    return exit_code;
}

/* -------------------------- Internal functions -------------------------- */

/**
 * redirect_input - Redirect stdin from previous pipe if needed.
 * @prev_fd: File descriptor of previous pipe read end, or -1 if none.
 */
static void redirect_input(int prev_fd) {
    if (prev_fd != -1) {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
}

/**
 * parse_line - Parses a given input line into an array of arguments.
 * @line: Input string containing a command and its arguments.
 *
 * Returns a pointer to a static array of strings representing the arguments.
 * Note: The returned array points to static memory and is overwritten on each call.
 */
static char **parse_line(char *line) {
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


