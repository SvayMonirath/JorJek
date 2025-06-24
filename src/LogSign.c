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
    FILE *file = fopen(filename, "r");  // text mode
    if (!file) {
        perror("Could not open file");
        return 0;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;  // remove newline

        // Format: "index:username,password,role"
        char *index_part = strtok(line, ":");
        char *rest = strtok(NULL, ":");
        if (!rest) continue;

        char *username = strtok(rest, ",");
        char *password = strtok(NULL, ",");
        char *role_str = strtok(NULL, ",");

        if (!username || !password || !role_str) continue;

        strcpy(accounts[count].Username, username);
        strcpy(accounts[count].Password, password);
        accounts[count].role = atoi(role_str);

        count++;
        if (count >= MAX_NUM_ACC) break;
    }

    fclose(file);
    return count;
}


//----------------------------- LOGIN ------------------------------//

// check username and password to see if can login or nah
bool VerifyLogin(const ACCOUNT accounts[], int accounts_count, const char *username, const char *password, ROLE *out_role) {
    int index = find_account_index(accounts, accounts_count, username);
    if (index == -1) return false; // username not found

    if (strcmp(password, accounts[index].Password) == 0) {
        if (out_role) *out_role = accounts[index].role;
        return true;  // login successful
    }
    return false; // password wrong
}

//----------------------------- SIGNUP ------------------------------//

// this one for creating new account 
bool SignUp(ACCOUNT accounts[], int *accounts_count, const char *username, const char *password) {
    if (*accounts_count >= MAX_NUM_ACC) {
        printf("Already too many accounts\n");
        PauseScreen(1000);
        return false;
    }

    if (find_account_index(accounts, *accounts_count, username) != -1) {
        printf("Username already taken\n");
        PauseScreen(1000);
        return false;
    }

    int index = *accounts_count;
    strcpy(accounts[index].Username, username);
    strcpy(accounts[index].Password, password);
    accounts[index].role = ROLE_USER;

    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        perror("Failed to open file in append\n");
        PauseScreen(1000);
        return false;
    }

    fprintf(file, "%d:%s,%s,%d\n", index, username, password, (int)ROLE_USER);
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
    if(role == ROLE_ADMIN) {
        switch(chat_choice) {
            case 2: // admin menu option 2
                start_server_chat(username);
                break;
            case 3: // admin menu option 3
                start_client_chat(username);
                break;
            case 4: // logout
                printf("Logging out...\n");
                PauseScreen(1000);
                break;
            default:
                printf("Invalid choice, try again.\n");
                PauseScreen(1000);
                break;
        }
    } else {
        switch(chat_choice) {
            case CHAT_SERVER:
                start_server_chat(username);
                break;
            case CHAT_CLIENT:
                start_client_chat(username);
                break;
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
}
