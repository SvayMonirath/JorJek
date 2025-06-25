#include "server.h"
#include "utils.h"
#include "chatlog.h"
#include "sound.h"

#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define BUF_SIZE 512
#define PORT 12345
#define SCREEN_WIDTH 80

void start_server_chat(const char *username) {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    char input_buffer[BUF_SIZE];
    char timestamp[MAX_FULL_TIMESTAMP_LEN];  // use new full timestamp length

    ClearScreen();
    printf("========================================\n");
    printf(" Hello %s! Starting Server Chat...\n", username);
    printf("========================================\n\n");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize: %d\n", WSAGetLastError());
        return;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    if (listen(server_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    ClearScreen();
    printf("========================================\n");
    printf(" Server ready. Waiting for client on port %d...\n", PORT);
    printf("========================================\n\n");

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    // Send server username to client
    send(client_socket, username, (int)strlen(username), 0);

    // Receive client username
    int name_len = recv(client_socket, buffer, BUF_SIZE - 1, 0);
    if (name_len <= 0) {
        printf("Failed to receive username from client.\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
    }
    buffer[name_len] = '\0';
    char client_username[BUF_SIZE];
    strcpy(client_username, buffer);

    // Generate chat filename
    char chat_filename[256];
    get_chat_filename(username, client_username, chat_filename, sizeof(chat_filename));

    // Load and display chat history
    CHAT_MESSAGE messages[1000];
    int msg_count = load_chat_messages(chat_filename, messages, 1000);

    ClearScreen();
    printf("\n--- START CHAT SESSION ---\n\n");
    print_chat_messages(messages, msg_count, username);

    while (1) {
        // Receive message from client
        int recv_len = recv(client_socket, buffer, BUF_SIZE - 1, 0);
        if (recv_len <= 0) {
            printf("Connection closed by client\n");
            break;
        }
        buffer[recv_len] = '\0';

        // Play notification sound on message receive
        chat_sound();

        // Format timestamp for received message
        format_timestamp(timestamp, sizeof(timestamp));
        printf("%s  [%s]: %s\n", client_username, timestamp + 11, buffer); // also print here for realtime feedback

        // Save received message
        CHAT_MESSAGE rcv_msg;
        strcpy(rcv_msg.sender, client_username);
        strcpy(rcv_msg.full_timestamp, timestamp);
        strcpy(rcv_msg.message, buffer);
        save_chat_message(username, client_username, &rcv_msg);

        // Prompt server input (right aligned)
        format_timestamp(timestamp, sizeof(timestamp));
        printf("\n");

        int padding = SCREEN_WIDTH - (int)strlen(username) - 3;
        if (padding < 0) padding = 0;

        printf("%*s > ", padding + (int)strlen(username), username);
        fflush(stdout);

        if (!fgets(input_buffer, BUF_SIZE, stdin)) break;
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        if (strcmp(input_buffer, "exit") == 0) {
            ClearScreen();
            printf("Closing chat...\n");
            PauseScreen(1000);
            break;
        }

        // Send server message to client
        send(client_socket, input_buffer, (int)strlen(input_buffer), 0);

        chat_sound();

        // Save sent message
        CHAT_MESSAGE sent_msg;
        strcpy(sent_msg.sender, username);
        strcpy(sent_msg.full_timestamp, timestamp);
        strcpy(sent_msg.message, input_buffer);
        save_chat_message(username, client_username, &sent_msg);

        // Print own message (right aligned)
        printf("%*s [%s]: %s\n\n", padding + (int)strlen(username), username, timestamp + 11, input_buffer);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}
