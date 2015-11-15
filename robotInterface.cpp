#include "solverMaze.h"
#include "ssd1306.h"
#include "SDL/SDL.h"
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
positionRobot *pt_zhonx_position;
labyrinthe maze_to_discovert = {{{{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},},{{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},},{{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000}}}};

void goOrientation(char *orientationZhonx, char directionToGo)
{
	*orientationZhonx= directionToGo;
    return;
}
void doUTurn(positionRobot *positionZhonx)
{
    return;
}
void moveZhonxArc (int direction_to_go, positionRobot *positionZhonx, int numberOfCase, char end_mid_of_case, char chain)
{
	positionZhonx->orientation = direction_to_go;
}
int floorSensorCalibrate(void)
{
    return 1;
}
int waitValidation(unsigned long timeout)
{
    return 1;
}
void newCell(walls new_walls, labyrinthe *maze, positionRobot positionZhonx)
{
    #if DEBUG > 1
	/*print walls position*/
	static char i=1;
	i++;
	ssd1306ClearRect(64,0,64,64);
	if (new_walls.front == WALL_PRESENCE)
	{
		ssd1306FillRect(64,0,54,5);
	}
	if (new_walls.left == WALL_PRESENCE)
	{
		ssd1306FillRect(64,0,5,54);
	}
	if (new_walls.right == WALL_PRESENCE)
	{
		ssd1306FillRect(113,0,5,54);
	}
	ssd1306Refresh();
	/*end print wall position*/
#endif
	switch (positionZhonx.orientation)
	{
		case NORTH :
			if(positionZhonx.midOfCell == false)
			{
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_east = new_walls.right;
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_west = new_walls.left;

				if (positionZhonx.cordinate.x < (MAZE_SIZE - 1))
					maze->cell[(int) (positionZhonx.cordinate.x + 1)][(int) (positionZhonx.cordinate.y)].wall_west = new_walls.right;
				if (positionZhonx.cordinate.x > 0)
					maze->cell[(int) (positionZhonx.cordinate.x - 1)][(int) (positionZhonx.cordinate.y)].wall_east = new_walls.left;
			}
			if (positionZhonx.cordinate.y > 0)
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y - 1)].wall_south = new_walls.front;

			maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_north = new_walls.front;
			break;

		case EAST :

				if(positionZhonx.midOfCell == false)
				{
					maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_south = new_walls.right;
					maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_north = new_walls.left;

					if (positionZhonx.cordinate.y < (MAZE_SIZE - 1))
						maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y + 1)].wall_north = new_walls.right;
					if (positionZhonx.cordinate.y > 0)
						maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y - 1)].wall_south = new_walls.left;

				}
			if (positionZhonx.cordinate.x < (MAZE_SIZE - 1) )
				maze->cell[(int) (positionZhonx.cordinate.x + 1)][(int) (positionZhonx.cordinate.y)].wall_west = new_walls.front;
			maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_east = new_walls.front;
			break;

		case SOUTH :

			if(positionZhonx.midOfCell == false)
			{
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_west = new_walls.right;
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_east = new_walls.left;

				if (positionZhonx.cordinate.x > 0)
					maze->cell[(int) (positionZhonx.cordinate.x - 1)][(int) (positionZhonx.cordinate.y)].wall_east = new_walls.right;
				if (positionZhonx.cordinate.x < (MAZE_SIZE - 1))
					maze->cell[(int) (positionZhonx.cordinate.x + 1)][(int) (positionZhonx.cordinate.y)].wall_west = new_walls.left;
			}
			if (positionZhonx.cordinate.y < (MAZE_SIZE -1))
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y + 1)].wall_north = new_walls.front;
			maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_south =
					new_walls.front;
			break;

		case WEST :
			if(positionZhonx.midOfCell == false)
			{
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_north = new_walls.right;
				maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_south = new_walls.left;


				if (positionZhonx.cordinate.y > 0)
					maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y - 1)].wall_south = new_walls.right;
				if (positionZhonx.cordinate.y < (MAZE_SIZE - 1))
					maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y + 1)].wall_north = new_walls.left;
			}
			if (positionZhonx.cordinate.x > 0)
				maze->cell[(int) (positionZhonx.cordinate.x - 1)][(int) (positionZhonx.cordinate.y)].wall_east = new_walls.front;
			maze->cell[(int) (positionZhonx.cordinate.x)][(int) (positionZhonx.cordinate.y)].wall_west = new_walls.front;
			break;
	}
}
walls getCellState (void)
{
    walls returnValue;
	switch (pt_zhonx_position->orientation)
	{
		case NORTH:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_north;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_east;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_west;
			break;
		case EAST:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_east;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_south;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_north;
			break;
		case SOUTH:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_south;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_west;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_east;
			break;
		case WEST:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_west;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_north;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->cordinate.x][pt_zhonx_position->cordinate.y].wall_south;
			break;
		default:
			break;
	}
    return returnValue;
}
void move_zhonx_arc (int direction_to_go, positionRobot *positionZhonx, int numberOfCell, char end_mid_of_case, char chain)
{
	positionZhonx->midOfCell=end_mid_of_case;
	positionZhonx->orientation = direction_to_go;
    return;
}

void telemetersStop()
{
    return;
}
void telemetersStart()
{
    return;
}
void telemetersInit()
{
    return;
}
void mainControlInit ()
{
    return;
}
void motorsSleepDriver (char is_on)
{
    return;
}
int move(float angle, float radius_or_distance, float max_speed, float end_speed)
{
    return 1;
}
int rotate90WithCal(int rotation_type, float max_speed, float end_speed)
{
    return 1;
}

bool isEndMove()
{
    return true;
}
