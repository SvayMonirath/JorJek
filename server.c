#include "server.h"
#include "utils.h"

#include <stdio.h>
#include <winsock2.h>
#include <string.h>


void start_server_chat(const char *username) {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer [BUF_SIZE];
    

    ClearScreen();
    printf("Hello %s! Starting server chat...\n", username);

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to inialize: %d\n", WSAGetLastError());
        return;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == INVALID_SOCKET) {
        printf("Could not create socket\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        WSACleanup();
        return;
    }

    if(listen(server_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    ClearScreen();
    printf("Server ready. Waiting for client to connect on port %d...\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if(client_socket == INVALID_SOCKET) {
        printf("Accept failed\n");
        WSACleanup();
        return;
    }

    // Send server username
    send(client_socket, username, (int)strlen(username), 0);

    // Receive client username
    char client_username[BUF_SIZE];
    int name_len = recv(client_socket, client_username, BUF_SIZE - 1, 0);
    if (name_len <= 0) {
        printf("Failed to receive username from client.\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
    }
    client_username[name_len] = '\0';

    ClearScreen();
    printf("%s connected! Start Chatting\n", client_username);
    PauseScreen(1000);

    while(1) {
        int recv_len = recv(client_socket, buffer, BUF_SIZE-1, 0);
        if(recv_len <= 0) {
            printf("Connection closed by client\n");
            break;
        } 

        buffer[recv_len] = '\0';
        printf("%s: %s\n", client_username, buffer);

        printf("%s: ", username);
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if(strcmp(buffer, "exit") == 0) {
            ClearScreen();
            printf("Closing chat...\n");
            PauseScreen(1000);
            break;
        }

        send(client_socket, buffer, (int)strlen(buffer), 0);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return;
}