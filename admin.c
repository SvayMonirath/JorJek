#include "admin.h"
#include "LogSign.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//----------------------------- ADMIN MENU ------------------------------//
void AdminPanel(const char *loggedInUsername, int *choice)  {
    printf("Welcome, %s!\n\n", loggedInUsername);
    printf("1. View all users\n");
    printf("2. Delete a user\n");
    printf("3. Reset a user's password\n");
    printf("4. leave admin mode\n");
    printf("Enter an option: ");
    scanf("%d", choice);
}

//----------------------------- SAVE TO FILE ------------------------------//
int save_accounts_to_file(const char *filename, ACCOUNT account[], int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("failed to open file in write mode\n");
        return -1;
    }

    // write each account line by line
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d:%s,%s,%d\n", i, account[i].Username, account[i].Password, (int)account[i].role);
    }

    fclose(file);
    return 0;
}

//----------------------------- VIEW ALL USER ------------------------------//
void ViewUser(ACCOUNT account[], int count) {

    ClearScreen();

    printf("ID:\tUsername\n\n");
    for(int i = 1; i < count; i++) { // start at 1, idk maybe first one is special?
        printf("%d:\t%s\n", i, account[i].Username);
    }
}

//----------------------------- DELETE USER ------------------------------//
int DeleteUser(ACCOUNT account[], int *count) {
    ViewUser(account, *count); // show users first

    char *usernameToDelete = get_input("Enter account to delete: ", MAX_NAME_LENGTH);
    int AccIndex = -1;

    // find the user to delete
    for (int i = 0; i < *count; i++) {
        if (strcmp(usernameToDelete, account[i].Username) == 0) {
            AccIndex = i;
            break;
        }
    }

    if (AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToDelete);
        return 0; // no deletion
    }

    char *confirmation = get_input("Are you sure? (yes/no): ", 10);
    if (strcmp(confirmation, "yes") == 0 || strcmp(confirmation, "Yes") == 0) {
        // shift everything left, like erasing the user
        for (int i = AccIndex; i < *count - 1; i++) {
            account[i] = account[i + 1]; // move each account to left
        }
        (*count)--; // less user now
        printf("User deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }

    free(usernameToDelete);
    free(confirmation);
    return 1;
}

//----------------------------- RESET PASSWORD ------------------------------//
int ResetPass(ACCOUNT account[], int count) {

    ViewUser(account, count); // show the list
    char *usernameToReset = get_input("\nEnter account to reset password: ", MAX_NAME_LENGTH);
    int AccIndex = -1;

    // find the user
    for(int i = 0; i < count; i++) {
        if (strcmp(usernameToReset, account[i].Username) == 0) {
            AccIndex = i;
            break;
        }
    }

    if(AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToReset);
        return 0; 
    } 

    while(1) { // keep asking until they give valid password
        char *ResetPass = get_input("\nEnter new password: ", MAX_PASS_LENGTH);
        if(ValidatePass(ResetPass)) {
            strcpy(account[AccIndex].Password, ResetPass); // ok password changed
            free(ResetPass);
            free(usernameToReset);
            return 1;
        }
    }
}
