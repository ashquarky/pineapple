/*
	PINEAPPLE
	https://github.com/QuarkTheAwesome/pineapple
*/

#include <stdlib.h>
#include <string.h>
//$DEVKITPRO/portlibs/ppc/include/iosuhax/iosuhax.h
//This may change if a new standard arises
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/extra_functions.h"
#include "iosuhax_init.h"
#include "screen.h"

int Menu_Main() {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	InitExtraFunctionPointers();

	screenInit();
	
	addToPrintBuf("Pineapple v0.0.1");
	renderScreen();
	
	int ret = iosuhaxInit();
	if (ret < 0) {
		//error behaviour
		goto promptAndExit;
	}
	
	addToPrintBuf("Cleaning up...");
	renderScreen();
	
	iosuhaxShutdown();

promptAndExit:	
	addToPrintBuf("Press HOME to exit.");
	renderScreen();
	
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
