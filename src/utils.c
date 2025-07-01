#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>
#include <direct.h>
#include <errno.h>



//----------------------------- INPUT VALIDATION ------------------------------//

bool ValidUsername(const char *username, ACCOUNT account[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(username, account[i].Username) == 0) {
            return false;
        }
    }
    return true;
}

bool ValidatePass(char *password) {
    int Min_Length = 8;
    if (strlen(password) < Min_Length) {
        printf("Password must be at least %d characters long.\n", Min_Length);
        return false;
    }

    bool hasUpper = false, hasLower = false, isAlphaNum = true, noSpace = true;

    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper((unsigned char)password[i])) hasUpper = true;
        if (islower((unsigned char)password[i])) hasLower = true;
        if (!isalnum((unsigned char)password[i]) && password[i] != '!') isAlphaNum = false;
        if (isspace((unsigned char)password[i])) noSpace = false;
    }

    if (!hasUpper) printf("Password must contain at least one uppercase letter.\n");
    if (!hasLower) printf("Password must contain at least one lowercase letter.\n");
    if (!isAlphaNum) printf("Password must contain only alphanumeric characters or '!'.\n");
    if (!noSpace) printf("Password must not contain spaces.\n");

    return hasUpper && hasLower && isAlphaNum && noSpace;
}

void removeUnwantedSpace(char *str) {
    int len = (int)strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

//----------------------------- SYSTEM FUNCTIONS ------------------------------//

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void PauseScreen(int time) {
#ifdef _WIN32
    Sleep(time);
#else
    usleep(time * 1000);
#endif
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//----------------------------- TIME FUNCTIONS ------------------------------//

void format_timestamp(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buf, size, "%04d-%02d-%02d %02d:%02d:%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
}

//----------------------------- ACCOUNT HELPERS ------------------------------//

int find_account_index(const ACCOUNT accounts[], int count, const char *username) {
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].Username, username) == 0) {
            return i;
        }
    }
    return -1;
}

//----------------------------- INPUT HANDLING  ------------------------------//

char* get_input(const char *prompt, int size) {
    char *input = (char*)malloc(size * sizeof(char));
    if (!input) {
        printf("Memory Allocation failed\n");
        exit(1);
    }
    printf("%s", prompt);
    fgets(input, size, stdin);
    input[strcspn(input, "\n")] = '\0';
    return input;
}

char* get_username_input(const char* prompt) {
    char* username = get_input(prompt, MAX_NAME_LENGTH);
    if (_stricmp(username, "exit") == 0) {
        free(username);
        return NULL;
    }
    return username;
}

char* get_password_input(const char* prompt) {
    char* password = get_input(prompt, HASHED_PASS_LENGTH);
    if (_stricmp(password, "exit") == 0) {
        free(password);
        return NULL;
    }
    return password;
}

bool get_confirmation(const char* prompt) {
    char* response = get_input(prompt, 10);
    bool confirmed = (_stricmp(response, "yes") == 0);
    free(response);
    return confirmed;
}


void wait_for_exit_prompt(const char *message) {
    char input[32];

    while (1) {
        printf("%s", message);
        fgets(input, sizeof(input), stdin);

        // remove trailing newline
        input[strcspn(input, "\r\n")] = 0;

        if (_stricmp(input, "exit") == 0) break;

        ClearScreen();
    }
}

void simple_hash(const char *input, char *output, size_t out_size) {
    unsigned char key = 0x5A;
    size_t len = strlen(input);

    if (out_size < len * 2 + 1) {
        fprintf(stderr, "Output buffer too small!\n");
        exit(1);
    }

    for (size_t i = 0; i < len; i++) {
        unsigned char xored = input[i] ^ key;
        sprintf(output + i * 2, "%02X", xored);
    }
    output[len * 2] = '\0';
}

// Helper function — internal, no need to expose in header
static int hex_char_to_int(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('A' <= c && c <= 'F') return c - 'A' + 10;
    else if ('a' <= c && c <= 'f') return c - 'a' + 10;
    return -1;  // Invalid hex char
}

// The public function declared in utils.h
void simple_unhash(const char *hashed_input, char *output, size_t out_size) {
    size_t len = strlen(hashed_input);
    if (len % 2 != 0 || out_size < len / 2 + 1) {
        fprintf(stderr, "Invalid input length or output buffer too small\n");
        if (out_size > 0) output[0] = '\0';
        return;
    }

    unsigned char key = 0x5A;

    for (size_t i = 0; i < len / 2; i++) {
        int high = hex_char_to_int(hashed_input[i * 2]);
        int low = hex_char_to_int(hashed_input[i * 2 + 1]);
        if (high == -1 || low == -1) {
            fprintf(stderr, "Invalid hex character in hashed input\n");
            if (out_size > 0) output[0] = '\0';
            return;
        }
        unsigned char xored = (high << 4) | low;
        output[i] = xored ^ key;
    }
    output[len / 2] = '\0';
}