
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "middleware/settings/settings.h"

#ifdef zhonx3

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "config/basetypes.h"

/* peripherale inlcudes*/
#include "peripherals/times_base/times_base.h"
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
#include "middleware/controls/motionControl/wallFollowControl.h"
#include "middleware/controls/motionControl/mainControl.h"

/*application include */
#include "application/solverMaze/solverMaze.h"
#include "application/solverMaze/robotInterface.h"
#include "application/solverMaze/run.h"
#endif
#ifdef zhonx2
#include "stm32f4xx.h"
#include "config/basetypes.h"
#include "config/config.h"
#include "oled/ssd1306.h"

#include "stm32f4xx.h"
#include "config/basetypes.h"
#include "config/config.h"
#include "oled/ssd1306.h"

/* peripherale inlcudes*/

/* meddleware include */
#include "hal/hal_sensor.h"
#include "hal/hal_os.h"
#include "hal/hal_step_motor.h"
#include "app/app_def.h"

/*application include */
#include "app/solverMaze/solverMaze.h"
#include "app/solverMaze/robotInterface.h"
#include "app/solverMaze/run.h"
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
#endif // simulator
#define debug
#ifdef debug
#undef END_SPEED_TRANSLATION
#define END_SPEED_TRANSLATION 0
#endif
int maze(void)
{
	coordinate start_coordinate, end_coordinate; // it's the coordinates which Zhonx have at the start
	labyrinthe maze;
	mazeInit (&maze);
	positionRobot positionZhonx;
	#ifdef ZHONX3
		mainControlInit();
		telemetersStart();
		move(0, 0, 0, 0);
		HAL_Delay(500);
	#endif
#ifdef simulator
	pt_zhonx_position=&positionZhonx;
#endif // simulator

	/*init for different micromouse competition*/

	positionZhonx.cordinate.x = 8;
	positionZhonx.cordinate.y = 8; // the robot start in the corner
	positionZhonx.orientation = NORTH;
	/*end of initialization for different micromouse competition*/
	positionZhonx.midOfCell = false;
	start_coordinate.x = positionZhonx.cordinate.x;
	start_coordinate.y = positionZhonx.cordinate.y;
	positionZhonx.orientation=(positionZhonx.orientation+2)%4;
	newCell ((walls) {WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE}, &maze, positionZhonx);
	positionZhonx.orientation=(positionZhonx.orientation+2)%4;
	positionZhonx.midOfCell = true;
	printMaze (maze, positionZhonx.cordinate);
	#ifdef ZHONX3
		move (0, -CELL_LENGTH/2, 50, 0);
		while(isEndMove() != true);
		motorsSleepDriver(ON);
	#endif
	#ifdef ZHONX2
		hal_step_motor_enable();
	#endif
	waitStart ();
	exploration (&maze, &positionZhonx, &end_coordinate);
	HAL_Delay(2000);
	goToPosition (&maze, &positionZhonx, start_coordinate);

	doUTurn (&positionZhonx);
	HAL_Delay(2000);
	#ifdef ZHONX3
	move (0, -CELL_LENGTH/2, 50, 0);
	#endif
	#ifdef ZHONX2
	if (zhonx_settings.calibration_enabled==true)
	{
		hal_os_sleep(1000);
		calibrateSimple();
	}
	#endif
	#ifdef ZHONX3
		motorsSleepDriver(OFF);
	#endif
	#ifdef ZHONX2
		hal_step_motor_disable();
	#endif
	run1 (&maze, &positionZhonx, start_coordinate, end_coordinate);
	run2 (&maze, &positionZhonx, start_coordinate, end_coordinate);
	#ifdef ZHONX3
		motorsSleepDriver(OFF);
	#endif
	#ifdef ZHONX2
		hal_step_motor_disable();
	#endif
	return MAZE_SOLVER_E_SUCCESS;
}

void exploration(labyrinthe *maze, positionRobot* positionZhonx,  coordinate *end_coordinate)
{
	positionRobot start_position;
	int rv = MAZE_SOLVER_E_SUCCESS;
	coordinate way[MAZE_SIZE*MAZE_SIZE] = {{-1,-1},{END_OF_LIST,END_OF_LIST}};
	coordinate last_coordinate;
	start_position = *positionZhonx;
	poids (maze, start_position.cordinate, true, false);

	while ( findArrival(*maze, end_coordinate) != MAZE_SOLVER_E_SUCCESS)
	{
		newCell (getCellState(), maze, *positionZhonx);
		clearMazelength (maze);
		poids (maze, *end_coordinate, true, true);
		rv = moveVirtualZhonx (*maze, *positionZhonx, way, *end_coordinate);
		if (rv != MAZE_SOLVER_E_SUCCESS)
		{
			ssd1306Printf(60,20, &Font_5x8,"no solution");
			bluetoothPrintf("no solution");
			ssd1306Refresh();
			printLength(*maze,-1,-1);
			while (1)
			{
				HAL_Delay (5000);
			}
		}
		moveRealZhonxArc(maze, positionZhonx, way);
		clearMazelength(maze);
		poids (maze, positionZhonx->cordinate, true, false);
		printLength(*maze, positionZhonx->cordinate.x, positionZhonx->cordinate.y);
	}
	#ifdef ZHONX2
		if (zhonx_settings.calibration_enabled==true)
		{
			hal_os_sleep(1000);
			calibrateSimple();
		}
	#endif
	last_coordinate = findEndCoordinate(way);
	do
	{
		clearMazelength (maze);
		poids (maze, *end_coordinate, true, false);
		rv = moveVirtualZhonx (*maze, start_position, way, *end_coordinate);
		if (rv == MAZE_SOLVER_E_SUCCESS)
		{
			last_coordinate=findEndCoordinate(way);
			if(last_coordinate.x == end_coordinate->x && last_coordinate.y == end_coordinate->y )
				break;
			goToPosition(maze,positionZhonx,last_coordinate);
		}
	} while ((last_coordinate.x != end_coordinate->x) || (last_coordinate.y != end_coordinate->y));
	#ifdef ZHONX2
		if (zhonx_settings.calibration_enabled==true)
			{
				hal_os_sleep(1000);
				calibrateSimple();
			}
	#endif
}
int goToPosition(labyrinthe *maze, positionRobot* positionZhonx,  coordinate end_coordinate)
{
	coordinate way[MAZE_SIZE*MAZE_SIZE];
	int rv;
	newCell (getCellState(), maze, *positionZhonx);
	while (positionZhonx->cordinate.x != end_coordinate.x || positionZhonx->cordinate.y != end_coordinate.y)
	{
		clearMazelength (maze);
		poids (maze, end_coordinate, true, false);
		rv = moveVirtualZhonx (*maze, *positionZhonx, way, end_coordinate);
		if (rv != MAZE_SOLVER_E_SUCCESS)
		{
			// no solution for go to the asked position
			return rv;
		}
		moveRealZhonxArc(maze, positionZhonx, way);
	}
	return MAZE_SOLVER_E_SUCCESS;
}



int moveVirtualZhonx(labyrinthe maze, positionRobot positionZhonxVirtuel,
		coordinate way[], coordinate end_coordinate)
{
	int i=0;
	while (positionZhonxVirtuel.cordinate.x != end_coordinate.x
			|| positionZhonxVirtuel.cordinate.y != end_coordinate.y)
	{
		if (maze.cell[(int) (positionZhonxVirtuel.cordinate.x + 1)][(int) (positionZhonxVirtuel.cordinate.y)].length < maze.cell[(int) (positionZhonxVirtuel.cordinate.x)][(int) (positionZhonxVirtuel.cordinate.y)].length
				&& positionZhonxVirtuel.cordinate.x+1<MAZE_SIZE
				&& maze.cell[(int)(positionZhonxVirtuel.cordinate.x)][(int)(positionZhonxVirtuel.cordinate.y)].wall_east==NO_WALL)
		{
			positionZhonxVirtuel.cordinate.x = positionZhonxVirtuel.cordinate.x + 1;
		}
		else if (maze.cell[(int) (positionZhonxVirtuel.cordinate.x)][(int) (positionZhonxVirtuel.cordinate.y + 1)].length < maze.cell[(int) (positionZhonxVirtuel.cordinate.x)][(int) (positionZhonxVirtuel.cordinate.y)].length
				&& positionZhonxVirtuel.cordinate.y+1<MAZE_SIZE
				&& maze.cell[(int)(positionZhonxVirtuel.cordinate.x)][(int)(positionZhonxVirtuel.cordinate.y)].wall_south==NO_WALL)
		{
			positionZhonxVirtuel.cordinate.y = positionZhonxVirtuel.cordinate.y + 1;
		}
		else if (maze.cell[(int) (positionZhonxVirtuel.cordinate.x - 1)][(int) (positionZhonxVirtuel.cordinate.y)].length < maze.cell[(int) (positionZhonxVirtuel.cordinate.x)][(int) (positionZhonxVirtuel.cordinate.y)].length
				&& positionZhonxVirtuel.cordinate.x>0
				&& maze.cell[(int)(positionZhonxVirtuel.cordinate.x)][(int)(positionZhonxVirtuel.cordinate.y)].wall_west==NO_WALL)
		{
			positionZhonxVirtuel.cordinate.x = positionZhonxVirtuel.cordinate.x - 1;
		}
		else if (maze.cell[(int) (positionZhonxVirtuel.cordinate.x)][(int) (positionZhonxVirtuel.cordinate.y - 1)].length < maze.cell[(int) (positionZhonxVirtuel.cordinate.x)][(int) (positionZhonxVirtuel.cordinate.y)].length
				&& positionZhonxVirtuel.cordinate.y>0
				&& maze.cell[(int)(positionZhonxVirtuel.cordinate.x)][(int)(positionZhonxVirtuel.cordinate.y)].wall_north==NO_WALL)
		{
			positionZhonxVirtuel.cordinate.y = positionZhonxVirtuel.cordinate.y - 1;
		}
		else
		{
			way[i].x = END_OF_LIST, way[i].y = END_OF_LIST;
			if (i != 0)
			{
				return MAZE_SOLVER_E_SUCCESS;
			}
			else
			{
				return MAZE_SOLVER_E_ERROR;
			}
		}

		printMaze(maze,positionZhonxVirtuel.cordinate);
		way[i].x = positionZhonxVirtuel.cordinate.x, way[i].y = positionZhonxVirtuel.cordinate.y;
		i++;
	}
	way[i].x = END_OF_LIST, way[i].y = END_OF_LIST;
	return MAZE_SOLVER_E_SUCCESS;
}


void moveRealZhonxArc(labyrinthe *maze, positionRobot *positionZhonx, coordinate way[])
{
	walls cell_state;
	char chain;
	int length;
	int i = 0;
	char additionY = 0;
	char additionX = 0;
	char orientaionToGo = NORTH;
	while (way[i].x != END_OF_LIST)
	{
		length = 0;
		if (way[i].x == (positionZhonx->cordinate.x + 1) && way[i].y == positionZhonx->cordinate.y)
		{
			additionX = 1;
			additionY = 0;
			orientaionToGo = EAST;
		}
		else if (way[i].x == (positionZhonx->cordinate.x - 1) && way[i].y == positionZhonx->cordinate.y)
		{
			additionX = -1;
			additionY = 0;
			orientaionToGo = WEST;
		}
		else if (way[i].y == (positionZhonx->cordinate.y - 1) && way[i].x == positionZhonx->cordinate.x)
		{

			additionX = 0;
			additionY = -1;
			orientaionToGo = NORTH;
		}
		else if (way[i].y == (positionZhonx->cordinate.y + 1) && way[i].x == positionZhonx->cordinate.x)
		{

			additionX = 0;
			additionY = 1;
			orientaionToGo = SOUTH;
		}
		else
		{
			#ifdef ZHONX3
			bluetoothPrintf("Error way : position zhonx x= %d y=%d \t way x= %d y=%d \n",\
					positionZhonx->cordinate.x,positionZhonx->cordinate.y, way[i].x, way[i].y);
			HAL_Delay (200);
			motorsSleepDriver (ON);
			#endif
			#ifdef ZHONX2
			hal_step_motor_disable();
			#endif
			ssd1306DrawString (60, 0, "Error way", &Font_5x8);
			ssd1306Refresh ();
			while (1)
			{
				HAL_Delay (500);
			}
		}

		while ((way[i].x != END_OF_LIST) && way[i].y == (positionZhonx->cordinate.y + additionY)
				&& way[i].x == positionZhonx->cordinate.x + additionX)
		{
			positionZhonx->cordinate.x = way[i].x;
			positionZhonx->cordinate.y = way[i].y;
			i++;
			length++;
		}
		if (way[i].x == END_OF_LIST)
			chain = false;
		else
			chain = true;
		move_zhonx_arc (orientaionToGo, positionZhonx, length, chain, chain);
		cell_state = getCellState ();
		newCell (cell_state, maze, *positionZhonx);

	}
}



void poids(labyrinthe *maze, coordinate end_coordinate, char wallNoKnow, char contournKnownCell)
{
	int i1 = 0, i2 = 0 ;
	int length = 0;
	int x = end_coordinate.x;
	int y = end_coordinate.y;
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
		while (dotes_to_verifie[i1].x != END_OF_LIST)
		{
			x = dotes_to_verifie[i1].x;
			y = dotes_to_verifie[i1].y;
			int part_length = 0;

			if (!(maze->cell[x][y].wall_north != WALL_PRESENCE)) // todo : do this test only if
			{
				part_length ++;
			}
			if (!(maze->cell[x][y].wall_east != WALL_PRESENCE))
			{
				part_length ++;
			}
			if (!(maze->cell[x][y].wall_south != WALL_PRESENCE))
			{
				part_length ++;
			}
			if (!(maze->cell[x][y].wall_west != WALL_PRESENCE))
			{
				part_length ++;
			}
			maze->cell[x][y].length += part_length;
			length = maze->cell[x][y].length + 5;

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
int numberOfWallInCell (labyrinthe maze, coordinate cellCoordinate)
{
//	if (;)
	return -1;
}
void mazeInit(labyrinthe *maze)
{
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

}

void printMaze(labyrinthe maze, coordinate robot_coordinate)
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
				ssd1306DrawDashedLine  ((x + 1) * size_cell_on_oled, y * size_cell_on_oled,
										(x + 1) * size_cell_on_oled, y * size_cell_on_oled + size_cell_on_oled + 1);
			}
		}
	}
	printLength(maze, robot_coordinate.x, robot_coordinate.y);
	ssd1306DrawRect((robot_coordinate.x * size_cell_on_oled) +1, (robot_coordinate.y * size_cell_on_oled)+1, 2, 2);
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
	bluetoothPrintf ("\n");
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		bluetoothPrintf ("    ");
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
		bluetoothPrintf ("\n ");

		bluetoothPrintf ("%2d ", i);
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
			maze->cell[x][y].length = CANT_GO;
		}
	}
}

char miniwayFind(labyrinthe *maze, coordinate start_coordinate, coordinate end_coordinate)
{
	// TODO not find the shorter in distance way but the faster
	coordinate way1[MAZE_SIZE*MAZE_SIZE];
	coordinate way2[MAZE_SIZE*MAZE_SIZE];
	clearMazelength (maze);
	poids (maze, end_coordinate, true, false);
	printMaze(*maze,(coordinate){-1,-1});
	positionRobot position;
	position.midOfCell = true;
	position.cordinate = start_coordinate;
	position.orientation = NORTH;
	moveVirtualZhonx (*maze, position, way1, end_coordinate);
	clearMazelength (maze);
	poids (maze, end_coordinate, false, false);
	printMaze(*maze,(coordinate){-1,-1});
	moveVirtualZhonx (*maze, position, way2, end_coordinate);
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
	// TODO : move this function in robot interface
	ssd1306ClearRect(SSD1306_LCDWIDTH/2,0,SSD1306_LCDWIDTH/2,SSD1306_LCDHEIGHT);
	ssd1306Printf(SSD1306_LCDWIDTH/2,0,&Font_5x8,"wait start");
	ssd1306Refresh();
	while (expanderJoyFiltered() != JOY_RIGHT)
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
coordinate findEndCoordinate (coordinate coordinate_tab[])
{
	int i=0;
	while(coordinate_tab[i].x != END_OF_LIST)
	{
		i++;
	}
	return coordinate_tab[i-1];
}

int findArrival (labyrinthe maze, coordinate *end_coordinate)
{
	int possible_end_find = CANT_GO;
	for (int x = 0; x< (MAZE_SIZE-1); x++)
	{
		for (int y = 0; y < (MAZE_SIZE-1); ++y)
		{
			if (maze.cell[x][y].wall_east == NO_WALL && maze.cell[x][y].wall_south == NO_WALL
					&& maze.cell[x][y+1].wall_east == NO_WALL && maze.cell[x+1][y].wall_south == NO_WALL
						&& maze.cell[x][y].length != CANT_GO)
			{
				end_coordinate->x = x;
				end_coordinate->y = y;
				bluetoothPrintf("end find at : %i; %i\n", x, y);
				return MAZE_SOLVER_E_SUCCESS;
			}
			if (possible_end_find > maze.cell[x][y].length &&
					maze.cell[x][y].wall_east != WALL_PRESENCE && maze.cell[x][y].wall_south != WALL_PRESENCE
								&& maze.cell[x][y+1].wall_east != WALL_PRESENCE && maze.cell[x+1][y].wall_south != WALL_PRESENCE
									&& maze.cell[x][y].length != CANT_GO
						&& maze.cell[x][y].length != CANT_GO && maze.cell[x+1][y].length != CANT_GO
						&& maze.cell[x][y+1].length != CANT_GO && maze.cell[x+1][y+1].length != CANT_GO)
			{
				bluetoothPrintf("possible end find at : %i; %i\n", x, y);
				if (maze.cell[x][y].wall_east != NO_WALL || maze.cell[x][y].wall_south != NO_WALL)
				{
					end_coordinate->x = x;
					end_coordinate->y = y;
				}
				else if (maze.cell[x+1][y].wall_west != NO_WALL || maze.cell[x+1][y].wall_south != NO_WALL)
				{
					end_coordinate->x = x+1;
					end_coordinate->y = y;
				}
				else if (maze.cell[x][y+1].wall_east != NO_WALL || maze.cell[x][y+1].wall_south != NO_WALL)
				{
					end_coordinate->x = x;
					end_coordinate->y = y+1;
				}
				else if (maze.cell[x+1][y+1].wall_west != NO_WALL || maze.cell[x+1][y+1].wall_south != NO_WALL)
				{
					end_coordinate->x = x+1;
					end_coordinate->y = y+1;
				}
				possible_end_find = maze.cell[end_coordinate->x][end_coordinate->y].length;

			}

		}
	}
	return MAZE_SOLVER_E_ERROR;
}
