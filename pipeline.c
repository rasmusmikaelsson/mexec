/**
 * Pipeline handling functions.
 * Includes functions to redirect input and output for pipes.
 * 
 * Version: 2025-09-15
 * Author:  Rasmus Mikaelsson (et24rmn)
 */

#include <unistd.h>
#include <stdio.h>
#include "pipeline.h"

/**
 * redirect_input - Redirect stdin from previous pipe if needed.
 * @prev_fd: File descriptor of previous pipe read end, or -1 if none.
 */
void redirect_input(int prev_fd) {
    if (prev_fd != -1) {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
}
