#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "utils.h"
#include "LogSign.h"
#include "server.h" 
#include "client.h" 


int main(void) {
    ACCOUNT account[MAX_NUM_ACC];
    int Acc_count = load_accounts_from_file(FILE_NAME, account);
    if (Acc_count < 0) Acc_count = 0;

    int choice_one = 0;
    char *loggedInUsername = NULL;

    cool_intro();

    while (choice_one != EXIT) {
        ClearScreen();
        FirstDisplay(&choice_one);
        clear_input_buffer();

        switch (choice_one) {
            case LOGIN:
                if (LogIn(account, Acc_count, &loggedInUsername)) {
                    int chat_choice = 0;
                    do {
                        ClearScreen();
                        printf("Welcome %s! Choose mode:\n", loggedInUsername);
                        printf("1. Start Server Chat\n");
                        printf("2. Start Client Chat\n");
                        printf("0. Logout\n");
                        printf("Enter choice: ");
                        scanf("%d", &chat_choice);
                        clear_input_buffer();

                        switch (chat_choice) {
                            case 1:
                                start_server_chat(loggedInUsername);
                                break;
                            case 2:
                                start_client_chat(loggedInUsername);
                                break;
                            case 0:
                                printf("Logging out...\n");
                                PauseScreen(1000);
                                break;
                            default:
                                printf("Invalid choice, try again.\n");
                                PauseScreen(1000);
                        }
                    } while (chat_choice != 0);
                }
                break;
            case SIGNUP:
                SignUp(account, &Acc_count);
                break;
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
