/**
 * command.h
 * 
 * Header file for command handling functions in mexec. Defines functions for
 * reading commands from a file, executing commands, and freeing allocated
 * memory for commands.
 * 
 * Version:     2025-09-17
 * Author:      Rasmus Mikaelsson (et24rmn)
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

/**
 * Free the allocated memory for commands
 * 
 * @param commands The array of commands
 * @param cmd_count The number of commands
 */
void free_commands(char **commands, int cmd_count);

/**
 * Execute a command in a child process
 * 
 * @param child_to_child The pipe to write to
 * @param resultingLine The command to execute
 * @return nothing, exits on failure
 */
void execute_command(int child_to_child[], char **resultingLine);

/**
 * Read commands from a file
 * 
 * @param file The file to read from
 * @param commands_ptr Pointer to store the array of commands
 * @return The number of commands read
 */
int read_commands(FILE *file, char ***commands_ptr);

#endif // COMMAND_H
