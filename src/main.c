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

                char *username = get_username_input("Username (or type 'exit' to cancel): ");
                if (!username) break;

                char *password = get_password_input("Password (or type 'exit' to cancel): ");
                if (!password) {
                    free(username);
                    break;
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
                            AdminMenuLoop(account, &Acc_count);
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

                char *username = get_username_input("Create username (or type 'exit' to cancel): ");
                if (!username) break;

                if (!ValidUsername(username, account, Acc_count)) {
                    printf("Username already taken\n");
                    free(username);
                    PauseScreen(1000);
                    break;
                }

                char *password = get_password_input("Create password (or type 'exit' to cancel): ");
                if (!password) {
                    free(username);
                    break;
                }

                if (!ValidatePass(password)) {
                    printf("Password not strong enough\n");
                    free(username);
                    free(password);
                    PauseScreen(1000);
                    break;
                }

                if (SignUp(account, &Acc_count, username, password)) {
                    if (save_accounts_to_file(FILE_NAME, account, Acc_count) == 0) {
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
                break;
        }
    }

    return 0;
}
