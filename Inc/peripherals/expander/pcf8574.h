/**************************************************************************/  
	/* ! @file PCF8574.h @author PLF (PACABOT) @date @version 0.0 
	   Driver for expander PCF8574 */ 
/**************************************************************************/ 
#ifndef __PCF8574_H__
#define __PCF8574_H__
	 
	/* Error codes */ 
#define PCF8574_DRIVER_E_SUCCESS  0
#define PCF8574_DRIVER_E_ERROR    1
	
#define JOY_UP 		1
#define JOY_DOWN 	2
#define JOY_LEFT 	3
#define JOY_RIGHT 	4
#define JOY_SEVERAL 255
char expanderJoyFiltered(void);
 
#endif	/*  */
