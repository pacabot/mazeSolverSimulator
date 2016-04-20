/*
 * @file : robotInterface.c
 * @author : Colin
 * @version : V 2.0
 * @date : 4 juin 2015
 * @brief : this file contain all the necessary stuff for make interface
 *          between the maze solver and the robot move function.
 */


#include "solverMaze.h"
#include "ssd1306.h"
#include "pcf8574.h"
#include "settings.h"
#include "SDL/SDL.h"
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
positionRobot *pt_zhonx_position;
labyrinthe maze_to_discovert = {{{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},},{{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000}}}};
void goOrientation(char *orientationZhonx, char directionToGo)
{
	*orientationZhonx= directionToGo;
    return;
}
void doUTurn(positionRobot *positionZhonx)
{
    return;
}
void move_zhonx (int direction_to_go, positionRobot *positionZhonx, int numberOfCase, char end_mid_of_case, char chain)
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
#ifdef DEBUG
	print_cell_state(new_walls);
	ssd1306Refresh();
	/*end print wall position*/
#endif
    switch (positionZhonx.orientation)
    {
        case NORTH:
            if (positionZhonx.midOfCell == false)
            {
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_east =
                        new_walls.right;
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_west =
                        new_walls.left;

                if (positionZhonx.coordinate_robot.x < (MAZE_SIZE - 1))
                    maze->cell[(int) (positionZhonx.coordinate_robot.x + 1)][(int) (positionZhonx.coordinate_robot.y)].wall_west =
                            new_walls.right;
                if (positionZhonx.coordinate_robot.x > 0)
                    maze->cell[(int) (positionZhonx.coordinate_robot.x - 1)][(int) (positionZhonx.coordinate_robot.y)].wall_east =
                            new_walls.left;
            }
            if (positionZhonx.coordinate_robot.y > 0)
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y - 1)].wall_south =
                        new_walls.front;

            maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_north =
                    new_walls.front;
            break;

        case EAST:

            if (positionZhonx.midOfCell == false)
            {
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_south =
                        new_walls.right;
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_north =
                        new_walls.left;

                if (positionZhonx.coordinate_robot.y < (MAZE_SIZE - 1))
                    maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y + 1)].wall_north =
                            new_walls.right;
                if (positionZhonx.coordinate_robot.y > 0)
                    maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y - 1)].wall_south =
                            new_walls.left;

            }
            if (positionZhonx.coordinate_robot.x < (MAZE_SIZE - 1))
                maze->cell[(int) (positionZhonx.coordinate_robot.x + 1)][(int) (positionZhonx.coordinate_robot.y)].wall_west =
                        new_walls.front;
            maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_east =
                    new_walls.front;
            break;

        case SOUTH:

            if (positionZhonx.midOfCell == false)
            {
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_west =
                        new_walls.right;
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_east =
                        new_walls.left;

                if (positionZhonx.coordinate_robot.x > 0)
                    maze->cell[(int) (positionZhonx.coordinate_robot.x - 1)][(int) (positionZhonx.coordinate_robot.y)].wall_east =
                            new_walls.right;
                if (positionZhonx.coordinate_robot.x < (MAZE_SIZE - 1))
                    maze->cell[(int) (positionZhonx.coordinate_robot.x + 1)][(int) (positionZhonx.coordinate_robot.y)].wall_west =
                            new_walls.left;
            }
            if (positionZhonx.coordinate_robot.y > 0)
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y + 1)].wall_north =
                        new_walls.front;
            maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_south =
                    new_walls.front;
            break;

        case WEST:
            if (positionZhonx.midOfCell == false)
            {
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_north =
                        new_walls.right;
                maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_south =
                        new_walls.left;

                if (positionZhonx.coordinate_robot.y > 0)
                    maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y - 1)].wall_south =
                            new_walls.right;
                if (positionZhonx.coordinate_robot.y < (MAZE_SIZE - 1))
                    maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y + 1)].wall_north =
                            new_walls.left;
            }
            if (positionZhonx.coordinate_robot.x > 0)
                maze->cell[(int) (positionZhonx.coordinate_robot.x - 1)][(int) (positionZhonx.coordinate_robot.y)].wall_east =
                        new_walls.front;
            maze->cell[(int) (positionZhonx.coordinate_robot.x)][(int) (positionZhonx.coordinate_robot.y)].wall_west =
                    new_walls.front;
            break;
    }
}
walls getCellState (void)
{
    walls returnValue;
	switch (pt_zhonx_position->orientation)
	{
		case NORTH:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_north;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_east;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_west;
			break;
		case EAST:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_east;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_south;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_north;
			break;
		case SOUTH:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_south;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_west;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_east;
			break;
		case WEST:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_west;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_north;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_south;
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

//void telemetersStop()
//{
//    return;
//}
//void telemetersStart()
//{
//    return;
//}
//void telemetersInit()
//{
//    return;
//}
//void mainControlInit ()
//{
//    return;
//}
//void motorsSleepDriver (char is_on)
//{
//    return;
//}
//int move(float angle, float radius_or_distance, float max_speed, float end_speed)
//{
//    return 1;
//}
//int rotate90WithCal(int rotation_type, float max_speed, float end_speed)
//{
//    return 1;
//}
//
//bool isEndMove()
//{
//    return true;
//}
void waitStart()
{
    // TODO : move this function in robot interface
    ssd1306ClearRect(SSD1306_LCDWIDTH / 2, 10, SSD1306_LCDWIDTH / 2,
            SSD1306_LCDHEIGHT);
    ssd1306PrintfAtLine(SSD1306_LCDWIDTH / 2, 1, &Font_5x8, "wait start");
 //   while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }
    ssd1306Refresh();
    while (expanderJoyFiltered() != JOY_RIGHT)
    {
        HAL_Delay(20);
    }
    ssd1306ClearRect(SSD1306_LCDWIDTH / 2, 10, SSD1306_LCDWIDTH / 2,
            SSD1306_LCDHEIGHT);
    ssd1306Refresh();
//TODO : wait start with front sensors
//  unsigned char sensors_state = hal_sensor_get_state(app_context.sensors);
//  while(check_bit(sensors_state, SENSOR_F10_POS)==true)
//      sensors_state = hal_sensor_get_state(app_context.sensors);
//  HAL_Delay(200);
//  while(check_bit(sensors_state, SENSOR_F10_POS)==false)
//      sensors_state = hal_sensor_get_state(app_context.sensors);
}
