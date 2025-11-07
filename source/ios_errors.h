// IOS errors
#include <gccore.h>

struct IOSError {
    s32 code;
    char* error_message;
};

void get_ios_error_message(char *buffer, s32 code);