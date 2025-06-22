#include "chatlog.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h> 

void get_chat_filename(const char *u1, const char *u2, char *out_filename, size_t max_len) {

    if (strcmp(u1, u2) > 0) {
        const char *tmp = u1;
        u1 = u2;
        u2 = tmp;
    }
    snprintf(out_filename, max_len, "chat_%s_%s.csv", u1, u2);
}

bool save_chat_message(const char *user1, const char *user2, CHAT_MESSAGE *msg) {
    char filename[256];
    get_chat_filename(user1, user2, filename, sizeof(filename));

    FILE *f = fopen(filename, "a");
    if (!f) return false;

    fprintf(f, "\"%s\",\"%s\",\"%s\"\n", msg->sender, msg->timestamp, msg->message);
    fclose(f);
    return true;
}

int load_chat_messages(const char *filename, CHAT_MESSAGE messages[], int max_messages) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    int count = 0;
    char line[1024];
    while (fgets(line, sizeof(line), f) && count < max_messages) {
        char sender[MAX_SENDER_LEN], timestamp[MAX_TIMESTAMP_LEN], message[MAX_MSG_LEN];
        if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", sender, timestamp, message) == 3) {
            strcpy(messages[count].sender, sender);
            strcpy(messages[count].timestamp, timestamp);
            strcpy(messages[count].message, message);
            count++;
        }
    }
    fclose(f);
    return count;
}

void print_chat_messages(CHAT_MESSAGE messages[], int count, const char *my_username) {
    for (int i = 0; i < count; i++) {
        int padding = 80 - (int)strlen(messages[i].sender) - 3;
        if (strcmp(messages[i].sender, my_username) == 0) {
            if (padding < 0) padding = 0;
            printf("%*s [%s]: %s\n", padding + (int)strlen(messages[i].sender), messages[i].sender, messages[i].timestamp, messages[i].message);
        } else {
            printf("%s  [%s]: %s\n", messages[i].sender, messages[i].timestamp, messages[i].message);
        }
    }
}
