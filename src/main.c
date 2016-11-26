/*
	PINEAPPLE
	https://github.com/QuarkTheAwesome/pineapple
*/

#include <stdlib.h>
#include <string.h>
//$DEVKITPRO/portlibs/ppc/include/iosuhax/iosuhax.h
//This may change if a new standard arises
#include <iosuhax/iosuhax.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/extra_functions.h"
#include "screen.h"

int Menu_Main() {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	InitExtraFunctionPointers();

	screenInit();

	int iosuhax = IOSUHAX_Open();
	
	char buf[256];
	__os_snprintf(buf, 255, "/dev/iosuhax handle: 0x%08X (%d)", iosuhax, iosuhax);
	addToPrintBuf(buf);
	renderScreen();
	
	IOSUHAX_Close();
	
	VPADInit();

	VPADData vpad;
	int error;

	for (;;) {
		VPADRead(0, &vpad, 1, &error);

		if (!error) {
			if (vpad.btns_h & VPAD_BUTTON_HOME) {
				break;
			}
		}
	}
	
	screenShutdown();

	return EXIT_SUCCESS;
}
