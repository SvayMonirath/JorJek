#include "admin.h"
#include "LogSign.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//----------------------------- ADMIN MENU ------------------------------//
void AdminPanel(int *choice)  {
    printf("============== ADMIN PANEL ==============\n\n");
    
    printf(" 1. View all users\n");
    printf(" 2. Delete a user\n");
    printf(" 3. Reset a user's password\n");
    printf(" 4. Promote to admin\n");
    printf(" 5. Leave admin mode\n");
    
    printf("\n------------------------------------\n");
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
    for(int i = 1; i < count; i++) {
        printf("%d:\t%s\n", i, account[i].Username);
    }
    printf("\n------------------------------------\n");
}

//----------------------------- DELETE USER ------------------------------//
bool DeleteUser(ACCOUNT account[], int *count) {
    ViewUser(account, *count);

    char *usernameToDelete = get_input("Enter account to delete (or type 'exit' to cancel): ", MAX_NAME_LENGTH);
    if (_stricmp(usernameToDelete, "exit") == 0) {
        free(usernameToDelete);
        printf("Deletion cancelled.\n");
        return false;
    }

    int AccIndex = -1;
    for (int i = 0; i < *count; i++) {
        if (strcmp(usernameToDelete, account[i].Username) == 0) {
            AccIndex = i;
            break;
        }
    }

    if (AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToDelete);
        return false;
    }

    char *confirmation = get_input("Are you sure? (yes/no): ", 10);
    if (strcmp(confirmation, "yes") == 0 || strcmp(confirmation, "Yes") == 0) {
        for (int i = AccIndex; i < *count - 1; i++) {
            account[i] = account[i + 1];
        }
        (*count)--;
        printf("User deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }

    free(usernameToDelete);
    free(confirmation);
    return true;
}

//----------------------------- RESET PASSWORD ------------------------------//
bool ResetPass(ACCOUNT account[], int count) {
    ViewUser(account, count);

    char *usernameToReset = get_input("\nEnter account to reset password (or type 'exit' to cancel): ", MAX_NAME_LENGTH);
    if (_stricmp(usernameToReset, "exit") == 0) {
        free(usernameToReset);
        printf("Password reset cancelled.\n");
        return false;
    }

    int AccIndex = -1;
    for(int i = 0; i < count; i++) {
        if (strcmp(usernameToReset, account[i].Username) == 0) {
            AccIndex = i;
            break;
        }
    }

    if(AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToReset);
        return false; 
    } 

    while(1) {
        char *ResetPass = get_input("\nEnter new password: ", MAX_PASS_LENGTH);
        if(ValidatePass(ResetPass)) {
            strcpy(account[AccIndex].Password, ResetPass);
            free(ResetPass);
            free(usernameToReset);
            return true;
        }
    }
}

//----------------------------- PROMOTE TO ADMIN ------------------------------//
bool promote(ACCOUNT account[], int count) {
    ViewUser(account, count);
    char *usernameToPromote = get_input("\nEnter account to promote (or type 'exit' to cancel): ", MAX_NAME_LENGTH);
    if (_stricmp(usernameToPromote, "exit") == 0) {
        free(usernameToPromote);
        printf("Promotion cancelled.\n");
        return false;
    }

    int AccIndex = -1;
    for(int i = 0; i < count; i++) {
        if (strcmp(usernameToPromote, account[i].Username) == 0) {
            AccIndex = i;
            break;
        }
    }

    if(AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToPromote);
        return false; 
    } 

    int chance = 3;
    while (chance != 0) {
        int code;
        printf("\nEnter Secret code: ");
        scanf("%d", &code);

        if (code == SECRET_CODE) {
            account[AccIndex].role = ROLE_ADMIN;
            printf("%s has been promoted to ADMIN\n", account[AccIndex].Username);
            free(usernameToPromote);
            return true;
        } else {
            chance--;
            if (chance > 0) {
                printf("Invalid code, %d %s left\n", chance, (chance > 1) ? "tries" : "try");
            } else {
                printf("No tries left. Promotion failed.\n");
            }
        }
    }

    free(usernameToPromote);
    return false;
}
