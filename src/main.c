/*
	PINEAPPLE
	https://github.com/QuarkTheAwesome/pineapple
*/

#include <stdlib.h>
#include <string.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"

void renderScreen();
void addToPrintBuf(char* text);

char* printBuf;
unsigned int printBufLen = 0;
unsigned int printBufLines = 0;

int Menu_Main() {
	InitOSFunctionPointers();
	InitVPadFunctionPointers();

	OSScreenInit();
	
	unsigned int buffer0Size = OSScreenGetBufferSizeEx(0);
	
	OSScreenSetBufferEx(0, (void*)0xF4000000);
	OSScreenSetBufferEx(1, (void*)(0xF4000000 + buffer0Size));
	
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
		
	printBuf = malloc(1024);
	
	char buf[10];
	for (int i = 0; i < 100; i++) {
		__os_snprintf(buf, 9, "n: %d", i);
		addToPrintBuf(buf);
		renderScreen();
	}
	
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
	
	DCFlushRange((void*)0xF4000000, buffer0Size + OSScreenGetBufferSizeEx(1));
	
	free(printBuf);
	return EXIT_SUCCESS;
}

void renderScreen() {
	OSScreenClearBufferEx(0, 0x000000FF);
	OSScreenClearBufferEx(1, 0x000000FF);
	
	OSScreenPutFontEx(0, 0, 0, printBuf);
	OSScreenPutFontEx(1, 0, 0, printBuf);
	
	DCFlushRange((void*)0xF4000000, OSScreenGetBufferSizeEx(0) + OSScreenGetBufferSizeEx(1));
	
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

#define WINDOW_HEIGHT 17

void addToPrintBuf(char* text) {
	if (printBufLines == 0) {
		__os_snprintf(printBuf, 1023, "%s%s", printBuf, text);
		printBufLines++;
	} else if (printBufLines == WINDOW_HEIGHT) {
		for (unsigned int i = 0; i < strlen(printBuf); i++) {
			if (printBuf[i] == '\n') {
				memcpy(printBuf, printBuf + i + 1, strlen(printBuf) - i - 1);
				printBuf[strlen(printBuf) - i - 1] = 0x00;
				__os_snprintf(printBuf, 1023, "%s\n%s", printBuf, text);
				break;
			}
		}
		
	} else {
		__os_snprintf(printBuf, 1023, "%s\n%s", printBuf, text);
		printBufLines++;
	}
}
