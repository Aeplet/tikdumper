#include <gccore.h>
#include <stdio.h>
#include <sdcard/wiisd_io.h>
#include <sdcard/gcsd.h>
#include <fat.h>
#include <string.h>

#include "globals.h"
#include "menu.h"

char device[8];
storage_device_t device_t;

void init_device() {
	device_select_menu();
}

bool mount_device() {
	switch (get_device_menu_selection()) {
		case SD_CARD:
			__io_wiisd.startup();
			if (__io_wiisd.isInserted() == 0 || fatMountSimple ("sd", &__io_wiisd) == 0) {
				__io_wiisd.shutdown();
				printf("Failed to mount storage device!\n");
				return false;
			}
			break;
		case USB_STORAGE:
			__io_usbstorage.startup();
			if (__io_usbstorage.isInserted() == 0 || fatMountSimple ("fat", &__io_usbstorage) == 0) {
				__io_usbstorage.shutdown();
				printf("Failed to mount storage device!\n");
				return false;
			}
			break;
		case SDGECKO_A:
			__io_gcsda.startup();
			if (__io_gcsda.isInserted() == 0 || fatMountSimple ("sda", &__io_gcsda) == 0) {
				__io_gcsda.shutdown();
				printf("Failed to mount storage device!\n");
				return false;
			}
			break;
		case SDGECKO_B:
			__io_gcsdb.startup();
			if (__io_gcsdb.isInserted() == 0 || fatMountSimple ("sdb", &__io_gcsdb) == 0) {
				__io_gcsdb.shutdown();
				printf("Failed to mount storage device!\n");
				return false;
			}
			break;
		default:
			printf("What is this unknown device? Please report on the GitHub issues page if you managed to get this error.");
			break;
	}
	return true;
	// if none errored then we're good
}

void get_storage_device(char *passed_device) {
    snprintf(passed_device, sizeof(device), device);
}

/* Archived code
bool mount_device() {
	__io_wiisd.startup();
	if (__io_wiisd.isInserted() == 0 || fatMountSimple ("sd", &__io_wiisd) == 0) {
		// If no SD card was inserted, then try a USB storage device
		printf("Failed to mount SD card, trying USB drive\n");
        __io_wiisd.shutdown();

		__io_usbstorage.startup();
		if (__io_usbstorage.isInserted() == 0 || fatMountSimple ("fat", &__io_usbstorage) == 0) {
			printf("Failed to mount USB drive!\n");
            __io_usbstorage.shutdown();
		}
		else {
            printf("Successfully mounted USB storage device!\n");
			strcpy(device, "fat");
            return true;
		}
	}
	else {
		printf("Successfully mounted SD card!\n");
		strcpy(device, "sd");
        return true;
	}
    return false;
}*/