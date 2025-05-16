#include "LogSign.h"
#include "utils.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//----------------------------- DISPLAY ------------------------------//

void FirstDisplay(int *choice) {
    printf("1. LogIn\n");
    printf("2. SignUp\n");
    printf("3. Exit\n");
    printf("Pick an option: ");
    scanf("%d", choice);
}

//----------------------------- LOGIN & SIGNUP ------------------------------//

int load_accounts_from_file(const char *filename, ACCOUNT accounts[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1;
    }

    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        int account_id;
        char username[MAX_NAME_LENGTH], password[MAX_PASS_LENGTH];

        if (sscanf(buffer, "%d:%29[^,],%19[^\n]", &account_id, username, password) == 3) {
            //clean up username and password to prevent invisible space difference
            removeUnwantedSpace(username);
            removeUnwantedSpace(password);

            //store the details in struct
            strcpy(accounts[count].Username, username);
            strcpy(accounts[count].Password, password);
            count++;
            if (count >= MAX_NUM_ACC) break;
        } else {
            printf("Warning: Skipping invalid line: %s", buffer);
        }
    }

    fclose(file);
    return count;
}

bool LogIn(ACCOUNT account[], int accounts_count, char **loggedInUsername) {
    if (accounts_count == 0) {
        ClearScreen();
        printf("No account exists\n");
        PauseScreen(1000);
        return false;
    }

    ClearScreen();
    char *username = get_input("Username: ", MAX_NAME_LENGTH);

    for (int i = 0; i < accounts_count; i++) {
        if (strcmp(username, account[i].Username) == 0) {
            ClearScreen();
            char *password = get_input("Password: ", MAX_PASS_LENGTH);
            if (strcmp(password, account[i].Password) == 0) {
                ClearScreen();
                printf("Welcome back, %s!\n", account[i].Username);
                *loggedInUsername = username;
                free(password);
                PauseScreen(1000);
                return true;
            } else {
                ClearScreen();
                printf("Incorrect password\n");
                free(username);
                free(password);
                PauseScreen(1000);
                printf("Going Back to Menu....\n");
                PauseScreen(1400);
                return false;
            }
        }
    }

    ClearScreen();
    printf("%s doesn't exist\n", username);
    free(username);
    PauseScreen(1000);
    return false;
}

void SignUp(ACCOUNT account[], int *accounts_count) {
    ClearScreen();

    if (*accounts_count >= MAX_NUM_ACC) {
        printf("Reached maximum number of accounts\n");
        PauseScreen(1000);
        return;
    }

    int current_acc = *accounts_count;
    char *temp_username;

    do {
        ClearScreen();
        temp_username = get_input("Create username: ", MAX_NAME_LENGTH);
        if (ValidUsername(temp_username, account, *accounts_count)) {
            strcpy(account[current_acc].Username, temp_username);
            free(temp_username);
            break;
        } else {
            printf("Username already taken\n");
            PauseScreen(1000);
            printf("Going Back to Menu....\n");
            PauseScreen(1400);
            free(temp_username);
            return;
        }
    } while (1);

    char *temp_password;

    do {
        ClearScreen();
        temp_password = get_input("Create password: ", MAX_PASS_LENGTH);
        if (ValidatePass(temp_password)) {
            strcpy(account[current_acc].Password, temp_password);
            free(temp_password);
            break;
        } else {
            printf("Password not strong enough\n");
            PauseScreen(1000);
            free(temp_password);
        }
    } while (1);

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        perror("Error opening file in append mode\n");
        PauseScreen(1000);
        exit(1);
    }

    fprintf(file, "%d:%s,%s\n", current_acc,
            account[current_acc].Username,
            account[current_acc].Password);
    fclose(file);

    (*accounts_count)++;
    
    printf("Account Created Successfully.\n");
    PauseScreen(1000);
}
