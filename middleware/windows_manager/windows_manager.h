/*
 * windows_manager.h
 *
 *  Created on: 8 nov. 2015
 *      Author: colin
 */

#ifndef WINDOWS_MANAGER_H_
#define WINDOWS_MANAGER_H_
/*dependences*/
#include "smallfonts.h"
/*defines*/
#define WINDOWS_MANAGER_DRIVER_E_SUCCESS           0
#define WINDOWS_MANAGER_DRIVER_E_ERROR            1 // defaul error
#define WINDOWS_MANAGER_DRIVER_E_ERROR_OUT_OF_BUF  1 // the pixel is out of the window

#define MAX_WINDOWS 10
/*typeDef*/
#define swap(a, b){ \
		unsigned char c; \
		c = a; \
		a = b; \
		b = c; \
}

typedef struct
{
	char width;
	char height;
	char pos_x;
	char pos_y;
	char* pixels;
}window_typedef;
/*function*/
int addWindow(window_typedef *window, char* window_pix, char width, char height, char pos_x, char pos_y, char place);
int removeWindow(window_typedef* window);
int windowClear(window_typedef* window);
int windowRefresh();

int windowDrawPixel(window_typedef* window,unsigned char x, unsigned char y);
int windowClearPixel(window_typedef* window,unsigned char x, unsigned char y);
int windowInvertPixel(window_typedef* window,unsigned char x, unsigned char y);
unsigned char windowGetPixel(window_typedef* window,unsigned char x, unsigned char y);
int windowDrawString(window_typedef* window,unsigned int x, unsigned int y, const char *text, const FONT_DEF *font);
int windowPrintf(window_typedef* window,int x, int y, const FONT_DEF *font, const char *format, ...);
int windowShiftFrameBuffer(window_typedef* window,unsigned char height);
int windowDrawBmp(window_typedef* window,const unsigned char *bitmap, unsigned char x, unsigned char y, unsigned char w, unsigned char h);
int windowDrawCircle(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char r);
int windowFillCircle(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char r);
int windowClearCircle(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char r);
int windowFillRect(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h);
int windowClearRect(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h);
int windowDrawRect(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h);
int windowInvertArea(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h);
int windowDrawDashedLine(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
int windowDrawLine(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
int windowProgressBar(window_typedef* window,unsigned char x, unsigned char y, unsigned char state);
int windowTest(void);


#endif /* WINDOWS_MANAGER_H_ */
