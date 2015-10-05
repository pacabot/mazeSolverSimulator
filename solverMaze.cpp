#include <stdio.h>
#include <string.h>
#include <math.h>

#include "middleware/settings/settings.h"

#ifndef codeblocks

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "config/basetypes.h"

/* peripherale inlcudes*/
#include "peripherals/display/ssd1306.h"
#include "peripherals/expander/pcf8574.h"
#include "peripherals/motors/motors.h"
#include "peripherals/lineSensors/lineSensors.h"
#include "peripherals/telemeters/telemeters.h"
#include "peripherals/bluetooth/bluetooth.h"

/* meddleware include */
#include "middleware/settings/settings.h"
#include "middleware/wall_sensors/wall_sensors.h"
#include "middleware/controls/pidController/pidController.h"
#include "middleware/controls/motionControl/positionControl.h"
#include "middleware/controls/motionControl/speedControl.h"
#include "middleware/controls/motionControl/transfertFunction.h"
#include "middleware/controls/motionControl/followControl.h"
#include "middleware/controls/motionControl/mainControl.h"

/*application include */
#include "application/solverMaze/solverMaze.h"
#include "application/solverMaze/robotInterface.h"
#include "application/solverMaze/run.h"

#else
#include "solverMaze.h"
#include <stdlib.h>
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include <SDL/SDL.h>
#include "run.h"
#include "robotInterface.h"
#include "smallfonts.h"
#include "ssd1306.h"
#include "pcf8574.h"
#endif // codeblocks
#define debug
#ifdef debug
#undef END_SPEED_TRANSLATION
#define END_SPEED_TRANSLATION 0
#endif

int maze(void)
{
	char posXStart, posYStart; // it's the coordinates which Zhonx have at the start
	labyrinthe maze;
	mazeInit (&maze);
	positionRobot positionZhonx;

	telemetersInit();
	telemetersStart();
	mainControlInit ();
#ifndef codeblocks
	control_params.follow_state = true;
	follow_control.follow_type = FOLLOW_WALL;//NOFOLLOW;
#else
	pt_zhonx_position=&positionZhonx;
    ssd1306ClearScreen();
    ssd1306Refresh();
#endif // codeblocks
	move(0, 0, 0, 0);
	HAL_Delay(500);

	/*init for different micromouse competition*/

	if (zhonxSettings.color_sensor_enabled == true) // if it's the Nimes micromouse competition
	{
		positionZhonx.x = MAZE_SIZE / 2;
		positionZhonx.y = MAZE_SIZE / 2; // the robot start at the middle of the maze
		positionZhonx.orientation = NORTH; // the robot is pointing the north
		zhonxSettings.x_finish_maze = 0;
		zhonxSettings.y_finish_maze = 0; // we want to go to the case how have address (0,0)
	}
	else // it's the Birmingham competition
	{
		positionZhonx.x = 0;
		positionZhonx.y = 0; // the robot start in the corner
		positionZhonx.orientation = EAST;
		// the position of the finish is defined in the menu
	}
	/*end of initialization for different micromouse competition*/
	positionZhonx.midOfCell = true;
	posXStart = positionZhonx.x;
	posYStart = positionZhonx.y;
	printMaze (maze, positionZhonx.x, positionZhonx.y);
//	if (zhonxSettings.calibration_enabled == true)
//	{
//		calibrateSimple ();
//	}
	motorsSleepDriver(OFF);
	for (int i = 0; i < 4; ++i)
	{
		rotate90WithCal(CW, 300, 0);
		while(isEndMove() != true);
		positionZhonx.orientation=(positionZhonx.orientation+1)%4;
		newCell (getCellState (), &maze, positionZhonx);
		printLength(maze,positionZhonx.x,positionZhonx.y);
	}
	move (0, -CELL_LENGTH/2, 50, 0);
	while(isEndMove() != true);
	motorsSleepDriver(ON);

	printMaze(maze,positionZhonx.x, positionZhonx.y);
	do
	{
		waitStart ();
		exploration (&maze, &positionZhonx, zhonxSettings.x_finish_maze,
				zhonxSettings.y_finish_maze);
//		if (zhonxSettings.calibration_enabled == true)
//			calibrateSimple ();
		for (int i = 0; i < 4; ++i)
		{
			rotate90WithCal(CW, 300, 0);
			while(isEndMove() != true);
			positionZhonx.orientation=(positionZhonx.orientation+1)%4;
			newCell (getCellState (), &maze, positionZhonx);
			printLength(maze,positionZhonx.x,positionZhonx.y);
		}
		HAL_Delay (2000);
		exploration (&maze, &positionZhonx, posXStart, posYStart);
//		if (zhonxSettings.calibration_enabled == true)
//			calibrateSimple ();
		doUTurn (&positionZhonx);
		HAL_Delay (2000);
	}while (false
			== miniwayFind (&maze, posXStart, posYStart,
					zhonxSettings.x_finish_maze, zhonxSettings.y_finish_maze));
	run1 (&maze, &positionZhonx, posXStart, posYStart);
	run2 (&maze, &positionZhonx, posXStart, posYStart);
	return MAZE_SOLVER_E_SUCCESS;
}

void exploration(labyrinthe *maze, positionRobot* positionZhonx, char xFinish,
		char yFinish)
{
	coordinate way[MAZE_SIZE*MAZE_SIZE] = {0};
	motorsSleepDriver (OFF);
	telemetersStart();
	HAL_Delay(1000);
	newCell (getCellState(), maze, *positionZhonx);
	telemetersStart();

	while (positionZhonx->x != xFinish || positionZhonx->y != yFinish)
	{
		clearMazelength (maze);
		poids (maze, xFinish, yFinish, true);
		moveVirtualZhonx (*maze, *positionZhonx, way, xFinish, yFinish);
		moveRealZhonxArc (maze, positionZhonx, way);//, &xFinish, &yFinish);
	}
	telemetersStop();
	HAL_Delay (200);
	motorsSleepDriver (ON);

}



void moveVirtualZhonx(labyrinthe maze, positionRobot positionZhonxVirtuel,
		coordinate way[], char xFinish, char yFinish)
{
	int i=0;
	while (positionZhonxVirtuel.x != xFinish
			|| positionZhonxVirtuel.y != yFinish)
	{
		if (maze.cell[(int) (positionZhonxVirtuel.x + 1)][(int) (positionZhonxVirtuel.y)].length + 1 == maze.cell[(int) (positionZhonxVirtuel.x)][(int) (positionZhonxVirtuel.y)].length && positionZhonxVirtuel.x+1<MAZE_SIZE && maze.cell[(int)(positionZhonxVirtuel.x)][(int)(positionZhonxVirtuel.y)].wall_east==NO_WALL)
		{
			positionZhonxVirtuel.x = positionZhonxVirtuel.x + 1;
		}
		else if (maze.cell[(int) (positionZhonxVirtuel.x)][(int) (positionZhonxVirtuel.y + 1)].length + 1 == maze.cell[(int) (positionZhonxVirtuel.x)][(int) (positionZhonxVirtuel.y)].length && positionZhonxVirtuel.y+1<MAZE_SIZE && maze.cell[(int)(positionZhonxVirtuel.x)][(int)(positionZhonxVirtuel.y)].wall_south==NO_WALL)
		{
			positionZhonxVirtuel.y = positionZhonxVirtuel.y + 1;
		}
		else if (maze.cell[(int) (positionZhonxVirtuel.x - 1)][(int) (positionZhonxVirtuel.y)].length + 1 == maze.cell[(int) (positionZhonxVirtuel.x)][(int) (positionZhonxVirtuel.y)].length && positionZhonxVirtuel.x>0 && maze.cell[(int)(positionZhonxVirtuel.x)][(int)(positionZhonxVirtuel.y)].wall_west==NO_WALL)
		{
			positionZhonxVirtuel.x = positionZhonxVirtuel.x - 1;
		}
		else if (maze.cell[(int) (positionZhonxVirtuel.x)][(int) (positionZhonxVirtuel.y - 1)].length + 1 == maze.cell[(int) (positionZhonxVirtuel.x)][(int) (positionZhonxVirtuel.y)].length && positionZhonxVirtuel.y>0 && maze.cell[(int)(positionZhonxVirtuel.x)][(int)(positionZhonxVirtuel.y)].wall_north==NO_WALL)
		{
			positionZhonxVirtuel.y = positionZhonxVirtuel.y - 1;
		}
		else
		{
			if (i != 0)
			{
				way[i].x = END_OF_LIST, way[i].y = END_OF_LIST;
				return;
			}
			else
			{
				ssd1306DrawString (60, 0, "no solution", &Font_5x8);
				ssd1306Refresh ();
				motorsSleepDriver (ON);
				while (true)
				{
					HAL_Delay (200);
				}
			}
		}

		printMaze(maze,positionZhonxVirtuel.x,positionZhonxVirtuel.y);
		ssd1306Refresh ();
		way[i].x = positionZhonxVirtuel.x, way[i].y = positionZhonxVirtuel.y;
		i++;
	}
	way[i].x = END_OF_LIST, way[i].y = END_OF_LIST;
	return;
}


void moveRealZhonxArc(labyrinthe *maze, positionRobot *positionZhonx, coordinate way[])
{
	walls cell_state;
	char endMidCase;
	char chain;
	int length;
	int i = 0;
	char additionY = 0;
	char additionX = 0;
	char orientaionToGo = NORTH;
	while (way[i].x != END_OF_LIST)
	{
		length = 0;
		if (way[i].x == (positionZhonx->x + 1) && way[i].y == positionZhonx->y)
		{
			additionX = 1;
			additionY = 0;
			orientaionToGo = EAST;
		}
		else if (way[i].x == (positionZhonx->x - 1) && way[i].y == positionZhonx->y)
		{
			additionX = -1;
			additionY = 0;
			orientaionToGo = WEST;
		}
		else if (way[i].y == (positionZhonx->y - 1) && way[i].x == positionZhonx->x)
		{

			additionX = 0;
			additionY = -1;
			orientaionToGo = NORTH;
		}
		else if (way[i].y == (positionZhonx->y + 1) && way[i].x == positionZhonx->x)
		{

			additionX = 0;
			additionY = 1;
			orientaionToGo = SOUTH;
		}
		else
		{
			bluetoothPrintf("Error way : position zhonx x= %d y=%d \t way x= %d y=%d \n",positionZhonx->x,positionZhonx->y, way[i].x, way[i].y);
			HAL_Delay (200);
			motorsSleepDriver (ON);
			ssd1306DrawString (60, 0, "Error way", &Font_5x8);
			ssd1306Refresh ();
			while (1)
			{
				HAL_Delay (500);
			}
		}

		while ((way[i].x != END_OF_LIST) && way[i].y == (positionZhonx->y + additionY)
				&& way[i].x == positionZhonx->x + additionX)
		{
			positionZhonx->x = way[i].x;
			positionZhonx->y = way[i].y;
			i++;
			length++;
		}
		if ((positionZhonx->x != 8 || positionZhonx->y != 8) )
			endMidCase = false;
		else
			endMidCase = true;
		if (way == NULL)
			chain = false;
		else
			chain = true;
		move_zhonx_arc (orientaionToGo, positionZhonx, length, endMidCase, chain);
		cell_state = getCellState ();
		newCell (cell_state, maze, *positionZhonx);

	}
}



void poids(labyrinthe *maze, int xFinish, int yFinish, char wallNoKnow)
{
	int i1 = 0, i2 = 0 ;
	int length = 0;
	int x = xFinish;
	int y = yFinish;
	maze->cell[x][y].length = length;
	coordinate dotes_to_verifie_tab [MAZE_SIZE*MAZE_SIZE];
	dotes_to_verifie_tab[0].x = x;
	dotes_to_verifie_tab[0].y = y;
	dotes_to_verifie_tab[1].x = END_OF_LIST;
	coordinate new_dotes_to_verifie_tab [MAZE_SIZE*MAZE_SIZE];
	coordinate *dotes_to_verifie = dotes_to_verifie_tab;
	coordinate *new_dotes_to_verifie = new_dotes_to_verifie_tab;
	coordinate *pt = NULL;

	while (dotes_to_verifie[0].x != END_OF_LIST)
	{
		length++;
		while (dotes_to_verifie[i1].x != END_OF_LIST)
		{
			x = dotes_to_verifie[i1].x;
			y = dotes_to_verifie[i1].y;
			if ((maze->cell[x][y].wall_north == NO_WALL
					|| (wallNoKnow == true
							&& maze->cell[x][y].wall_north == NO_KNOWN))
					&& maze->cell[x][y - 1].length > length - 1 && y > 0)
			{
				new_dotes_to_verifie[i2].x = x;
				new_dotes_to_verifie[i2].y = y-1;
				i2++;
				maze->cell[x][y - 1].length = length;
			}
			if ((maze->cell[x][y].wall_east == NO_WALL
					|| (wallNoKnow == true
							&& maze->cell[x][y].wall_east == NO_KNOWN))
					&& maze->cell[x + 1][y].length > length&& x+1<MAZE_SIZE)
			{
				new_dotes_to_verifie[i2].x = x+1;
				new_dotes_to_verifie[i2].y = y;
				i2++;
				maze->cell[x + 1][y].length = length;
			}
			if ((maze->cell[x][y].wall_south == NO_WALL
					|| (wallNoKnow == true
							&& maze->cell[x][y].wall_south == NO_KNOWN))
					&& maze->cell[x][y + 1].length > length&& y+1<MAZE_SIZE)
			{
				new_dotes_to_verifie[i2].x = x;
				new_dotes_to_verifie[i2].y = y+1;
				i2++;
				maze->cell[x][y + 1].length = length;
			}
			if ((maze->cell[x][y].wall_west == NO_WALL
					|| (wallNoKnow == true
							&& maze->cell[x][y].wall_west == NO_KNOWN))
					&& maze->cell[x - 1][y].length > length && x > 0)
			{
				new_dotes_to_verifie[i2].x = x-1;
				new_dotes_to_verifie[i2].y = y;
				i2++;
				maze->cell[x - 1][y].length = length;
			}
			i1++;
		}
		new_dotes_to_verifie[i2].x = END_OF_LIST;
		pt = dotes_to_verifie;
		dotes_to_verifie = new_dotes_to_verifie;
		new_dotes_to_verifie = pt;
		i2=0;
		i1=0;
	}
}

//void newDot(coordinate **old_dot, int x, int y)
//{
//	if ((*old_dot) != NULL)
//	{
//		(*old_dot)->next = (coordinate*)calloc_s (1, sizeof(coordinate));
//		coordinate *pt = *old_dot;
//		*old_dot = pt->next;
//		(*old_dot)->previous = pt;
//	}
//	else
//	{
//		(*old_dot) = (coordinate*) calloc_s (1, sizeof(coordinate));
//		(*old_dot)->previous = NULL;
//	}
//	(*old_dot)->x = x;
//	(*old_dot)->y = y;
//	(*old_dot)->next = NULL;
//}
void mazeInit(labyrinthe *maze)
{
#ifndef test
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		for (int y = 0; y < MAZE_SIZE; y++)
		{
			maze->cell[i][y].wall_north = NO_KNOWN;
			maze->cell[i][y].wall_west = NO_KNOWN;
			maze->cell[i][y].wall_south = NO_KNOWN;
			maze->cell[i][y].wall_east = NO_KNOWN;
			maze->cell[i][y].length = 2000;
		}
	}
	for (int i = 0; i < 16; i++)
	{
		maze->cell[i][0].wall_north = WALL_PRESENCE;
		maze->cell[i][MAZE_SIZE - 1].wall_south = WALL_PRESENCE;
		maze->cell[0][i].wall_west = WALL_PRESENCE;
		maze->cell[MAZE_SIZE - 1][i].wall_east = WALL_PRESENCE;
	}
#else
	labyrinthe maze_initial=
	{
		{
			{
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}},
			{
				{	WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},
				{	NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000}}}};

	*maze=maze_initial;
#endif
}

void printMaze(const labyrinthe maze, const int x_robot, const int y_robot)
{
#ifdef DEBUG
	ssd1306ClearRect(0,0,64,64);
	int size_cell_on_oled = ((63) / MAZE_SIZE);
	int x, y;
	for (y = 0; y < MAZE_SIZE; y++)
	{
		for (x = 0; x < MAZE_SIZE; x++)
		{
			if (maze.cell[x][y].wall_north == WALL_PRESENCE)
			{
				ssd1306DrawLine (x * size_cell_on_oled, y * size_cell_on_oled,
		x * size_cell_on_oled + size_cell_on_oled + 1, y * size_cell_on_oled);
			}
			else if (maze.cell[x][y].wall_north == NO_KNOWN)
			{
//                ssd1306DrawLine (x * size_cell_on_oled, y * size_cell_on_oled,
//                x * size_cell_on_oled + size_cell_on_oled + 1, y * size_cell_on_oled);
				ssd1306DrawDashedLine (x * size_cell_on_oled, y * size_cell_on_oled,
			   x * size_cell_on_oled + size_cell_on_oled + 1, y * size_cell_on_oled);
			}
			if (maze.cell[x][y].wall_west == WALL_PRESENCE)
			{
				ssd1306DrawLine (x * size_cell_on_oled, y * size_cell_on_oled,
								 x * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
			}
			else if (maze.cell[x][y].wall_west == NO_KNOWN)
			{
//				ssd1306DrawLine (x * size_cell_on_oled, y * size_cell_on_oled,
//								 x * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
				ssd1306DrawDashedLine (x * size_cell_on_oled, y * size_cell_on_oled,
									   x * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
			}

			if (maze.cell[x][y].wall_south == WALL_PRESENCE)
			{
				ssd1306DrawLine (x * size_cell_on_oled,(y + 1) * size_cell_on_oled,
			 size_cell_on_oled + x * size_cell_on_oled,(y + 1) * size_cell_on_oled);
			}
			else if (maze.cell[x][y].wall_south == NO_KNOWN)
			{
//				ssd1306DrawLine (x * size_cell_on_oled,(y + 1) * size_cell_on_oled,
//			 size_cell_on_oled + x * size_cell_on_oled,(y + 1) * size_cell_on_oled);
				ssd1306DrawDashedLine (x * size_cell_on_oled,(y + 1) * size_cell_on_oled,
				   size_cell_on_oled + x * size_cell_on_oled,(y + 1) * size_cell_on_oled);
			}
			if (maze.cell[x][y].wall_east == WALL_PRESENCE)
			{
				ssd1306DrawLine ((x + 1) * size_cell_on_oled, y * size_cell_on_oled,
								 (x + 1) * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
			}
			else if (maze.cell[x][y].wall_east == NO_KNOWN)
			{
//				ssd1306DrawLine ((x + 1) * size_cell_on_oled, y * size_cell_on_oled,
//								 (x + 1) * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
				ssd1306DrawDashedLine  ((x + 1) * size_cell_on_oled, y * size_cell_on_oled,
										(x + 1) * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
			}
		}
	}
	printLength(maze, x_robot, y_robot);
	ssd1306DrawRect((x_robot * size_cell_on_oled) +1, (y_robot * size_cell_on_oled)+1, 2, 2);
	ssd1306Refresh ();
#endif
}

void* calloc_s(size_t nombre, size_t taille)
{
	void* pt = calloc (nombre, taille);
	if (pt == NULL)
	{
		printf ("null pointer exception, full memory");
		while (1)
		{
			HAL_Delay(500);
		}
	}
	return pt;
}

void printLength(const labyrinthe maze,const int x_robot, const int y_robot)
{
#if DEBUG > 2
    bluetoothPrintf ("zhonx : %d; %d\n", x_robot, y_robot);
	bluetoothPrintf ("  ");
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		bluetoothPrintf ("%5d", i);
	}
	bluetoothPrintf ("\n\n");
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		bluetoothPrintf ("%2d ", i);
		for (int j = 0; j < MAZE_SIZE; j++)
		{
			if (maze.cell[j][i].wall_north == NO_KNOWN)
			{
				bluetoothPrintf("= = *");
			}
			else if (maze.cell[j][i].wall_north == WALL_PRESENCE)
			{
				bluetoothPrintf("====*");
			}
			else
			{
				bluetoothPrintf ("    *");
			}
		}
		bluetoothPrintf ("\n   ");
		for (int j = 0; j < MAZE_SIZE; j++)
		{
			bluetoothPrintf ("%4d", maze.cell[j][i].length);
			if (maze.cell[j][i].wall_east == NO_KNOWN)
			{
				bluetoothPrintf ("!");
			}
			else if (maze.cell[j][i].wall_east == WALL_PRESENCE)
			{
				bluetoothPrintf ("|");
			}
			else
			{
				bluetoothPrintf (" ");
			}
		}
		bluetoothPrintf ("\n");
	}
	bluetoothPrintf ("\n");
#endif
}

void clearMazelength(labyrinthe* maze)
{
	int x, y;
	for (y = 0; y < MAZE_SIZE; y++)
	{
		for (x = 0; x < MAZE_SIZE; x++)
		{
			maze->cell[x][y].length = 2000;
		}
	}
}

char miniwayFind(labyrinthe *maze, char xStart, char yStart, char xFinish,
		char yFinish)
{
	// TODO not find the shorter in distance way but the faster
	coordinate way1[MAZE_SIZE*MAZE_SIZE];
	coordinate way2[MAZE_SIZE*MAZE_SIZE];
	clearMazelength (maze);
	poids (maze, xFinish, yFinish, true);
	printMaze(*maze,-1,-1);
	positionRobot position;
	position.midOfCell = true;
	position.x = xStart;
	position.y = yStart;
	position.orientation = NORTH;
	moveVirtualZhonx (*maze, position, way1, xFinish, yFinish);
	clearMazelength (maze);
	poids (maze, xFinish, yFinish, false);
	printMaze(*maze,-1,-1);
	moveVirtualZhonx (*maze, position, way2, xFinish, yFinish);
	ssd1306ClearScreen ();
	char waySame = diffway (way1, way2);
	switch (waySame)
	{
		case true :
			ssd1306DrawString (0, 20, "2 way = : yes", &Font_5x8);
			break;
		case false :
			ssd1306DrawString (0, 20, "2 way = : no", &Font_5x8);
			break;
	}
	ssd1306Refresh ();
	HAL_Delay (3000);
	return (waySame);
}

char diffway(coordinate way1[], coordinate way2[])
{
	int i = 0;
	while ((way1[i].x != END_OF_LIST) && (way2[i].x != END_OF_LIST))
	{
		if (way1[i].x != way2[i].x || way1[i].y != way2[i].y)
		{
			return false;
		}
		i++;
	}
	if (!NAND((way1[i].x != END_OF_LIST),(way2[i].x != END_OF_LIST)))
	{
		return false;
	}
	return true;
}

void waitStart()
{
	ssd1306ClearRect(SSD1306_LCDWIDTH/2,0,SSD1306_LCDWIDTH/2,SSD1306_LCDHEIGHT);
	ssd1306Printf(SSD1306_LCDWIDTH/2,0,&Font_5x8,"wait start");
	ssd1306Refresh();
	//while (expanderJoyFiltered() != JOY_RIGHT)
	{
		HAL_Delay (20);
	}
	ssd1306ClearRect(SSD1306_LCDWIDTH/2,0,SSD1306_LCDWIDTH/2,SSD1306_LCDHEIGHT);
	ssd1306Refresh();
//TODO : wait start with front sensors
//	unsigned char sensors_state = hal_sensor_get_state(app_context.sensors);
//	while(check_bit(sensors_state, SENSOR_F10_POS)==true)
//		sensors_state = hal_sensor_get_state(app_context.sensors);
//	HAL_Delay(200);
//	while(check_bit(sensors_state, SENSOR_F10_POS)==false)
//		sensors_state = hal_sensor_get_state(app_context.sensors);
}
