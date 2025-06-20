#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "LogSign.h"
#include <stddef.h> 

// Input Validation
bool ValidUsername(const char *username, ACCOUNT account[], int count);
bool ValidatePass(char *password);
void removeUnwantedSpace(char *str);

// UI and I/O
void ClearScreen();
void PauseScreen(int time);
void clear_input_buffer();
char* get_input(const char *prompt, int size);
void format_timestamp(char *buf, size_t size);

#endif