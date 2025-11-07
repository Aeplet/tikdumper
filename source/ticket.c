#include <ogc/ipc.h>
#include <gccore.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "globals.h"
#include "ios_errors.h"

s32 get_title_ticket(char *ticket_buffer, u64 id) {
    char file_path[256];

    snprintf(file_path, sizeof(file_path), "/ticket/%08x/%08x.tik", TITLE_UPPER(id), TITLE_LOWER(id));

    int fd = IOS_Open(file_path, 1);
    if (fd < 0) {
        IOS_Close(fd);

        char error_message[MAX_IOS_ERROR_MESSAGE_SIZE];
        get_ios_error_message(error_message, fd);

        printf("Failed to read ticket for title %016llx: \n%s\n", id, error_message);
        return 0; // file probably does not exist
    }

    memset(ticket_buffer, 0, TICKET_SIZE);
    int ret = IOS_Read(fd, ticket_buffer, TICKET_SIZE);
    if (ret > TICKET_SIZE) {
        IOS_Close(fd);
        printf("Failed to read ticket!\n (ret = %d)", ret);
        return 0;
    }
    IOS_Close(fd);
    return 1;
}