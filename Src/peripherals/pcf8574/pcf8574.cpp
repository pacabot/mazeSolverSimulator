/**************************************************************************/  
	/* ! @file expander.c @author PLF (PACABOT) @date @version 0.0 
	   Driver for expander PCF8574 */ 
/**************************************************************************/ 
#include "peripherals/expander/pcf8574.h"
#include <SDL2/SDL.h>

char expanderJoyFiltered(void)  { 	SDL_Event event;
 	do
	{
		SDL_WaitEvent(&event);
	} while (event.type != SDL_KEYDOWN);

	switch (event.type)
	{
		case SDL_QUIT:
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_UP:
					return JOY_UP;
					break;
				case SDLK_DOWN:
					return JOY_DOWN;
					break;
				case SDLK_RIGHT:
					return JOY_RIGHT;
					break;
				case SDLK_LEFT:
					return JOY_LEFT;
					break;
				default:
				break;
			}
			break;
		default:
			break;
	}
	return JOY_SEVERAL;
}
