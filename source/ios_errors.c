#include <stdio.h>

#include "ios_errors.h"
#include "globals.h"

struct IOSError IOSErrors[] = {
    {0, "Success!"},
    {-102, "No permissions?"},
    {-106, "Ticket for title does not exist!"},
    {-996, "Read error"},
    // Too lazy to add more, will add if they come up using the application. Send a pull request if you want to add more.
    // No more than 100 characters by the way
    // - Aep
};

void get_ios_error_message(char *buffer, s32 code) {
    size_t error_list_size = sizeof(IOSErrors) / sizeof(IOSErrors[0]);
    for (int i = 0; i < error_list_size; i++) {
        if (IOSErrors[i].code == code) {
            const char *message = IOSErrors[i].error_message; 
            snprintf(buffer, MAX_IOS_ERROR_MESSAGE_SIZE, "%s", message);
            return;
        }
    }
    // shitty fallback, but whatever
    static const char* fallback_string = "Unknown error, please report on the GitHub issues page.";
    snprintf(buffer, MAX_IOS_ERROR_MESSAGE_SIZE, "%s (code %d)", fallback_string, code);
}