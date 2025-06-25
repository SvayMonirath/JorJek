#include "LogSign.h"
#include "utils.h"
#include "server.h"
#include "client.h"
#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------- LOAD_ACCOUNT ------------------------------//

int load_accounts_from_file(const char *filename, ACCOUNT accounts[]) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;

        char *index_part = strtok(line, ":");
        char *rest = strtok(NULL, ":");
        if (!rest) continue;

        char *username = strtok(rest, ",");
        char *password = strtok(NULL, ",");
        char *role_str = strtok(NULL, ",");
        char *failed_str = strtok(NULL, ",");
        char *lockout_str = strtok(NULL, ",");

        if (!username || !password || !role_str || !failed_str || !lockout_str) continue;

        strcpy(accounts[count].Username, username);
        strcpy(accounts[count].Password, password);
        accounts[count].role = atoi(role_str);
        accounts[count].failed_attempts = atoi(failed_str);
        accounts[count].lockout_until = atol(lockout_str);

        count++;
        if (count >= MAX_NUM_ACC) break;
    }

    fclose(file);
    return count;
}

// Save accounts to file - implemented only here
int save_accounts_to_file(const char *filename, ACCOUNT accounts[], int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("failed to open file in write mode");
        return -1;
    }

    char datetime[20];  

    for (int i = 0; i < count; i++) {
        if (accounts[i].lockout_until == 0) {
            
            strcpy(datetime, "0000-00-00 00:00:00");
        } else {
            struct tm *lt = localtime(&accounts[i].lockout_until);
            strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", lt);
        }

        fprintf(file, "%d:%s,%s,%d,%d,%s\n", i,
            accounts[i].Username,
            accounts[i].Password,
            accounts[i].role,
            accounts[i].failed_attempts,
            datetime);
    }

    fclose(file);
    return 0;
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
    click_sound();
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
    click_sound();
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
