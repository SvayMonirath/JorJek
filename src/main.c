#include "utils.h"
#include "LogSign.h"
#include "server.h"
#include "client.h"
#include "admin.h"

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

int main(void) {
    ACCOUNT account[MAX_NUM_ACC];
    int Acc_count = load_accounts_from_file(FILE_NAME, account);
    if (Acc_count < 0) Acc_count = 0;

    int choice_one = 0;
    char username_buffer[MAX_NAME_LENGTH];
    ROLE user_role = ROLE_USER;

    while (choice_one != EXIT) {
        ClearScreen();
        FirstDisplay(&choice_one);
        clear_input_buffer();

        switch (choice_one) {
            case LOGIN: {
                ClearScreen();
                printf("================ LOG IN =================\n\n");

                char *username = get_input("Username (or type 'exit' to cancel): ", MAX_NAME_LENGTH);
                if (_stricmp(username, "exit") == 0) {
                    free(username);
                    break; // go back to main menu loop
                }
                char *password = get_input("Password (or type 'exit' to cancel): ", MAX_PASS_LENGTH);
                if (_stricmp(username, "exit") == 0) {
                    free(username);
                    break; // go back to main menu loop
                }

                bool success = VerifyLogin(account, Acc_count, username, password, &user_role);

                if (success) {
                    strcpy(username_buffer, username);
                    free(username);
                    free(password);

                    int chat_choice = 0;

                    do {
                        ClearScreen();
                        MainMenu(username_buffer, user_role, &chat_choice);
                        clear_input_buffer();

                        if (user_role == ROLE_ADMIN && chat_choice == 1) {
                            // Admin Panel
                            int AdminChoice = 0;
                            do {
                                ClearScreen();
                                AdminPanel(&AdminChoice);
                                clear_input_buffer();

                                switch (AdminChoice) {
                                    case 1: { // View users
                                        while (1) {
                                            ViewUser(account, Acc_count);
                                            char *input = get_input("Write (exit) to leave: ", 10);
                                            if (_stricmp(input, "exit") == 0) {
                                                free(input);
                                                break;
                                            }
                                            free(input);
                                        }
                                        break;
                                    }
                                    case 2: // Delete user
                                        if (DeleteUser(account, &Acc_count)) {
                                            if (save_accounts_to_file(FILE_NAME, account, Acc_count)) {
                                                printf("\nAccount Deleted Successfully\n");
                                            } else {
                                                printf("\nError saving changes!\n");
                                            }
                                            PauseScreen(2000);
                                        }
                                        break;

                                    case 3: // Reset password
                                        if (ResetPass(account, Acc_count)) {
                                            if (save_accounts_to_file(FILE_NAME, account, Acc_count)) {
                                                printf("Password Reset Successfully\n");
                                            } else {
                                                printf("\nError saving changes!\n");
                                            }
                                            PauseScreen(2000);
                                        }
                                        break;

                                    case 4: // Promote user
                                        if (promote(account, Acc_count)) {
                                            if (save_accounts_to_file(FILE_NAME, account, Acc_count)) {
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
                                }
                            } while (AdminChoice != 5);
                        } else {
                            handle_chat_menu(username_buffer, chat_choice, user_role);
                        }

                    } while ((user_role == ROLE_ADMIN && chat_choice != LOGOUT_ADMIN) ||
                             (user_role == ROLE_USER && chat_choice != LOGOUT_USER));

                } else {
                    ClearScreen();
                    printf("Incorrect username or password.\n");
                    free(username);
                    free(password);
                    PauseScreen(1000);
                }
                break;
            }

            case SIGNUP: {
                ClearScreen();
                printf("=============== SIGN UP =================\n\n");

                char *username = get_input("Create username (or type 'exit' to cancel): ", MAX_NAME_LENGTH);

                if (!ValidUsername(username, account, Acc_count)) {
                    printf("Username already taken\n");
                    free(username);
                    PauseScreen(1000);
                    break;
                }

                if (_stricmp(username, "exit") == 0) {
                    free(username);
                    break; // go back to main menu loop
                }

                char *password = get_input("Create password (or type 'exit' to cancel): ", MAX_PASS_LENGTH);

                if (_stricmp(password, "exit") == 0) {
                    free(username);
                    free(password);
                    break; // go back to main menu loop
                }

                if (!ValidatePass(password)) {
                    printf("Password not strong enough\n");
                    free(username);
                    free(password);
                    PauseScreen(1000);
                    break;
                }

                if (SignUp(account, &Acc_count, username, password)) {
                    if (save_accounts_to_file(FILE_NAME, account, Acc_count)) {
                        printf("\nYou can now login with your new account!\n");
                    } else {
                        printf("\nError saving new account!\n");
                    }
                    PauseScreen(1500);
                }

                free(username);
                free(password);
                break;
            }

            case EXIT:
                ClearScreen();
                printf("Exiting program...\n");
                PauseScreen(1000);
                break;

            default:
                ClearScreen();
                printf("Invalid option. Try again.\n");
                PauseScreen(1000);
        }
    }

    return 0;
}