/*
	PINEAPPLE
	https://github.com/QuarkTheAwesome/pineapple
	
	Most of this was by CreeperMario - very cool stuff
*/

#include <iosuhax/iosuhax.h>
#include <iosuhax/iosuhax_devoptab.h>
#include "dynamic_libs/os_functions.h"
#include "iosuhax_init.h"
#include "screen.h"

int fsaFd;

int iosuhaxInit() {
	char buf[256];
	
	addToPrintBuf("Opening /dev/iosuhax...");
	renderScreen();
	
	int ret = IOSUHAX_Open();
	if (ret < 0) {
		__os_snprintf(buf, 255, "Could not open /dev/iosuhax! Error %d (0x%08X)", ret, ret);
		addToPrintBuf(buf);
		renderScreen();
		iosuhaxShutdown();
		return -1;
	}
	__os_snprintf(buf, 255, "/dev/iosuhax handle: 0x%08X (%d)", ret, ret);
	addToPrintBuf(buf);
	renderScreen();
	
	addToPrintBuf("Initialising IOSU filesystem...");
	renderScreen();
	
	fsaFd = IOSUHAX_FSA_Open();
	if (fsaFd < 0) {
		__os_snprintf(buf, 255, "Could not initialise IOSU filesystem! Error %d (0x%08X)", fsaFd, fsaFd);
		addToPrintBuf(buf);
		renderScreen();
		iosuhaxShutdown();
		return -1;
	}
	__os_snprintf(buf, 255, "IOSU filesystem initialised. Handle: 0x%08X (%d)", fsaFd, fsaFd);
	addToPrintBuf(buf);
	renderScreen();
	
	addToPrintBuf("Mounting NAND (/vol/storage_mlc01 -> nand:/)...");
	renderScreen();
	
	ret = mount_fs("nand", fsaFd, NULL, "/vol/storage_mlc01");
	if (ret < 0) {
		__os_snprintf(buf, 255, "Could not mount NAND! Error %d (0x%08X)", ret, ret);
		addToPrintBuf(buf);
		renderScreen();
		iosuhaxShutdown();
		return -1;
	}
	__os_snprintf(buf, 255, "Mounted NAND! (0x%08X)", ret);
	addToPrintBuf(buf);
	renderScreen();
	
	return 0;
}

void iosuhaxShutdown() {
	unmount_fs("nand");
	fsaFd = IOSUHAX_FSA_Close(fsaFd);
	IOSUHAX_Close();
}
