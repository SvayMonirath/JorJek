#ifndef LOGSIGN_H
#define LOGSIGN_H

#include <stdbool.h>

//----------------------------- DEFINES ------------------------------//
#define MAX_PASS_LENGTH 20
#define MAX_NAME_LENGTH 30
#define MAX_NUM_ACC 20
#define FILE_NAME "user.csv"
#define CHAT_FILE_NAME "chat.csv"


//----------------------------- ENUM ------------------------------//
typedef enum {
    LOGIN = 1, SIGNUP, EXIT
} LogSign;


//----------------------------- STRUCT ------------------------------//
typedef struct {
    char Username[MAX_NAME_LENGTH];
    char Password[MAX_PASS_LENGTH];
} ACCOUNT;


int load_accounts_from_file(const char *filename, ACCOUNT accounts[]);
bool LogIn(ACCOUNT account[], int accounts_count, char **loggedInUsername);
void SignUp(ACCOUNT account[], int *accounts_count);
void FirstDisplay(int *choice);
void MainMenu(char *loggedInUsername, int *choice);

#endif