/*
	PINEAPPLE
	https://github.com/QuarkTheAwesome/pineapple
*/

#ifndef _SCREEN_H_
#define _SCREEN_H_

#define PRINT_BUF_SIZE 2048
#define WINDOW_HEIGHT 17

void screenInit();
void screenShutdown();
void renderScreen();
void addToPrintBuf(char* text);

#endif //_SCREEN_H_
