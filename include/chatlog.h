#ifndef CHATLOG_H
#define CHATLOG_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_SENDER_LEN 30
#define MAX_FULL_TIMESTAMP_LEN 20  
#define MAX_MSG_LEN 512

typedef struct {
    char sender[MAX_SENDER_LEN];
    char full_timestamp[MAX_FULL_TIMESTAMP_LEN];
    char message[MAX_MSG_LEN];
} CHAT_MESSAGE;

void get_chat_filename(const char *user1, const char *user2, char *out_filename, size_t max_len);
bool save_chat_message(const char *user1, const char *user2, CHAT_MESSAGE *msg);
int load_chat_messages(const char *filename, CHAT_MESSAGE messages[], int max_messages);
void print_chat_messages(CHAT_MESSAGE messages[], int count, const char *my_username);

#endif // CHATLOG_H
