/*
 * run.c
 *
 *  Created on: 4 juin 2015
 *      Author: Colin
 */

#include "settings.h"

#ifdef ZHONX3

/* peripherale inlcudes*/
#include "peripherals/motors/motors.h"
#include "peripherals/display/smallfonts.h"
#include "peripherals/display/ssd1306.h"
#include "peripherals/expander/pcf8574.h"
#include "peripherals/telemeters/telemeters.h"
/* meddleware include */

/*application include */
#include "application/solverMaze/solverMaze.h"
#include "application/solverMaze/robotInterface.h"
#include "application/solverMaze/run.h"
#elif defined SIMULATOR
#include "SDL/SDL.h"
#include "solverMaze.h"
#include "robotInterface.h"
#include "robotInterface.h"
#include "ssd1306.h"
#include "pcf8574.h"
#endif // simulator
void run1(labyrinthe *maze, positionRobot *positionZhonx, coordinate start_oordinate, coordinate end_coordinate)
{
	coordinate way[MAZE_SIZE*MAZE_SIZE];
	char choice;
	do
	{
		choice = -1;
		clearMazelength(maze);
		poids(maze, end_coordinate, false, false);
		moveVirtualZhonx(*maze, *positionZhonx, way, end_coordinate);
		waitStart ();
		moveRealZhonxArc(maze, positionZhonx, way);
		goToPosition(maze,positionZhonx,start_oordinate);
		doUTurn (positionZhonx);

		ssd1306ClearScreen ();
		ssd1306DrawString (10, 10, "presse \"RIGHT\" to ", &Font_5x8);
		ssd1306DrawString (10, 18, "do a new run 1", &Font_5x8);
		ssd1306Refresh ();
		while (choice == -1)
		{
			if (expanderJoyFiltered () == JOY_RIGHT)
			{
				choice = 1;
			}

			if (expanderJoyFiltered () != JOY_RIGHT
					&& expanderJoyFiltered () != 0)
			{
				choice = 0;
			}
		}
	}while (choice == 1);
}
void run2(labyrinthe *maze, positionRobot *positionZhonx, coordinate start_oordinate, coordinate end_coordinate)
{
	coordinate way[MAZE_SIZE*MAZE_SIZE];
	char choice;
	do
	{
		choice = -1;
		clearMazelength(maze);
		poids(maze,zhonxSettings.maze_end_coordinate, false, false);
		printMaze(*maze,positionZhonx->coordinate_robot);
		waitStart ();
		moveVirtualZhonx (*maze, *positionZhonx, way, end_coordinate);
		moveRealZhonxArc (maze, positionZhonx, way);
//		if (zhonxSettings.calibration_enabled == true)
//			calibrateSimple ();
		HAL_Delay (2000);
		//exploration (maze, positionZhonx, start_oordinate);
//		if (zhonxSettings.calibration_enabled == true)
//			calibrateSimple ();
		doUTurn (positionZhonx);
		ssd1306ClearScreen ();
		ssd1306DrawString (10, 10, "presse \"RIGHT\" to ", &Font_5x8);
		ssd1306DrawString (10, 18, "do a new run 2", &Font_5x8);
		ssd1306Refresh ();
		while (choice == -1)
		{
			if (expanderJoyFiltered () == JOY_RIGHT)
			{
				choice = 1;
			}

			if (expanderJoyFiltered () != JOY_RIGHT
					&& expanderJoyFiltered () != 0)
			{
				choice = 0;
			}
		}
	}while (choice == 1);
}
