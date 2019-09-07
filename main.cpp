#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL2/SDL.h>
#include "middleware/settings/settings.h"
#include "application/solverMaze/solverMaze.h"
settings zhonxSettings;
SDL_Surface *screen;
SDL_Window *window;
void pause()
{
	SDL_Event event;
	do
	{
		SDL_WaitEvent(&event);
	}
	while (event.type != SDL_KEYDOWN);
}

int main(int argc, char **argv)
{
	zhonxSettings.maze_end_coordinate.x = 8;
	zhonxSettings.maze_end_coordinate.y = 8;
	zhonxSettings.cell_cost = 5;
	zhonxSettings.wall_know_cost = 2;
	zhonxSettings.start_orientation = 0;

	// initialize SDL videolabel
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	// make sure SDL cleans up before exit
	atexit(SDL_Quit);

	// create a new window
	window = SDL_CreateWindow("maze simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 128 * multiplicateur, 64 * multiplicateur, 0);
	screen = SDL_GetWindowSurface(window);
	if (!screen)
	{
		printf("Unable to set 640x480 video: %s\n", SDL_GetError());
		return 1;
	}

	// load an image
	SDL_Surface *bmp = SDL_LoadBMP("PacaBot-2.bmp");
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
	SDL_SetColorKey(bmp, 1,
					SDL_MapRGB(bmp->format, 255, 255, 255));
	SDL_BlitSurface(bmp, 0, screen, &dstrect);
	SDL_FreeSurface(bmp);
	SDL_UpdateWindowSurface(window);
	pause();
	maze_solver_new_maze();
	SDL_Quit();
	// all is well ;)
	printf("Exited cleanly\n");
	return 0;
}
