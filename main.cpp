#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include "middleware/settings/settings.h"
#include "solverMaze.h"
settings zhonxSettings;
SDL_Surface* screen;
void pause ()
{
    SDL_Event event;
    do
            {
            SDL_WaitEvent(&event);
            }
    while (event.type!= SDL_KEYDOWN);
}

int main ( int argc, char** argv )
{
	zhonxSettings.x_finish_maze=8;
	zhonxSettings.y_finish_maze=8;
	zhonxSettings.color_sensor_enabled=false;

    // initialize SDL videolabel
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(128*multiplicateur, 64*multiplicateur, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface* bmp = SDL_LoadBMP("PacaBot-2.bmp");
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

    // draw bitmap
    SDL_SetColorKey(bmp, SDL_SRCCOLORKEY, SDL_MapRGB(bmp->format, 255, 255, 255));
    SDL_BlitSurface(bmp, 0, screen, &dstrect);
    SDL_FreeSurface(bmp);
    SDL_Flip(screen);
    pause();
    maze();
    SDL_Quit();
    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
