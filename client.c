#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#include "client.h"
#include "utils.h"

void start_client_chat(const char *username) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];


    ClearScreen();
    printf("Enter IP: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize\n");
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET) {
        printf("Could not create socket\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(buffer);
    server_addr.sin_port = htons(PORT);

    printf("Connecting to %s:%d...\n", buffer, PORT);

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        closesocket(sock);
        WSACleanup();
        return;
    }

    ClearScreen();
    printf("Connected to chat!\n");
    PauseScreen(1000);

    while(1) {
        printf("%s: ", username);
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if(strcmp(buffer, "exit") == 0) {
            printf("Closing connection...\n");
            break;
        }

        send(sock, buffer, (int)strlen(buffer), 0);

        int recv_len = recv(sock, buffer, BUF_SIZE - 1, 0);
        if(recv_len <= 0) {
            printf("Connection close by server.\n");
            break;
        }

        buffer[recv_len] = '\0';
        printf("Server: %s\n", buffer);
    }

    closesocket(sock);
    WSACleanup();
    return;
}