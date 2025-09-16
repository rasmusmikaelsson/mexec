/**
 * Pipeline handling functions.
 * Includes functions to redirect input and output for pipes.
 * 
 * Version: 2025-09-15
 * Author:  Rasmus Mikaelsson (et24rmn)
 */

#ifndef PIPELINE_H
#define PIPELINE_H

#include <unistd.h>

/**
 * redirect_input - Redirect stdin from previous pipe if needed.
 * 
 * @prev_fd: File descriptor of previous pipe read end, or -1 if none.
 */
void redirect_input(int prev_fd);

#endif // PIPELINE_H
