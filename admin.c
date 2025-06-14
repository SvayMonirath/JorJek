#include "admin.h"
#include "LogSign.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void AdminPanel(const char *loggedInUsername, int *choice)  {
    printf("Welcome %s!\n\n", loggedInUsername);
    printf("1. View all users\n");
    printf("2. Delete a user\n");
    printf("3. Reset a user's password\n");
    printf("4. View all chat logs\n");
    printf("5. Exit admin mode\n");
    printf("Enter choice: ");
    scanf("%d", choice);
}


int save_accounts_to_file(const char *filename, ACCOUNT account[], int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d:%s,%s,%d\n", i, account[i].Username, account[i].Password, (int)account[i].role);
    }

    fclose(file);
    return 0;
}


void ViewUser(ACCOUNT account[], int count) {

    ClearScreen();

        printf("ID:\tUsername\n\n");
        for(int i=1; i < count; i++) {
            printf("%d:\t%s\n",i, account[i].Username);
        }

}

int DeleteUser(ACCOUNT account[], int *count) {
    ViewUser(account, *count);

    char *usernameToDelete = get_input("Enter Account to delete: ", MAX_NAME_LENGTH);
    int AccIndex = -1;

    // Find the user index
    for (int i = 0; i < *count; i++) {
        if (strcmp(usernameToDelete, account[i].Username) == 0) {
            AccIndex = i;
            break;
        }
    }

    if (AccIndex == -1) {
        printf("User not found.\n");
        free(usernameToDelete);
        return 0;  // no deletion
    }

    char *confirmation = get_input("Are you sure you want to delete this user? (yes/no): ", 10);
    if (strcmp(confirmation, "yes") == 0 || strcmp(confirmation, "Yes") == 0) {
        // Shift accounts left to overwrite deleted user
        for (int i = AccIndex; i < *count - 1; i++) {
            account[i] = account[i + 1];  // struct assignment (deep copy)
        }
        (*count)--; // decrement count
        printf("User deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }

    free(usernameToDelete);
    free(confirmation);
    return 1;
}


