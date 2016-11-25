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
/* 
	A very cool function that turns a normal char buffer into a scrolling text console! ooh!
*/
void addToPrintBuf(char* text) {
	//If it's the first line, just append text
	if (printBufLines == 0) {
		__os_snprintf(printBuf, 1023, "%s%s", printBuf, text);
		printBufLines++;
	//If the Gamepad is out of space, we need to remove the top line from view.
	//This will make room for the new line.
	} else if (printBufLines == WINDOW_HEIGHT) {
		//Loop through and try to find the first \n (end of the top line)
		for (unsigned int i = 0; i < strlen(printBuf); i++) {
			if (printBuf[i] == '\n') {
				//Move text *after* the \n backwards to the start of the array
				memcpy(printBuf, printBuf + i + 1, strlen(printBuf) - i - 1);
				//Fix the null char
				printBuf[strlen(printBuf) - i - 1] = 0x00;
				//Append text
				__os_snprintf(printBuf, 1023, "%s\n%s", printBuf, text);
				break;
			}
		}
	//Otherwise, just a normal print. Append with a newline.
	} else {
		__os_snprintf(printBuf, 1023, "%s\n%s", printBuf, text);
		printBufLines++;
	}
}
