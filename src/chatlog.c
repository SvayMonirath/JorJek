#include "chatlog.h"
#include <stdio.h>
#include <string.h>

//----------------------------- CHAT LOG UTILITIES ------------------------------//

void get_chat_filename(const char *u1, const char *u2, char *out_filename, size_t max_len) {
    if (strcmp(u1, u2) > 0) {
        const char *tmp = u1;
        u1 = u2;
        u2 = tmp;
    }
    snprintf(out_filename, max_len, "data/chat_%s_%s.csv", u1, u2);
}

bool save_chat_message(const char *user1, const char *user2, CHAT_MESSAGE *msg) {
    char filename[256];
    get_chat_filename(user1, user2, filename, sizeof(filename));

    FILE *f = fopen(filename, "a");
    if (!f) return false;

    fprintf(f, "\"%s\",\"%s\",\"%s\"\n", msg->sender, msg->full_timestamp, msg->message);
    fclose(f);
    return true;
}

int load_chat_messages(const char *filename, CHAT_MESSAGE messages[], int max_messages) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    int count = 0;
    char line[1024];
    while (fgets(line, sizeof(line), f) && count < max_messages) {
        char sender[MAX_SENDER_LEN];
        char full_timestamp[MAX_FULL_TIMESTAMP_LEN];
        char message[MAX_MSG_LEN];

        if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", sender, full_timestamp, message) == 3) {
            strcpy(messages[count].sender, sender);
            strcpy(messages[count].full_timestamp, full_timestamp);
            strcpy(messages[count].message, message);
            count++;
        }
    }
    fclose(f);
    return count;
}

void print_chat_messages(CHAT_MESSAGE messages[], int count, const char *my_username) {
    char last_date[11] = ""; // YYYY-MM-DD + '\0'

    for (int i = 0; i < count; i++) {

        // extract date (first 10 chars)
        char current_date[11] = "";
        strncpy(current_date, messages[i].full_timestamp, 10);
        current_date[10] = '\0';

        // print date separator if date changed
        if (strcmp(current_date, last_date) != 0) {
            printf("\n--- %s ---\n", current_date);
            strcpy(last_date, current_date);
        }

        // extract time part (last 8 chars)
        char time_only[9] = "";
        strncpy(time_only, messages[i].full_timestamp + 11, 8);
        time_only[8] = '\0';

        // right-align own messages
        int padding = 80 - (int)strlen(messages[i].sender) - 3;
        if (padding < 0) padding = 0;

        if (strcmp(messages[i].sender, my_username) == 0) {
            printf("%*s [%s]: %s\n", padding + (int)strlen(messages[i].sender), messages[i].sender, time_only, messages[i].message);
        } else {
            printf("%s  [%s]: %s\n", messages[i].sender, time_only, messages[i].message);
        }
    }
}
