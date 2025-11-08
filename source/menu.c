// PLEASE AT SOMEPOINT REDO THIS MENU THAT IS HELD TOGETHER BY FUCKING I DONT KNOW ANYMORE
#include <gccore.h>
#include <stdio.h>
#include <wiiuse/wpad.h>

#include "globals.h"

#define MAX_STORAGE_DEVICE_ENTRY_SIZE 18

struct StorageDeviceEntry {
    storage_device_t device;
    char* name;
};

struct StorageDeviceEntry StorageDeviceEntries[] = {
    {SD_CARD, "SD Card"},
    {USB_STORAGE, "USB Storage"},
    {SDGECKO_A, "SD Gecko (Slot A)"},
    {SDGECKO_B, "SD Gecko (Slot B)"},
};

storage_device_t current_device = SD_CARD; // Default to SD
size_t storage_device_list_size = sizeof(StorageDeviceEntries) / sizeof(StorageDeviceEntries[0]);

// get the name

void get_storage_entry_name(char *buffer, storage_device_t device) {
    for (int i = 0; i < storage_device_list_size; i++) {
        if (StorageDeviceEntries[i].device == device) {
            const char *name = StorageDeviceEntries[i].name; 
            snprintf(buffer, MAX_STORAGE_DEVICE_ENTRY_SIZE, "%s", name);
            return;
        }
    }
}

void clear_console() {
    printf("\x1b[2J");
}

void print_top() {
    printf("tikdumper %s\n", APP_VERSION);
    printf("Created by Aep\n\n");
    printf("Press HOME/START to exit\n");
}

void full_screen_reset() {
    clear_console();
    print_top();
}

void print_device_select_text() {
    full_screen_reset();
    char current_device_name[MAX_STORAGE_DEVICE_ENTRY_SIZE];
    get_storage_entry_name(current_device_name, current_device);
    printf("Select storage device to use: < %s >\n\n", current_device_name);
}

void device_select_menu() {
    print_device_select_text();
}

void move_left_right(u8 direction) {
    // really messy, scuffed as fucking hell, this needs to be cleaned up.
    // 0 = left
    // 1 = right
    if (direction == 1 && (int)current_device < (storage_device_list_size - 1)) { // upper limit (4 - 1 currently)
        current_device++;
        device_select_menu();
    }
    else if (direction == 0 && (int)current_device >= 1) { // lower limit (1)
        current_device--;
        device_select_menu();
    }
}

storage_device_t get_device_menu_selection() {
    return current_device;
}