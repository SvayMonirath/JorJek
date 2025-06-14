#include "LogSign.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------- LOAD_ACCOUNT ------------------------------//

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
        char username[MAX_NAME_LENGTH];
        char password[MAX_PASS_LENGTH];
        int role_int;

        // format: id:username,password,role
        if (sscanf(buffer, "%d:%29[^,],%19[^,],%d", &account_id, username, password, &role_int) == 4) {
            removeUnwantedSpace(username);
            removeUnwantedSpace(password);

            strcpy(accounts[count].Username, username);
            strcpy(accounts[count].Password, password);
            accounts[count].role = (role_int == 1) ? ROLE_ADMIN : ROLE_USER;

            count++;
            if (count >= MAX_NUM_ACC) break;
        } else {
            printf("Warning: Skipping invalid line: %s", buffer);
        }
    }

    fclose(file);
    return count;
}

//----------------------------- LOGIN ------------------------------//

bool VerifyLogin(const ACCOUNT accounts[], int accounts_count, const char *username, const char *password, ROLE *out_role) {
    for (int i = 0; i < accounts_count; i++) {
        if (strcmp(username, accounts[i].Username) == 0) {
            if (strcmp(password, accounts[i].Password) == 0) {
                if (out_role) *out_role = accounts[i].role;
                return true;
            }
            return false;  // username found but wrong password
        }
    }
    return false;  // username not found
}

//----------------------------- SIGNUP ------------------------------//

bool SignUp(ACCOUNT accounts[], int *accounts_count, const char *username, const char *password) {
    if (*accounts_count >= MAX_NUM_ACC) {
        printf("Reached maximum number of accounts\n");
        PauseScreen(1000);
        return false;
    }

    // check if username already exists
    for (int i = 0; i < *accounts_count; i++) {
        if (strcmp(accounts[i].Username, username) == 0) {
            printf("Username already taken\n");
            PauseScreen(1000);
            return false;
        }
    }

    // add new account
    int index = *accounts_count;
    strcpy(accounts[index].Username, username);
    strcpy(accounts[index].Password, password);
    accounts[index].role = ROLE_USER; 

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        perror("Error opening file in append mode");
        PauseScreen(1000);
        return false;
    }

    fprintf(file, "%d:%s,%s,%d\n", index, username, password, (int)ROLE_USER);
    fclose(file);

    (*accounts_count)++;
    printf("Account Created Successfully.\n");
    PauseScreen(1000);
    return true;
}

//----------------------------- FIRST MENU ------------------------------//

void FirstDisplay(int *choice) {
    printf("1. LogIn\n");
    printf("2. SignUp\n");
    printf("3. Exit\n");
    printf("Pick an option: ");
    scanf("%d", choice);
}

//----------------------------- MAIN MENU ------------------------------//

void MainMenu(const char *loggedInUsername, ROLE role, int *choice) {
    printf("Welcome %s!\n\n", loggedInUsername);
    if (role == ROLE_ADMIN) {
        printf("1. Admin Panel\n");
        printf("2. Start Server Chat\n");
        printf("3. Start Client Chat\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
    } else {
        printf("1. Start Server Chat\n");
        printf("2. Start Client Chat\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
    }
    scanf("%d", choice);
}
