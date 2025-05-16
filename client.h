#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUF_SIZE 512

void start_client_chat(const char *username);

#endif