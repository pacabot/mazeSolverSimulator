/**************************************************************************/
/*!
 @file    Bluetooth.c
 @author  PLF (PACABOT)
 @date
 @version  0.0
 */
/**************************************************************************/
#include "config/config.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

/* Peripheral declarations */
#include "peripherals/display/ssd1306.h"
#include "peripherals/display/smallfonts.h"

/* Middleware declarations */
// TODO: Remove middleware declarations, as we are in lower level here
//#include "middleware/ring_buffer/ring_buffer.h"
#include "middleware/display/banner.h"

/* Declarations for this module */
#include "peripherals/bluetooth/bluetooth.h"

#define BLUETOOTH_BUFFER_SIZE 51200

// Array of possible baud rates
const int baudrates[] = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800,
//921600,
-0x7FFFFFFF    // Indicates the end of the array
        };

void bluetoothPrintf(const char *format, ...)
{
#if !defined DISABLE_BLUETOOTH
    static char buffer[BLUETOOTH_BUFFER_SIZE];
    va_list va_args;

    va_start(va_args, format);
    vsnprintf(buffer, BLUETOOTH_BUFFER_SIZE, format, va_args);
    va_end(va_args);

    printf(buffer);
#endif
}

void bluetoothWaitReady(void)
{
}

int isBluetoothEvent(char *evnt)
{
    if (strncmp(evnt, "AT-AB ", 6) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************
 * TEST FUNCTIONS
 *****************************************************************************/
