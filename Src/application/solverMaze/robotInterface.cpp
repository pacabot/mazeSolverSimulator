/*
 * @file : robotInterface.c
 * @author : Colin
 * @version : V 2.0
 * @date : 4 juin 2015
 * @brief : this file contain all the necessary stuff for make interface
 *          between the maze solver and the robot move function.
 */


#include "application/solverMaze/solverMaze.h"
#include "peripherals/display/ssd1306.h"
#include "peripherals/expander/pcf8574.h"
#include "middleware/settings/settings.h"
#include "middleware/wall_sensors/wall_sensors.h"
#include <SDL2/SDL.h>
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
positionRobot *pt_zhonx_position;
labyrinthe maze_to_discovert = {
		{
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,WALL_PRESENCE,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,NO_WALL,NO_WALL,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,NO_WALL,NO_WALL,2000}
				},
				{
						{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,WALL_PRESENCE,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE,2000},{WALL_PRESENCE,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,NO_WALL,WALL_PRESENCE,NO_WALL,2000},{NO_WALL,WALL_PRESENCE,WALL_PRESENCE,NO_WALL,2000}
				}
		}
};
void goOrientation(char *orientationZhonx, char directionToGo)
{
	*orientationZhonx= directionToGo;
    return;
}
void doUTurn(positionRobot *positionZhonx, int max_speed_rotation, int max_speed_translation, int min_speed_translation)
{
    return;
}
void move_zhonx(char direction_to_go, positionRobot *positionZhonx, unsigned int numberOfCell, char end_mid_of_case,
                char chain, int max_speed_rotation, int max_speed_translation, int min_speed_translation)
{
	positionZhonx->robot_direction= direction_to_go;
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
#ifdef PRINT_CELL_STATE
	print_cell_state(new_walls);
	ssd1306Refresh();
	/*end print wall position*/
#endif
    switch (positionZhonx.robot_direction)
    {
        case north:
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

        case east:

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

        case south:

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

        case west:
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
	switch (pt_zhonx_position->robot_direction)
	{
		case north:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_north;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_east;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_west;
			break;
		case east:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_east;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_south;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_north;
			break;
		case south:
			returnValue.front=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_south;
			returnValue.right=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_west;
			returnValue.left=maze_to_discovert.cell[pt_zhonx_position->coordinate_robot.x][pt_zhonx_position->coordinate_robot.y].wall_east;
			break;
		case west:
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
	positionZhonx->robot_direction = direction_to_go;
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
    ssd1306ClearRect(SSD1306_LCDWIDTH / 2, 10, SSD1306_LCDWIDTH / 2,
            SSD1306_LCDHEIGHT);
    ssd1306PrintfAtLine(SSD1306_LCDWIDTH / 2, 1, &Font_5x8, "wait start");

    ssd1306Refresh();
    while (expanderJoyFiltered() != JOY_RIGHT)
    {
        HAL_Delay(20);
    }
    ssd1306ClearRect(SSD1306_LCDWIDTH / 2, 10, SSD1306_LCDWIDTH / 2,
            SSD1306_LCDHEIGHT);
    ssd1306Refresh();
}

void print_cell_state (walls cell_state)
{
    #ifdef PRINT_WALLS_DETECTED
    ssd1306ClearRect(64,DISPLAY_OFFSET,54,5);
    ssd1306ClearRect(64,DISPLAY_OFFSET,5,54);
    ssd1306ClearRect(113,DISPLAY_OFFSET,5,54);

    if (cell_state.front == WALL_PRESENCE)
    {
        ssd1306FillRect(64,DISPLAY_OFFSET,54,5);
    }
    if (cell_state.left == WALL_PRESENCE)
    {
        ssd1306FillRect(64,DISPLAY_OFFSET,5,54);
    }
    if (cell_state.right == WALL_PRESENCE)
    {
        ssd1306FillRect(113,DISPLAY_OFFSET,5,54);
    }
    #endif
    #ifdef PRINT_CELL_STATE_BLEUTOOTH
    if (cell_state.front == WALL_PRESENCE)
    {
        bluetoothPrintf("_");
    }
    if (cell_state.left == WALL_PRESENCE)
    {
        bluetoothPrintf("|");
    }
    else
    {
        bluetoothPrintf(" ");
    }
    if (cell_state.right == WALL_PRESENCE)
    {
        bluetoothPrintf("|");
    }
    bluetoothPrintf("\n");
    #endif
}

int saveMaze(labyrinthe *maze, positionRobot *start_position, coordinate  *end_coordinate)
{
	return 1;
}
int loadMaze(labyrinthe *maze, positionRobot *start_position, coordinate  *end_coordinate)
{
	return 1;
}
