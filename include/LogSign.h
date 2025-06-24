#ifndef LOGSIGN_H
#define LOGSIGN_H

#include <stdbool.h>

//----------------------------- DEFINES ------------------------------//
#define MAX_PASS_LENGTH 20
#define MAX_NAME_LENGTH 30
#define MAX_NUM_ACC 20
#define FILE_NAME "data/user.csv"
#define CHAT_FILE_NAME "data/chat.csv"

#define CHAT_SERVER 1
#define CHAT_CLIENT 2
#define LOGOUT_ADMIN 4
#define LOGOUT_USER 3

//----------------------------- ENUM ------------------------------//
typedef enum {
    ROLE_USER = 0,
    ROLE_ADMIN = 1
} ROLE;

typedef enum {
    LOGIN = 1, SIGNUP, EXIT
} LogSign;

//----------------------------- STRUCT ------------------------------//
typedef struct {
    char Username[MAX_NAME_LENGTH];
    char Password[MAX_PASS_LENGTH];
    ROLE role;
} ACCOUNT;

//----------------------------- FUNCTION PROTOTYPES ------------------------------//
int load_accounts_from_file(const char *filename, ACCOUNT accounts[]);
bool VerifyLogin(const ACCOUNT accounts[], int accounts_count, const char *username, const char *password, ROLE *out_role);
bool SignUp(ACCOUNT accounts[], int *accounts_count, const char *username, const char *password);
void FirstDisplay(int *choice);
void MainMenu(const char *loggedInUsername, ROLE role, int *choice);
void handle_chat_menu(const char *username, int chat_choice, ROLE role);

#endif
