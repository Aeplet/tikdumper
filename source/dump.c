#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <fat.h>
#include <errno.h>

#include "globals.h"
#include "ticket.h"
#include "storage.h"

// actual dumping to the SD/USB.
s32 dump_to_storage(u64 title_id, char *ticket_buffer, char* storage_device) {
    //printf("Dumping %016llx's ticket to %s!\n", title_id, storage_device); (for debugging)
    char dump_path[256];
    snprintf(dump_path, sizeof(dump_path), "%s/%016llx.tik", DUMP_FOLDER, title_id);
    if (mkdir(DUMP_FOLDER, 0) == 0 || errno == EEXIST) {
        FILE *file = fopen(dump_path, "wb");
        if (!file) {
            printf("Failed to open path %s on the storage device? Is your SD locked?", dump_path);
            return 0;
        }
        size_t written = fwrite(ticket_buffer, 1, TICKET_SIZE, file);
        fclose(file);

        if (written != TICKET_SIZE) {
            printf("Write incomplete! wrote %zu of %zu bytes\n", written, TICKET_SIZE);
            return 0;
        }
        printf("Dumped ticket for title %016llx!\n", title_id);
        return 1;
    }
    return 0;
}

// thank you darkmattercore/bushing for the title fetching :3
s32 start_dump() {
    printf("\nTicket dump started!\n");
    // hm. the fun begins
    // how many titles do we have?
    u32 title_count;
    int res = ES_GetNumTitles(&title_count);

    u64 *buffer = memalign(32, sizeof(u64) * title_count);
    if (!buffer) {
        printf("Failed to allocate title list memory!");
        return 0;
    }

    res = ES_GetTitles(buffer, title_count);
    if (res < 0) {
        printf("ES_GetTitles returned %d", res);
        free(buffer);
        return 0;
    }

    char storage_device[4];
    get_storage_device(storage_device);

    int dump_count = 0;

    // reminder to self: use %016llx to format U64 title IDs

    for (u32 i = 0; i < title_count; i++) {
        //printf("%016llx\n", buffer[i]);
        u64 title_id = buffer[i];
        u8 *ticket_buffer = memalign(32, TICKET_ALLOC);   // must be a 32-byte aligned heap buffer to avoid a crash! and also 0x20 padding
        if (!ticket_buffer) {
            printf("ticket_buffer alloc failed\n");
            free(buffer);
            return 0;
        }

        if (get_title_ticket((char*)ticket_buffer, title_id)) {
            if (!dump_to_storage(title_id, (char*)ticket_buffer, storage_device)) {
                printf("Failed to dump %016llx\n's ticket!\n", title_id);
            }
            else {
                dump_count++;
            }
        }

        free(ticket_buffer);
    }
    return dump_count;
}