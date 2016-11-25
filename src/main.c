/*
	PINEAPPLE
	https://github.com/QuarkTheAwesome/pineapple
*/

#include <stdlib.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"

int Menu_Main() {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();

	OSScreenInit();
	
	unsigned int buffer0Size = OSScreenGetBufferSizeEx(0);
	unsigned int totalBufferSize = buffer0Size + OSScreenGetBufferSizeEx(1);
	
	OSScreenSetBufferEx(0, (void*)0xF4000000);
	OSScreenSetBufferEx(1, (void*)(0xF4000000 + buffer0Size));
	
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	
	OSScreenClearBufferEx(0, 0x000000FF);
	OSScreenClearBufferEx(1, 0x000000FF);
	
	OSScreenPutFontEx(0, 0, 0, "Hello World!");
	OSScreenPutFontEx(1, 0, 0, "Hello World 2!");
	
	DCFlushRange((void*)0xF4000000, totalBufferSize);
	
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
	
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
	
	OSScreenClearBufferEx(0, 0x00000000);
	OSScreenClearBufferEx(1, 0x00000000);
	
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
	
	OSScreenClearBufferEx(0, 0x00000000);
	OSScreenClearBufferEx(1, 0x00000000);
	
	DCFlushRange((void*)0xF4000000, totalBufferSize);
	
	return EXIT_SUCCESS;
}
