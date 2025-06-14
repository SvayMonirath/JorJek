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
                char *username = get_input("Username: ", MAX_NAME_LENGTH);
                char *password = get_input("Password: ", MAX_PASS_LENGTH);

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

                        if (user_role == ROLE_ADMIN) {
                            switch (chat_choice) {
                                case 1: {
                                    int AdminChoice = 0;
                                    do {
                                        ClearScreen();
                                        AdminPanel(username_buffer, &AdminChoice);
                                        clear_input_buffer();
                                        switch (AdminChoice) {
                                            case 1:
                                                while(1) {
                                                    ViewUser(account, Acc_count);
                                                    char *input = get_input("Write (exit) to leave: ", 10);
                                                    if(strcmp(input, "exit") == 0) {
                                                        break;
                                                    }
                                                }
                                                break;
                                                
                                            case 2:
                                                //delete user
                                                if(DeleteUser(account, &Acc_count)) {
                                                    if(save_accounts_to_file(FILE_NAME, account, Acc_count)) printf("\nAccounted Deleted Successfully\n");
                                                    PauseScreen(2000); 
                                                }
                                                break;

                                            // case 3:
                                            //     //reset user password
                                            //     break;

                                            // case 4:
                                            //     //view all chat 
                                            //     break;
                                                
                                            case 5:
                                                ClearScreen();
                                                printf("Exiting Back to Main Menu...\n");
                                                PauseScreen(1000);
                                                break;  

                                            default:
                                                printf("Invalid choice, try again.\n");
                                                PauseScreen(1000);
                                                break;
                                        }



                                    } while(AdminChoice != 5);
                                }   
                                    break;
                                case 2:
                                    start_server_chat(username_buffer);
                                    break;
                                case 3:
                                    start_client_chat(username_buffer);
                                    break;
                                case 4:
                                    printf("Logging out...\n");
                                    PauseScreen(1000);
                                    break;
                                default:
                                    printf("Invalid choice, try again.\n");
                                    PauseScreen(1000);
                            }
                        } else {
                            switch (chat_choice) {
                                case 1:
                                    start_server_chat(username_buffer);
                                    break;
                                case 2:
                                    start_client_chat(username_buffer);
                                    break;
                                case 3:
                                    printf("Logging out...\n");
                                    PauseScreen(1000);
                                    break;
                                default:
                                    printf("Invalid choice, try again.\n");
                                    PauseScreen(1000);
                            }
                        }

                    } while ((user_role == ROLE_ADMIN && chat_choice != 4) || (user_role == ROLE_USER && chat_choice != 3));

                } else {
                    ClearScreen();
                    printf("Incorrect username or password.\n");
                    PauseScreen(1000);
                }
                break;
            }
            case SIGNUP: {
                ClearScreen();
                char *username = get_input("Create username: ", MAX_NAME_LENGTH);
                if (!ValidUsername(username, account, Acc_count)) {
                    printf("Username already taken\n");
                    free(username);
                    PauseScreen(1000);
                    break;
                }
                char *password = get_input("Create password: ", MAX_PASS_LENGTH);
                if (!ValidatePass(password)) {
                    printf("Password not strong enough\n");
                    free(username);
                    free(password);
                    PauseScreen(1000);
                    break;
                }
                if (SignUp(account, &Acc_count, username, password)) {
                    // success message already printed
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
                printf("Invalid option. Try again\n");
                PauseScreen(1000);
                break;
        }
    }

    return 0;
}

