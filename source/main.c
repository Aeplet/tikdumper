#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#include "globals.h"
#include "ios.h"
#include "dump.h"
#include "storage.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void return_to_loader() {
   printf("\n\nExiting...");
   exit(0);
}

bool started_dumping = false;
bool can_exit = true;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
    // you know the drill, we need that IOS reloaded, and that bomb dropped
    IOS_ReloadIOS(IOS_GetVersion());
    disable_ahbprot();
    apply_runtime_ios_patches();
	// Initialise the video system
	VIDEO_Init();

	// These functions initialize the attached controllers
    PAD_Init();
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth-20,rmode->xfbHeight-20,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	//SYS_STDIO_Report(true);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Clear the framebuffer
	VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);

	// Make the display visible
	VIDEO_SetBlack(false);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");

	printf("tikdumper %s\n", VERSION);
    printf("Created by Aep\n\n");

	bool device_mounted = mount_device();

	if (!device_mounted) {
		printf("\nFailed to mount storage device! Please insert an SD Card or\nUSB drive, then run this application again.\n\n");
	}
	else {
		printf("Press A to start dumping tickets\n");
	}
    printf("Press HOME/START to exit\n");

	while(1) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
        PAD_ScanPads();
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);
        u32 pressed_gc = PAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ((pressed & WPAD_BUTTON_HOME || pressed_gc & PAD_BUTTON_START) && can_exit) return_to_loader();

        // Start the dumping process
        if ((pressed & WPAD_BUTTON_A || pressed_gc & PAD_BUTTON_A) && !started_dumping && device_mounted) {
            started_dumping = true;
			can_exit = false;
            s32 res = start_dump();
			if (res <= 0) {
				 printf("Something... went wrong. Please report this on the GitHub issues page.\n");
			}
			else {
				printf("Successfully dumped %d tickets!\n", res);
			}
			can_exit = true;
			printf("Press HOME/START to exit\n");
        }

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}