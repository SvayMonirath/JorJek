#ifndef ADMIN_H
#define ADMIN_H


#include <stdbool.h>
#include "LogSign.h"

#define SECRET_CODE 314159

void AdminMenuLoop(ACCOUNT accounts[], int *accounts_count);
void AdminPanel(int *choice);
void ViewUser(ACCOUNT account[], int count);
bool DeleteUser(ACCOUNT account[], int *count);
bool ResetPass(ACCOUNT account[], int count);
bool promote(ACCOUNT account[], int count);

#endif