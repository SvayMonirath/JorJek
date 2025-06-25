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
            
                int index = find_account_index(account, Acc_count, username);
                if (index == -1) {
                    printf("Account not found.\n");
                    free(username);
                    PauseScreen(1000);
                    break;
                }
            
                time_t now = time(NULL);
                if (account[index].lockout_until > now) {
                    printf("\n====================================\n");
                    printf("\tACCOUNT IS LOCKED UNTIL\n\n\t%s", ctime(&account[index].lockout_until));
                    printf("=====================================\n");
                    wait_for_exit_prompt("\n\nType 'exit' to go back: ");
                    free(username);
                    break;
                }
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

                    account[index].failed_attempts = 0;
                    account[index].lockout_until = 0;
                    save_accounts_to_file(FILE_NAME, account, Acc_count);
            
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
                    printf("Incorrect password.\n");
                    account[index].failed_attempts++;
                    
                    if (account[index].failed_attempts >= 3) {
                        account[index].lockout_until = now + 30;
                        printf("\n\nToo many failed attempts. Account locked for 30 seconds.\n");
                    }
            
                    save_accounts_to_file(FILE_NAME, account, Acc_count);
                    free(username);
                    free(password);
                    PauseScreen(2000);
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
