#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "LogSign.h"
#include <stddef.h>

//----------------------------- INPUT VALIDATION ------------------------------//
bool ValidUsername(const char *username, ACCOUNT account[], int count);
bool ValidatePass(char *password);
void removeUnwantedSpace(char *str);

//----------------------------- SYSTEM / UTILITY ------------------------------//
void ClearScreen();
void PauseScreen(int time);
void clear_input_buffer();
void format_timestamp(char *buf, size_t size);

//----------------------------- ACCOUNT HELPER ------------------------------//
int find_account_index(const ACCOUNT accounts[], int count, const char *username);

//----------------------------- INPUT HANDLING ------------------------------//
char* get_input(const char *prompt, int size);
char* get_username_input(const char* prompt);
char* get_password_input(const char* prompt);
bool get_confirmation(const char* prompt);
void wait_for_exit_prompt(const char *message);

void playChatSound();

void simple_hash(const char *input, char *output, size_t out_size);
void simple_unhash(const char *hashed_input, char *output, size_t out_size);


#endif // UTILS_H
