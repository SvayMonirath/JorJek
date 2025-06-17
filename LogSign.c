#include "LogSign.h"
#include "utils.h"
#include "server.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------- LOAD_ACCOUNT ------------------------------//

// this one just get account info from file and put into struct
int load_accounts_from_file(const char *filename, ACCOUNT accounts[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("could not open file man");
        return -1;
    }

    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        int account_id;
        char username[MAX_NAME_LENGTH];
        char password[MAX_PASS_LENGTH];
        int role_int;

        // the format (id:username,password,role)
        if (sscanf(buffer, "%d:%29[^,],%19[^,],%d", &account_id, username, password, &role_int) == 4) {
            removeUnwantedSpace(username);
            removeUnwantedSpace(password);

            // ok now just copy to the account list
            strcpy(accounts[count].Username, username);
            strcpy(accounts[count].Password, password);
            accounts[count].role = (role_int == 1) ? ROLE_ADMIN : ROLE_USER;

            count++;
            if (count >= MAX_NUM_ACC) break; // nah bro we full now
        } else {
            printf("Warning: Skipping invalid line: %s", buffer);
        }
    }

    fclose(file);
    return count; // return how many accounts we got
}

//----------------------------- LOGIN ------------------------------//

// check username and password to see if can login or nah
bool VerifyLogin(const ACCOUNT accounts[], int accounts_count, const char *username, const char *password, ROLE *out_role) {
    for (int i = 0; i < accounts_count; i++) {
        if (strcmp(username, accounts[i].Username) == 0) {
            if (strcmp(password, accounts[i].Password) == 0) {
                if (out_role) *out_role = accounts[i].role; // tell them the role
                return true; // we good, can login 
            }
            return false; // username right but password wrong
        }
    }
    return false; // username not found at all bro
}

//----------------------------- SIGNUP ------------------------------//

// this one for creating new account 
bool SignUp(ACCOUNT accounts[], int *accounts_count, const char *username, const char *password) {
    if (*accounts_count >= MAX_NUM_ACC) {
        printf("Already too many accounts\n");
        PauseScreen(1000);
        return false;
    }

    // see if username already taken or not
    for (int i = 0; i < *accounts_count; i++) {
        if (strcmp(accounts[i].Username, username) == 0) {
            printf("Username already taken\n");
            PauseScreen(1000);
            return false;
        }
    }

    // ok username free, let's make new account now
    int index = *accounts_count;
    strcpy(accounts[index].Username, username);
    strcpy(accounts[index].Password, password);
    accounts[index].role = ROLE_USER; // new account alway user by default

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        perror("Failed to open file in append\n");
        PauseScreen(1000);
        return false;
    }

    fprintf(file, "%d:%s,%s,%d\n", index, username, password, (int)ROLE_USER); // write it down to file
    fclose(file);

    (*accounts_count)++;
    printf("Account created successfully!\n");
    PauseScreen(1000);
    return true;
}

//----------------------------- FIRST MENU ------------------------------//

void FirstDisplay(int *choice) {
    printf("================ JOR JEK ================\n\n");
    printf("1. Log in\n");
    printf("2. Sign up\n");
    printf("3. Exit\n");
    printf("\n---------------------------------------\n");
    printf("Enter an option: ");
    scanf("%d", choice);
}

//----------------------------- MAIN MENU ------------------------------//

// the menu after u login, where u go next
void MainMenu(const char *loggedInUsername, ROLE role, int *choice) {
    printf("====================================\n");
    printf("        Welcome, %s!\n", loggedInUsername);
    printf("====================================\n\n");

    if (role == ROLE_ADMIN) {
        printf(" 1. Admin Panel\n");
        printf(" 2. Start Server Chat\n");
        printf(" 3. Start Client Chat\n");
        printf(" 4. Logout\n");
    } else {
        printf(" 1. Start Server Chat\n");
        printf(" 2. Start Client Chat\n");
        printf(" 3. Logout\n");
    }

    printf("\n------------------------------------\n");
    printf("Enter an option (number): ");
    scanf("%d", choice);
}

//----------------------------- HANDLE MENU ------------------------------//
void handle_chat_menu(const char *username, int chat_choice, ROLE role) {
    switch (chat_choice) {
        case CHAT_SERVER:
            start_server_chat(username);
            break;

        case CHAT_CLIENT:
            start_client_chat(username);
            break;

        case LOGOUT_ADMIN:
        case LOGOUT_USER:
            printf("Logging out...\n");
            PauseScreen(1000);
            break;

        default:
            printf("Invalid choice, try again.\n");
            PauseScreen(1000);
            break;
    }
}
