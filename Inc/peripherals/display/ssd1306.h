/**************************************************************************/
/*!
    @file     ssd1306.h
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10
*/
/**************************************************************************/
#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <peripherals/display/smallfonts.h>


/* Error codes */
#define SSD1306_DRIVER_E_SUCCESS  0
#define SSD1306_DRIVER_E_ERROR    1

#define SSD1306_LCDWIDTH                  (128)
#define SSD1306_LCDHEIGHT                 (64)
#define SSD1306_LCDPAGEHEIGHT             (8)   //8 lines per page

#define LINE_SPACING                    10
#define HEAD_MARGIN                     11

#define BANNER_AREA 1
#define MAIN_AREA 1
#define ALL_AREA 1

// Initialisation/Config Prototypes
void ssd1306WaitReady();
void ssd1306Init(unsigned char vccstate);
void ssd1306DrawPixel(unsigned char x, unsigned char y);
void ssd1306ClearPixel(unsigned char x, unsigned char y);
void ssd1306InvertPixel(unsigned char x, unsigned char y);
unsigned char ssd1306GetPixel(unsigned char x, unsigned char y);
void ssd1306ClearScreen(int clearType);
void ssd1306Refresh(void);
void ssd1306DrawString(unsigned int x, unsigned int y, const char *text, const FONT_DEF *font);
void ssd1306PrintInt(unsigned int x,unsigned int y, const char *text, unsigned int val, const FONT_DEF *font);
void ssd1306Printf(int x, int y, const FONT_DEF *font, const char *format, ...);
void ssd1306ShiftFrameBuffer(unsigned char height);
void ssd1306DrawBmp(const unsigned char *bitmap, unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ssd1306DrawCircle(unsigned char x0, unsigned char y0, unsigned char r);
void ssd1306FillCircle(unsigned char x0, unsigned char y0, unsigned char r);
void ssd1306ClearCircle(unsigned char x0, unsigned char y0, unsigned char r);
void ssd1306FillRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ssd1306ClearRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ssd1306DrawRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ssd1306InvertArea(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ssd1306DrawDashedLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void ssd1306DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void ssd1306ProgressBar(unsigned char x, unsigned char y, unsigned char state);
void ssd1306DrawStringAtLine(unsigned int x, unsigned int line, const char *text, const FONT_DEF *font);
void ssd1306PrintfAtLine(int x, int line, const FONT_DEF *font, const char *format, ...);
#endif
