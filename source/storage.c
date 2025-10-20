#include <gccore.h>
#include <stdio.h>
#include <sdcard/wiisd_io.h>
#include <fat.h>
#include <string.h>

// TODO: SWITCH TO A USER PROMPT INSTEAD OF DOING SD THEN USB
// Try to mount SD card first
// Bit messy.
char device[4];

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
}

void get_storage_device(char *passed_device) {
    strcpy(passed_device, device);
}