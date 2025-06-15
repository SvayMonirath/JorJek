#ifndef ADMIN_H
#define ADMIN_H


#include <stdbool.h>
#include "LogSign.h"


int save_accounts_to_file(const char *filename, ACCOUNT account[], int count);
void AdminPanel(const char *loggedInUsername, int *choice);
void ViewUser(ACCOUNT account[], int count);
int DeleteUser(ACCOUNT account[], int *count);
int ResetPass(ACCOUNT account[], int count);

#endif