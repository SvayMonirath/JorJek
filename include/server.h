#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock2.h>
#include "chatlog.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUF_SIZE 512

void start_server_chat(const char *username);


#endif