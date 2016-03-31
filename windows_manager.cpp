/*
 * windows_manager.c
 *
 *  Created on: 8 nov. 2015
 *      Author: colin
 */
#include "middleware/windows_manager/windows_manager.h"
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
window_typedef *windows_declared[MAX_WINDOWS]={NULL};


int addWindow(window_typedef *window, char* window_pix, char width, char height, char pos_x, char pos_y, char place)
{
	int i;
	window->height = height;
	window->width = width;
	window->pixels = window_pix;
	window->pos_x = pos_x;
	window->pos_y = pos_y;
	while(windows_declared[i] != NULL)
	{
		i++;
	}
	windows_declared[i] = window;
	return WINDOWS_MANAGER_DRIVER_E_ERROR;
}

// todo refresh
/*
 * private method
 */
int windowDrawChar(window_typedef *window, unsigned int x, unsigned int y, unsigned char c, const FONT_DEF *font)
{
	int rv;
	unsigned char col, column[font->u8Width];

	// Check if the requested character is available
	if ((c >= font->u8FirstChar) && (c <= font->u8LastChar))
	{
		// Retrieve appropriate columns from font data
		for (col = 0; col < font->u8Width; col++)
		{
			column[col] = font->au8FontTable[((c - 32) * font->u8Width) + col];    // Get first column of appropriate character
		}
	}
	else
	{
		// Requested character is not available in this font ... send a space instead
		for (col = 0; col < font->u8Width; col++)
		{
			column[col] = 0xFF;    // Send solid space
		}
	}

	// Render each column
	unsigned int xoffset, yoffset;
	for (xoffset = 0; xoffset < font->u8Width; xoffset++)
	{
		for (yoffset = 0; yoffset < (font->u8Height + 1); yoffset++)
		{
			unsigned char bit = 0x00;
			bit = (column[xoffset] << (8 - (yoffset + 1)));     // Shift current row bit left
			bit = (bit >> 7);                                   // Shift current row but right (results in 0x01 for black, and 0x00 for white)
			if (bit)
			{
				rv = windowDrawPixel(window, x + xoffset, y + yoffset);
				if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
					return rv;
			}
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
/*
 * public method
 */
int windowDrawPixel(window_typedef* window,unsigned char x, unsigned char y)
{
	if ((x >= window->width) || (y >= window->height))
		return WINDOWS_MANAGER_DRIVER_E_ERROR_OUT_OF_BUF;

	window->pixels[x+ ((y/8)*window->width) + 1] |= (1 << y%8);
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowClearPixel(window_typedef* window,unsigned char x, unsigned char y)
{
	if ((x >= window->width) || (y >= window->height))
			return WINDOWS_MANAGER_DRIVER_E_ERROR_OUT_OF_BUF;

		window->pixels[x+ ((y/8)*window->width) + 1] &= ~(1 << y%8);
		return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
int windowInvertPixel(window_typedef* window,unsigned char x, unsigned char y)
{
	if ((x >= window->width) || (y >= window->height))
				return WINDOWS_MANAGER_DRIVER_E_ERROR_OUT_OF_BUF;

	window->pixels[x+ ((y/8)*window->width) + 1] ^= (1 << (y % 8));
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
int windowClear(window_typedef* window)
{
	int number_of_cell = (window->height*window->width)/8;
	if ((window->height*window->width)%8 != 0)
		number_of_cell ++;
	memset(window->pixels,0,number_of_cell);
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
int windowDrawString(window_typedef* window,unsigned int x, unsigned int y, const char *text, const FONT_DEF *font)
{
	int rv;
	unsigned char l;
	for (l = 0; l < strlen((const char *)text); l++)
	{
		rv = windowDrawChar(window, x + (l * (font->u8Width + 1)), y, text[l], font);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
int windowPrintf(window_typedef* window,int x, int y, const FONT_DEF *font, const char *format, ...)
{
	int rv;
	char temp_buffer[43];
	va_list va_args;

	va_start(va_args, format);
    vsnprintf(temp_buffer, 43, format, va_args);
    va_end(va_args);

    rv = windowDrawString(window, x,y,(char *)temp_buffer, font);
    if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
    {
    	return rv;
    }
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
int windowDrawBmp(window_typedef* window,const unsigned char *bitmap, unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	unsigned char i;
	unsigned char j;
	int rv;

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			if (bitmap[i + (j / 8) * w] & 1 << (j % 8))
			{
				rv = windowDrawPixel(window, x + i, y + j);
				if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				{
					return rv;
				}
			}
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowDrawCircle(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char r)
{
	signed char f = 1 - r;
	signed char ddF_x = 1;
	signed char ddF_y = -2 * r;
	signed char x = 0;
	signed char y = r;
	windowDrawPixel(window, x0, y0+r);
	windowDrawPixel(window, x0, y0-r);
	windowDrawPixel(window, x0+r, y0);
	windowDrawPixel(window, x0-r, y0);

	while (x<y)
	{
		if (f>= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		windowDrawPixel(window, x0 + x, y0 + y);
		windowDrawPixel(window, x0 - x, y0 + y);
		windowDrawPixel(window, x0 + x, y0 - y);
		windowDrawPixel(window, x0 - x, y0 - y);

		windowDrawPixel(window, x0 + y, y0 + x);
		windowDrawPixel(window, x0 - y, y0 + x);
		windowDrawPixel(window, x0 + y, y0 - x);
		windowDrawPixel(window, x0 - y, y0 - x);
	}
	return 1;
}
int windowFillCircle(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char r)
{
	int i;
	int rv;
	signed char f = 1 - r;
	signed char ddF_x = 1;
	signed char ddF_y = -2 * r;
	signed char x = 0;
	signed char y = r;

	for (i=y0-r; i<=y0+r; i++)
	{
		rv =windowDrawPixel(window,x0, i);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
	}

	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		for (i=y0-y; i<=y0+y; i++)
		{
			rv =windowDrawPixel(window,x0+x, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
			rv =windowDrawPixel(window,x0-x, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
		for (i=y0-x; i<=y0+x; i++)
		{
			rv =windowDrawPixel(window,x0+y, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
			rv =windowDrawPixel(window,x0-y, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
	}
	return 1;
}

int windowClearCircle(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char r)
{
	int i;
	signed char f = 1 - r;
	signed char ddF_x = 1;
	signed char ddF_y = -2 * r;
	signed char x = 0;
	signed char y = r;
	int rv;

	for (i=y0-r; i<=y0+r; i++)
	{
		rv = windowClearPixel(window, x0, i);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
	}

	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		for (i=y0-y; i<=y0+y; i++)
		{
			windowClearPixel(window, x0+x, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
			windowClearPixel(window, x0-x, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
		for (i=y0-x; i<=y0+x; i++)
		{
			rv = windowClearPixel(window, x0+y, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
			rv = windowClearPixel(window, x0-y, i);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
int windowFillRect(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	int i, j, rv;
	// stupidest version - just pixels - but fast with internal buffer!
	for (i=x; i<x+w; i++) {
		for (j=y; j<y+h; j++) {
			rv =windowDrawPixel(window,i, j);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowInvertArea(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	int i, j, rv;
	// stupidest version - just pixels - but fast with internal buffer!
	for (i = x; i < (x + w); i++)
	{
		for (j = y; j < (y + h); j++)
		{
			rv =windowInvertPixel(window, i, j);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowClearRect(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	int i, j, rv;
	// stupidest version - just pixels - but fast with internal buffer!
	for (i=x; i<x+w; i++) {
		for (j=y; j<y+h; j++) {
			rv = windowClearPixel(window, i, j);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowDrawRect(window_typedef* window,unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	int i, rv;
	// stupidest version - just pixels - but fast with internal buffer!
	for (i=x; i<x+w; i++) {
		rv =windowDrawPixel(window,i, y);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
		rv =windowDrawPixel(window,i, y+h-1);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
	}
	for (i=y; i<y+h; i++) {
		rv =windowDrawPixel(window,x, i);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
		rv =windowDrawPixel(window,x+w-1, i);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowDrawDashedLine(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	int rv ;
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int8_t err = dx / 2;
	int8_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;}

	for (; x0<x1; x0++) {
		if (steep)
		{
			if ((x0 % 2) == 0)
			{
				rv =windowDrawPixel(window, y0, x0);
				if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
					return rv;
			}
		} else
		{
			if ((x0 % 2) == 0)
			{
				rv =windowDrawPixel(window,x0, y0);
				if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
					return rv;
			}
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowDrawLine(window_typedef* window,unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	int rv;
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int8_t err = dx / 2;
	int8_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;}

	for (; x0<x1; x0++) {
		if (steep)
		{
			rv = windowDrawPixel(window, y0, x0);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
		else
		{
			rv = windowDrawPixel(window, x0, y0);
			if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
				return rv;
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}

int windowProgressBar(window_typedef* window,unsigned char x, unsigned char y, unsigned char state)
{
	int rv;
	windowDrawRect(window, x, y, 104, 8);
	if (state > 0 || state <= 100)
	{
		rv = windowFillRect(window, x + 2, y + 2, state, 4);
		if (rv != WINDOWS_MANAGER_DRIVER_E_SUCCESS)
			return rv;
	}
	return WINDOWS_MANAGER_DRIVER_E_SUCCESS;
}
