#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

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
        if (isupper(password[i])) hasUpper = true;
        if (islower(password[i])) hasLower = true;
        if (!isalnum(password[i]) && password[i] != '!') isAlphaNum = false;
        if (isspace(password[i])) noSpace = false;
    }

    if (!hasUpper) printf("Password must contain at least one uppercase letter.\n");
    if (!hasLower) printf("Password must contain at least one lowercase letter.\n");
    if (!isAlphaNum) printf("Password must contain only alphanumeric characters.\n");
    if (!noSpace) printf("Password must not contain spaces.\n");

    return hasUpper && hasLower && isAlphaNum && noSpace;
}

void removeUnwantedSpace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

//----------------------------- INPUT FUNCTIONS ------------------------------//

char* get_input(const char *prompt, int size) {
    char *input = (char*)malloc(size * sizeof(char));
    if (!input) {
        printf("Memory Allocation failed\n");
        exit(1);
    }
    printf("%s", prompt);
    fgets(input, size, stdin);
    input[strcspn(input, "\n")] = '\0';  // remove newline
    return input;
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
    Sleep(time);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
