#include "admin.h"
#include "utils.h"
#include "LogSign.h"
#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------- ADMIN PANEL ------------------------------//
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
    clear_input_buffer();
}

//----------------------------- VIEW USER ------------------------------//
void ViewUser(ACCOUNT accounts[], int count) {
    ClearScreen();
    printf("============== USER LIST ==============\n\n");
    printf("ID:\tUsername\n\n");

    int display_id = 1;
    for (int i = 0; i < count; i++) {
        if (accounts[i].role != ROLE_ADMIN) {
            printf("%d:\t%s\n", display_id, accounts[i].Username);
            display_id++;
        }
    }
    if (display_id == 1) {
        printf("No regular users found.\n");
    }
    printf("\n------------------------------------\n");

    if (get_confirmation("Do you want to view passwords? (yes/no): ")) {
        int chance = 3;
        while (chance > 0) {
            int code;
            printf("\nEnter secret code: ");
            scanf("%d", &code);
            clear_input_buffer();

            if (code == SECRET_CODE) {
                ClearScreen();
                printf("============== USER LIST WITH PASSWORDS ==============\n\n");
                printf("ID:\tUsername\tPassword\n\n");

                int pass_display_id = 1;
                for (int i = 0; i < count; i++) {
                    if (accounts[i].role != ROLE_ADMIN) {
                        printf("%d:\t%s\t\t%s\n", pass_display_id, accounts[i].Username, accounts[i].Password);
                        pass_display_id++;
                    }
                }
                printf("\n------------------------------------\n");
                break;
            } else {
                chance--;
                if (chance > 0) {
                    printf("Incorrect code. %d %s left.\n", chance, (chance > 1) ? "tries" : "try");
                } else {
                    printf("No tries left. Access to passwords denied.\n");
                }
            }
        }
    } else {
        printf("\n\nPassword view skipped.\n");
    }
}

//----------------------------- DELETE USER ------------------------------//
bool DeleteUser(ACCOUNT accounts[], int *count) {
    ViewUser(accounts, *count);

    char *usernameToDelete = get_username_input("Enter account to delete (or type 'exit' to cancel): ");
    if (!usernameToDelete) {
        printf("Deletion cancelled.\n");
        return false;
    }

    int AccIndex = find_account_index(accounts, *count, usernameToDelete);

    if (AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToDelete);
        return false;
    }

    if (get_confirmation("Are you sure you want to delete this user? (yes/no): ")) {
        for (int i = AccIndex; i < *count - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        (*count)--;
        printf("User deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }

    free(usernameToDelete);
    return true;
}

//----------------------------- RESET PASSWORD ------------------------------//
bool ResetPass(ACCOUNT accounts[], int count) {
    ViewUser(accounts, count);

    char *usernameToReset = get_username_input("\nEnter account to reset password (or type 'exit' to cancel): ");
    if (!usernameToReset) {
        printf("Password reset cancelled.\n");
        return false;
    }

    int AccIndex = find_account_index(accounts, count, usernameToReset);

    if (AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToReset);
        return false;
    }

    while (1) {
        char *newPass = get_password_input("\nEnter new password (or type 'exit' to cancel): ");
        if (!newPass) {
            free(usernameToReset);
            printf("Password reset cancelled.\n");
            return false;
        }

        if (ValidatePass(newPass)) {
            strcpy(accounts[AccIndex].Password, newPass);
            free(newPass);
            free(usernameToReset);
            printf("Password reset successful.\n");
            return true;
        } else {
            printf("Password not strong enough, try again.\n");
            free(newPass);
        }
    }
}

//----------------------------- PROMOTE USER ------------------------------//
bool promote(ACCOUNT accounts[], int count) {
    ViewUser(accounts, count);

    char *usernameToPromote = get_username_input("\nEnter account to promote (or type 'exit' to cancel): ");
    if (!usernameToPromote) {
        printf("Promotion cancelled.\n");
        return false;
    }

    int AccIndex = find_account_index(accounts, count, usernameToPromote);

    if (AccIndex == -1) {
        printf("Account not found.\n");
        free(usernameToPromote);
        return false;
    }

    int chance = 3;
    while (chance > 0) {
        int code;
        printf("\nEnter Secret code: ");
        scanf("%d", &code);
        clear_input_buffer();

        if (code == SECRET_CODE) {
            accounts[AccIndex].role = ROLE_ADMIN;
            printf("%s has been promoted to ADMIN\n", accounts[AccIndex].Username);
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

//----------------------------- ADMIN MENU LOOP ------------------------------//
void AdminMenuLoop(ACCOUNT accounts[], int *accounts_count) {
    int AdminChoice = 0;
    do {
        ClearScreen();
        AdminPanel(&AdminChoice);

        switch (AdminChoice) {
            case 1: // View users
                while (1) {
                    ViewUser(accounts, *accounts_count);
                    char *input = get_input("Write (exit) to leave: ", 10);
                    if (_stricmp(input, "exit") == 0) {
                        free(input);
                        break;
                    }
                    free(input);
                }
                break;

            case 2: // Delete user
                if (DeleteUser(accounts, accounts_count)) {
                    if (save_accounts_to_file(FILE_NAME, accounts, *accounts_count) == 0) {
                        printf("\nAccount Deleted Successfully\n");
                    } else {
                        printf("\nError saving changes!\n");
                    }
                    PauseScreen(2000);
                }
                break;

            case 3: // Reset password
                if (ResetPass(accounts, *accounts_count)) {
                    if (save_accounts_to_file(FILE_NAME, accounts, *accounts_count) == 0) {
                        printf("Password Reset Successfully\n");
                    } else {
                        printf("\nError saving changes!\n");
                    }
                    PauseScreen(2000);
                }
                break;

            case 4: // Promote user
                if (promote(accounts, *accounts_count)) {
                    if (save_accounts_to_file(FILE_NAME, accounts, *accounts_count) == 0) {
                        printf("Change Saved Successfully\n");
                    } else {
                        printf("\nError saving changes!\n");
                    }
                    PauseScreen(2000);
                }
                break;

            case 5: // Exit admin panel
                ClearScreen();
                printf("Exiting Back to Main Menu...\n");
                PauseScreen(1000);
                break;

            default:
                printf("Invalid choice, try again.\n");
                PauseScreen(1000);
                break;
        }
    } while (AdminChoice != 5);
}

//----------------------------- ENSURE DEFAULT ADMIN ------------------------------//
void EnsureDefaultAdminExists(ACCOUNT accounts[], int *count) {
    bool admin_exists = false;
    for (int i = 0; i < *count; i++) {
        if (accounts[i].role == ROLE_ADMIN) {
            admin_exists = true;
            break;
        }
    }

    if (!admin_exists) {
        printf("No admin found! Creating default admin account.\n");

        ACCOUNT admin_account;
        strcpy(admin_account.Username, "Monirath");
        strcpy(admin_account.Password, "AdminRath1");
        admin_account.role = ROLE_ADMIN;

        if (*count < MAX_NUM_ACC) {
            accounts[(*count)++] = admin_account;
            if (save_accounts_to_file(FILE_NAME, accounts, *count) == 0) {
                printf("Default admin account created\n");
            } else {
                printf("Failed to save default admin account!\n");
            }
        } else {
            printf("Max account limit reached! Cannot create default admin.\n");
        }

        printf("Press Enter to continue...");
        getchar();
    }
}