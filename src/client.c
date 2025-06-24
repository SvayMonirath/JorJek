#include "client.h"
#include "utils.h"
#include "chatlog.h"

#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define BUF_SIZE 512
#define PORT 12345
#define SCREEN_WIDTH 80

void start_client_chat(const char *username) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char input_buffer[BUF_SIZE];
    char timestamp[MAX_FULL_TIMESTAMP_LEN];

    ClearScreen();
    printf("Enter IP: ");
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize\n");
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(buffer);
    server_addr.sin_port = htons(PORT);

    printf("Connecting to %s:%d...\n", buffer, PORT);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Send client username
    send(sock, username, (int)strlen(username), 0);

    // Receive server username
    int name_len = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (name_len <= 0) {
        printf("Failed to receive server username.\n");
        closesocket(sock);
        WSACleanup();
        return;
    }
    buffer[name_len] = '\0';
    char server_username[BUF_SIZE];
    strcpy(server_username, buffer);

    // Generate chat filename for this pair of users
    char chat_filename[256];
    get_chat_filename(username, server_username, chat_filename, sizeof(chat_filename));

    // Load and display chat history before starting chat
    CHAT_MESSAGE messages[1000];
    int msg_count = load_chat_messages(chat_filename, messages, 1000);

    ClearScreen();
    printf("\n--- START CHAT SESSION ---\n\n");
    print_chat_messages(messages, msg_count, username);

    while (1) {
        // Prompt user input (right aligned)
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
            printf("Closing connection...\n");
            PauseScreen(1000);
            break;
        }

        // Send message
        send(sock, input_buffer, (int)strlen(input_buffer), 0);

        // Save sent message
        CHAT_MESSAGE sent_msg;
        strcpy(sent_msg.sender, username);
        strcpy(sent_msg.full_timestamp, timestamp);
        strcpy(sent_msg.message, input_buffer);
        save_chat_message(username, server_username, &sent_msg);

        // Print own message (right aligned)
        printf("%*s [%s]: %s\n\n", padding + (int)strlen(username), username, timestamp + 11, input_buffer);

        // Receive server message (left aligned)
        int recv_len = recv(sock, buffer, BUF_SIZE - 1, 0);
        if (recv_len <= 0) {
            printf("Connection closed by server.\n");
            break;
        }
        buffer[recv_len] = '\0';

        format_timestamp(timestamp, sizeof(timestamp));
        printf("%s  [%s]: %s\n\n", server_username, timestamp + 11, buffer);

        // Save received message
        CHAT_MESSAGE rcv_msg;
        strcpy(rcv_msg.sender, server_username);
        strcpy(rcv_msg.full_timestamp, timestamp);
        strcpy(rcv_msg.message, buffer);
        save_chat_message(username, server_username, &rcv_msg);
    }

    closesocket(sock);
    WSACleanup();
}
