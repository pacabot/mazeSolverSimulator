/*
 * @file : solverMaze.c
 * @author : Colin
 * @version : V 2.1.1
 * @date : 4 juin 2015
 * @brief : this file contain all maze solver application.
 *            this file need robotInterface.c for make hard interface and need
 *            runc.c for explain strategy for run.
 *
 */
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "peripherals/bluetooth/bluetooth.h"

#include "config/config.h"
#include "middleware/settings/settings.h"
#include "middleware/display/pictures.h"


/* application include */
#include <application/solverMaze/robotInterface.h>
#include <application/solverMaze/run.h>
#include <application/solverMaze/solverMaze.h>
#include <peripherals/beeper/beeper.h>
#include <peripherals/display/ssd1306.h>
#include <peripherals/expander/pcf8574.h>
#include "application/solverMaze/solverMaze.h"

/* peripheral include */


/*
 *  ********************** int maze (void) **********************
 *  this function is the main function of the maze solver
 *  return value is the succes of the maze exploration and maze run
 */
int maze_solver_new_maze(void)
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    coordinate end_coordinate; // it's the coordinates which Zhonx have at the start
    positionRobot zhonx_position, start_position;
    labyrinthe maze;
    unsigned int explorationTime;
    pt_zhonx_position = &zhonx_position;

    memset(&end_coordinate, 0, sizeof(coordinate));
    memset(&zhonx_position, 0, sizeof(positionRobot));
    memset(&start_position, 0, sizeof(positionRobot));
    memset(&maze, 0, sizeof(labyrinthe));

    mazeInit(&maze);

    /*init zhonx start position for nime micromouse competition*/

    zhonx_position.coordinate_robot.x = 8;
    zhonx_position.coordinate_robot.y = 8; // the robot start in the center of the maze
    zhonx_position.robot_direction = north;
    /*end of initialization for nime micromouse competition*/
    zhonx_position.midOfCell = FALSE;
    memcpy(&start_position, &zhonx_position, sizeof(zhonx_position));
    start_position.robot_direction = (start_position.robot_direction + 2) % 4;
    start_position.midOfCell = FALSE;
    newCell((walls){WALL_PRESENCE,WALL_PRESENCE,WALL_PRESENCE}, &maze, start_position);
    printLength(maze, zhonx_position.coordinate_robot.x, zhonx_position.coordinate_robot.y);
    memcpy(&start_position, &zhonx_position, sizeof(positionRobot));

    #ifdef PRINT_BLUETOOTH_MAZE_DURING_RUN
        printLength(maze,8,8);
    #endif
    #ifdef PRINT_MAZE_DURING_RUN
        printMaze(maze, zhonx_position.coordinate_robot);
    #endif

ssd1306ClearScreen(MAIN_AREA);
    ssd1306Refresh();
    HAL_Delay(400);
    telemetersStart();
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(30, 1, &Font_5x8, "WAIT START...");
    ssd1306Refresh();
    waitStart();
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(40, 0, &Font_5x8, "SCAN...");
    ssd1306Refresh();
    HAL_Delay(100);

    explorationTime = HAL_GetTick();
    rv = exploration(&maze, &zhonx_position, &start_position, &end_coordinate); //make exploration for go from the robot position and the end of the maze
    explorationTime = HAL_GetTick() - explorationTime;

    if (rv != MAZE_SOLVER_E_SUCCESS)
    {
        #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                bluetoothWaitReady();
                bluetoothPrintf("no solution");
        #endif
        ssd1306WaitReady();
        ssd1306ClearScreen(MAIN_AREA);
        ssd1306DrawBmp(warning_Img, 1, 15, 48, 43);
        ssd1306PrintfAtLine(55, 1, &Font_5x8, "NO SOLUTION");
        ssd1306Refresh();
        tone(D4, 200);
        HAL_Delay(50);
        tone(C4H, 100);
        HAL_Delay(40);
        tone(C4, 150);
    }
    else
    {

        ssd1306ClearScreen(MAIN_AREA);
        ssd1306PrintfAtLine(0, 0, &Font_5x8, "TARGET REACHED !!!");
        ssd1306PrintfAtLine(0, 2, &Font_7x8, "TIME : %d.%ds", explorationTime / 1000, explorationTime % 1000);
        ssd1306Refresh();

        motorsDriverSleep(ON); //because flash write c²ause interrupts damages
        telemetersStop();//because flash write cause interrupts damages

        clearMazelength(&maze);
        computeCellWeight(&maze,start_position.coordinate_robot, false, false);
        findArrival(maze, &end_coordinate);

        rv=saveMaze(&maze, &start_position, &end_coordinate);    // Save maze into flash memory

        motorsDriverSleep(OFF); //because flash write cause interrupts damages
        telemetersStart();//because flash write cause interrupts damages

        findTheShortestPath(&maze, &zhonx_position, &start_position, &end_coordinate); //find the shortest path
    }
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(0,0,&Font_5x8,"RETURN TO START CELL");
    ssd1306PrintfAtLine(0, 2, &Font_7x8, "TIME : %d.%ds", explorationTime / 1000, explorationTime % 1000);
    ssd1306Refresh();
    goToPosition(&maze, &zhonx_position, start_position.coordinate_robot);  //goto start position

    doUTurn(&zhonx_position, SAFE_SPEED_ROTATION, SAFE_SPEED_TRANSLATION, SAFE_SPEED_TRANSLATION);//initial position
    clearMazelength(&maze);
    computeCellWeight(&maze, start_position.coordinate_robot, FALSE, FALSE);
    findArrival(maze, &end_coordinate);

    #ifdef PRINT_BLUETOOTH_MAZE
        printLength(maze,8,8);
    #endif
    #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
        bluetoothPrintf("TIME : %d.%ds", explorationTime / 1000, explorationTime % 1000);
    #endif
    HAL_Delay(2000);
    motorsDriverSleep(ON); //because flash write cause interrupts damages
    telemetersStop();//because flash write cause interrupts damages
    rv=saveMaze(&maze, &start_position, &end_coordinate);    // Save maze into flash memory
    #ifdef PRINT_MAZE
        ssd1306ClearScreen(MAIN_AREA);
        printMaze(maze, zhonx_position.coordinate_robot);
    #endif
    while (expanderJoyFiltered() != JOY_LEFT);
    return MAZE_SOLVER_E_SUCCESS;
}

int startRun1(void)
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    rv = maze_solver_run(1);
    return rv;
}
int startRun2(void)
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    rv = maze_solver_run(2);
    return rv;
}

int maze_solver_run(const int runType)
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    coordinate end_coordinate; // it's the coordinates which Zhonx have at the start
    positionRobot zhonx_position, start_position;
    labyrinthe maze;

    memset(&end_coordinate, 0, sizeof(coordinate));
    memset(&zhonx_position, 0, sizeof(positionRobot));
    memset(&start_position, 0, sizeof(positionRobot));
    memset(&maze, 0, sizeof(labyrinthe));

    mazeInit(&maze);
    loadMaze(&maze, &start_position, &end_coordinate);

    memcpy(&zhonx_position, &start_position, sizeof(positionRobot));


    #ifdef PRINT_BLUETOOTH_MAZE
        printLength(maze,end_coordinate.x,end_coordinate.y);
    #endif

    #ifdef PRINT_MAZE
        ssd1306ClearScreen(MAIN_AREA);
        printMaze(maze, zhonx_position.coordinate_robot);
    #endif

    run(&maze, &zhonx_position,
        start_position.coordinate_robot,
        end_coordinate, runType);

    HAL_Delay(100);
    motorsDriverSleep(ON);
    return MAZE_SOLVER_E_SUCCESS;
}

int restartExplo()
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    coordinate end_coordinate; // it's the coordinates which Zhonx have at the start
    positionRobot zhonx_position, start_position;
    labyrinthe maze;

    memset(&end_coordinate, 0, sizeof(coordinate));
    memset(&zhonx_position, 0, sizeof(positionRobot));
    memset(&start_position, 0, sizeof(positionRobot));
    memset(&maze, 0, sizeof(labyrinthe));

    loadMaze(&maze, &start_position, &end_coordinate);
    memcpy(&zhonx_position, &start_position, sizeof(positionRobot));
    mainControlSetFollowType(WALL_FOLLOW);
    positionControlSetPositionType(GYRO);
    #ifdef PRINT_BLUETOOTH_MAZE
        printLength(maze,8,8);
    #endif

    #ifdef PRINT_MAZE
        ssd1306ClearScreen(MAIN_AREA);
        printMaze(maze, zhonx_position.coordinate_robot);
    #endif
    telemetersStart();
    waitStart();
    clearMazelength(&maze);
    computeCellWeight(&maze, start_position.coordinate_robot, TRUE, FALSE);
    if (findArrival(maze, &end_coordinate) != MAZE_SOLVER_E_SUCCESS)
    {
        rv = exploration(&maze, &zhonx_position, &start_position, &end_coordinate);
        if(rv != MAZE_SOLVER_E_SUCCESS)
        {
            return rv;
        }
    }

    findTheShortestPath(&maze, &zhonx_position, &start_position, &end_coordinate); //find the shortest path

    goToPosition(&maze, &zhonx_position, start_position.coordinate_robot);  //goto start position

    doUTurn(&zhonx_position, SAFE_SPEED_ROTATION, SAFE_SPEED_TRANSLATION, SAFE_SPEED_TRANSLATION);//initial position
    clearMazelength(&maze);
    computeCellWeight(&maze, start_position.coordinate_robot, FALSE, FALSE);
    findArrival(maze, &end_coordinate);
    HAL_Delay(1000);
    motorsDriverSleep(ON); //because flash write cause interrupts damages
    telemetersStop();//because flash write cause interrupts damages
    rv=saveMaze(&maze, &start_position, &end_coordinate);    // Save maze into flash memory
    return MAZE_SOLVER_E_SUCCESS;
}


int printStoredMaze ()
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    labyrinthe maze;
    positionRobot start_position;
    coordinate end_coordinate;
    loadMaze(&maze, &start_position, &end_coordinate);
    #ifdef PRINT_BLUETOOTH_MAZE
        printLength(maze, end_coordinate.x, end_coordinate.y);
    #endif
    printMaze(maze, (coordinate){-1,-1});
    while (expanderJoyFiltered() != JOY_LEFT);
    return rv;
}

int exploration(labyrinthe *maze, positionRobot* positionZhonx,
                const positionRobot *start_position, coordinate *end_coordinate)
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    coordinate way[MAZE_SIZE * MAZE_SIZE] = { { -1, -1 }, { END_OF_LIST,
            END_OF_LIST } };
    clearMazelength(maze);
    computeCellWeight(maze, positionZhonx->coordinate_robot, TRUE, FALSE);

    while (findArrival(*maze, end_coordinate) != MAZE_SOLVER_E_SUCCESS)
    {
        newCell(getCellState(), maze, *positionZhonx);
        clearMazelength(maze);
        computeCellWeight(maze, *end_coordinate, TRUE, TRUE);
        rv = moveVirtualZhonx(*maze, *positionZhonx, way, *end_coordinate);
        if (rv != MAZE_SOLVER_E_SUCCESS)
        {
            #ifdef PRINT_MAZE_DURING_RUN
                printMaze(*maze, positionZhonx->coordinate_robot);
            #endif
            #ifdef PRINT_BLUETOOTH_MAZE_DURING_RUN
                printLength(*maze, -1, -1);
            #endif
            ssd1306ClearScreen(MAIN_AREA);
            ssd1306DrawBmp(warning_Img, 1, 15, 48, 43);
            ssd1306PrintfAtLine(55, 1, &Font_5x8, "NO SOLUTION");
            ssd1306Refresh();
            HAL_Delay(5000);
            return rv;
        }
        rv = moveRealZhonxArc(maze, positionZhonx, way, zhonxSettings.speeds_scan.max_speed_rotation, zhonxSettings.speeds_scan.max_speed_traslation, zhonxSettings.speeds_scan.min_speed);
        if (rv != MAZE_SOLVER_E_SUCCESS)
        {
            ssd1306ClearScreen(MAIN_AREA);
            ssd1306DrawBmp(warning_Img, 1, 15, 48, 43);
            ssd1306PrintfAtLine(55, 1, &Font_5x8, "ERROR WAY");
            ssd1306Refresh();
            HAL_Delay(5000);
            return rv;
        }
        clearMazelength(maze);
        computeCellWeight(maze, positionZhonx->coordinate_robot, TRUE, FALSE);
        #ifdef   PRINT_BLUETOOTH_MAZE_DURING_RUN
                printLength(*maze, positionZhonx->coordinate_robot.x,
                            positionZhonx->coordinate_robot.y);
        #endif
    }

    return rv;
}

int findTheShortestPath(labyrinthe *maze, positionRobot* positionZhonx,
                        const positionRobot *start_position, coordinate *end_coordinate)
{
    int rv = MAZE_SOLVER_E_SUCCESS;
    coordinate way[MAZE_SIZE * MAZE_SIZE] = { { -1, -1 }, { END_OF_LIST,
            END_OF_LIST } };
    coordinate last_coordinate;
    clearMazelength(maze);
    computeCellWeight(maze, *end_coordinate, TRUE, FALSE);
    rv = moveVirtualZhonx(*maze, *start_position, way, *end_coordinate);
    if (rv != MAZE_SOLVER_E_SUCCESS)
    {
        print_no_solution();
    }
    last_coordinate = findEndCoordinate(way);
    while ((last_coordinate.x != end_coordinate->x)
                || (last_coordinate.y != end_coordinate->y))
    {
    	if(BETWEEN(last_coordinate.x, 0, MAZE_SIZE)
    			&& BETWEEN(last_coordinate.y, 0, MAZE_SIZE))
    	{
    		goToPosition(maze, positionZhonx, last_coordinate);
    	}
    	else
    	{
    		goToPosition(maze, positionZhonx, start_position->coordinate_robot);
    	}
        newCell(getCellState(), maze, *positionZhonx);
        clearMazelength(maze);
        computeCellWeight(maze, *end_coordinate, TRUE, FALSE);
        rv = moveVirtualZhonx(*maze, *start_position, way, *end_coordinate);
        if (rv != MAZE_SOLVER_E_SUCCESS)
        {
            print_no_solution();
        }
        last_coordinate = findEndCoordinate(way);
    }
    return rv;
}

int print_no_solution()
{
    ssd1306WaitReady();
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306DrawBmp(warning_Img, 1, 15, 48, 43);
    ssd1306PrintfAtLine(55, 1, &Font_5x8, "NO SOLUTION");
    ssd1306Refresh();
    return MAZE_SOLVER_E_SUCCESS;
}

int goToPosition(labyrinthe *maze, positionRobot* positionZhonx,
                 coordinate end_coordinate)
{
    int max_speed_rotation = SAFE_SPEED_ROTATION;
    int max_speed_translation = SAFE_SPEED_TRANSLATION;
    int min_speed_translation = SAFE_SPEED_TRANSLATION;

    coordinate way[MAZE_SIZE * MAZE_SIZE];
    int rv;
    newCell(getCellState(), maze, *positionZhonx);
    while (positionZhonx->coordinate_robot.x != end_coordinate.x
            || positionZhonx->coordinate_robot.y != end_coordinate.y)
    {
        clearMazelength(maze);  // clear the length for make it with new walls
        computeCellWeight(maze, end_coordinate, TRUE, FALSE);// calculate length into the new maze
        rv = moveVirtualZhonx(*maze, *positionZhonx, way, end_coordinate);// create way for go to the end coordinate if it possible
        if (rv != MAZE_SOLVER_E_SUCCESS)
        {
        #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                    bluetoothWaitReady();
                    bluetoothPrintf("no solution");
        #endif
            // no solution for go to the asked position
            return rv;
        }
        rv = moveRealZhonxArc(maze, positionZhonx, way, max_speed_rotation, max_speed_translation, min_speed_translation);  // use way for go the end position or closer position if there are no-know wall
        if (rv != MAZE_SOLVER_E_SUCCESS)
        {
            #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                        bluetoothWaitReady();
                        bluetoothPrintf("error way");
            #endif
            // no solution for go to the asked position
            return rv;
        }
    }
    return MAZE_SOLVER_E_SUCCESS;
}

int moveVirtualZhonx(labyrinthe maze, positionRobot positionZhonxVirtuel,
                     coordinate way[], coordinate end_coordinate)
{
    int i = 0;
    while (positionZhonxVirtuel.coordinate_robot.x != end_coordinate.x
            || positionZhonxVirtuel.coordinate_robot.y != end_coordinate.y)
    {
        if (maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x + 1)][(int)(positionZhonxVirtuel.coordinate_robot.y)].length
                < maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].length&& positionZhonxVirtuel.coordinate_robot.x+1<MAZE_SIZE
                && maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].wall_east==NO_WALL)
        {
            positionZhonxVirtuel.coordinate_robot.x = positionZhonxVirtuel.coordinate_robot.x
                    + 1;
        }
        else if (maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y
                + 1)].length
                < maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].length&& positionZhonxVirtuel.coordinate_robot.y+1<MAZE_SIZE
                && maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].wall_south==NO_WALL)
        {
            positionZhonxVirtuel.coordinate_robot.y = positionZhonxVirtuel.coordinate_robot.y
                    + 1;
        }
        else if (maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x - 1)][(int)(positionZhonxVirtuel.coordinate_robot.y)].length
                < maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].length&& positionZhonxVirtuel.coordinate_robot.x>0
                && maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].wall_west==NO_WALL)
        {
            positionZhonxVirtuel.coordinate_robot.x = positionZhonxVirtuel.coordinate_robot.x
                    - 1;
        }
        else if (maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y
                - 1)].length
                < maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].length&& positionZhonxVirtuel.coordinate_robot.y>0
                && maze.cell[(int)(positionZhonxVirtuel.coordinate_robot.x)][(int)(positionZhonxVirtuel.coordinate_robot.y)].wall_north==NO_WALL)
        {
            positionZhonxVirtuel.coordinate_robot.y = positionZhonxVirtuel.coordinate_robot.y
                    - 1;
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
        #ifdef PRINT_MAZE_DURING_RUN
                printMaze(maze, positionZhonxVirtuel.coordinate_robot);
        #endif
        #ifdef PRINT_BLUETOOTH_MAZE_DURING_RUN
                printLength(maze, positionZhonxVirtuel.coordinate_robot.x, positionZhonxVirtuel.coordinate_robot.y);
        #endif
        way[i].x = positionZhonxVirtuel.coordinate_robot.x, way[i].y =
                positionZhonxVirtuel.coordinate_robot.y;
        i++;
    }
    way[i].x = END_OF_LIST, way[i].y = END_OF_LIST;
    return MAZE_SOLVER_E_SUCCESS;
}

/**
 * \fn moveRealZhonxDiag(labyrinthe *maze, positionRobot *positionZhonx,
 *                      coordinate *way_tab, int max_speed_rotation, int max_speed_translation, int min_speed_translation)
 * \brief move zhonx with diagonal move
 *
 * this function will move through the way tab by doing dagonal move if its possible
 *
 * \param maze the labyrinth map for update it if it possible
 * \param positionZhonx the robot position to know where is the robot and update it
 * \param way_tab the list of coordinate where the robot will go gradually
 */

int moveRealZhonxDiag(labyrinthe *maze, positionRobot *positionZhonx,
                     coordinate *way_tab, int max_speed_rotation, int max_speed_translation, int min_speed_translation)
{
    walls cell_state;
    char chain;
    int nb_move;
    int i = 0;
    char additionY = 0;
    char additionX = 0;
    enum direction directionToGo = north;
    #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
        bluetoothWaitReady();
        bluetoothPrintf("position adresse : %d\n", positionZhonx);
        bluetoothWaitReady();
        bluetoothPrintf("way :\n");
        while (way_tab[i].x != END_OF_LIST)
        {

            bluetoothWaitReady();
            bluetoothPrintf("%d;%d\ti:%d\n",way_tab[i].x, way_tab[i].y, i);
            i++;
        }
        bluetoothWaitReady();
        bluetoothPrintf("go :\n");
    #endif
    i=0;
    while (way_tab[i].x != END_OF_LIST)
    {
        nb_move = 0;
        if (way_tab[i].x == (positionZhonx->coordinate_robot.x + 1)
                && way_tab[i].y == positionZhonx->coordinate_robot.y)
        {
            additionX = 1;
            additionY = 0;
            directionToGo = east;
        }
        else if (way_tab[i].x == (positionZhonx->coordinate_robot.x - 1)
                && way_tab[i].y == positionZhonx->coordinate_robot.y)
        {
            additionX = -1;
            additionY = 0;
            directionToGo = west;
        }
        else if (way_tab[i].y == (positionZhonx->coordinate_robot.y - 1)
                && way_tab[i].x == positionZhonx->coordinate_robot.x)
        {

            additionX = 0;
            additionY = -1;
            directionToGo = north;
        }
        else if (way_tab[i].y == (positionZhonx->coordinate_robot.y + 1)
                && way_tab[i].x == positionZhonx->coordinate_robot.x)
        {

            additionX = 0;
            additionY = 1;
            directionToGo = south;
        }
        else
        {
            #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                bluetoothWaitReady();
                bluetoothPrintf("Error way : position zhonx x= %d y=%d \t way x= %d y=%d i : %d \n",
                                positionZhonx->coordinate_robot.x,positionZhonx->coordinate_robot.y, way_tab[i].x, way_tab[i].y, i);
            #endif
            return MAZE_SOLVER_E_ERROR;
        }

        while ((way_tab[i].x != END_OF_LIST)
                && way_tab[i].y == (positionZhonx->coordinate_robot.y + additionY)
                && way_tab[i].x == (positionZhonx->coordinate_robot.x + additionX))
        {
            positionZhonx->coordinate_robot.x = way_tab[i].x;
            positionZhonx->coordinate_robot.y = way_tab[i].y;
            #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
                bluetoothWaitReady();
                bluetoothPrintf("%d;%d\ti:%d \t\t%d;%d\n",way_tab[i].x, way_tab[i].y, i,way_tab[19].x, way_tab[19].y);
            #endif
            i++;
            nb_move++;
        }
        if (way_tab[i].x == END_OF_LIST)
            chain = FALSE;
        else
            chain = TRUE;
        move_zhonx(directionToGo, positionZhonx, nb_move, FALSE, TRUE, max_speed_rotation, max_speed_translation, min_speed_translation);
        cell_state = getCellState();

        #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
                bluetoothWaitReady();
                bluetoothPrintf("befor move %d;%d\ti:%d \t\t%d;%d\n",way_tab[i].x, way_tab[i].y, i,way_tab[19].x, way_tab[19].y);
        #endif
        newCell(cell_state, maze, *positionZhonx);
        #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
                bluetoothWaitReady();
                bluetoothPrintf("after move %d;%d\ti:%d \t\t%d;%d\n",way_tab[i].x, way_tab[i].y, i,way_tab[19].x, way_tab[19].y);
        #endif

    }

    #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
        bluetoothWaitReady();
        bluetoothPrintf("end coordinate : %d;%d\ti:%d\n",way_tab[i].x, way_tab[i].y, i);
    #endif
    return MAZE_SOLVER_E_SUCCESS;
}
int moveRealZhonxArc(labyrinthe *maze, positionRobot *positionZhonx,
                     coordinate *way_tab, int max_speed_rotation, int max_speed_translation, int min_speed_translation)
{
    walls cell_state;
    char chain;
    int nb_move;
    int i = 0;
    char additionY = 0;
    char additionX = 0;
    enum direction directionToGo = north;
    #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
        bluetoothWaitReady();
        bluetoothPrintf("position adresse : %d\n", positionZhonx);
        bluetoothWaitReady();
        bluetoothPrintf("way :\n");
        while (way_tab[i].x != END_OF_LIST)
        {

            bluetoothWaitReady();
            bluetoothPrintf("%d;%d\ti:%d\n",way_tab[i].x, way_tab[i].y, i);
            i++;
        }
        bluetoothWaitReady();
        bluetoothPrintf("go :\n");
    #endif
    i=0;
    while (way_tab[i].x != END_OF_LIST)
    {
        nb_move = 0;
        if (way_tab[i].x == (positionZhonx->coordinate_robot.x + 1)
                && way_tab[i].y == positionZhonx->coordinate_robot.y)
        {
            additionX = 1;
            additionY = 0;
            directionToGo = east;
        }
        else if (way_tab[i].x == (positionZhonx->coordinate_robot.x - 1)
                && way_tab[i].y == positionZhonx->coordinate_robot.y)
        {
            additionX = -1;
            additionY = 0;
            directionToGo = west;
        }
        else if (way_tab[i].y == (positionZhonx->coordinate_robot.y - 1)
                && way_tab[i].x == positionZhonx->coordinate_robot.x)
        {

            additionX = 0;
            additionY = -1;
            directionToGo = north;
        }
        else if (way_tab[i].y == (positionZhonx->coordinate_robot.y + 1)
                && way_tab[i].x == positionZhonx->coordinate_robot.x)
        {

            additionX = 0;
            additionY = 1;
            directionToGo = south;
        }
        else
        {
            #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                bluetoothWaitReady();
                bluetoothPrintf("Error way : position zhonx x= %d y=%d \t way x= %d y=%d i : %d \n",
                                positionZhonx->coordinate_robot.x,positionZhonx->coordinate_robot.y, way_tab[i].x, way_tab[i].y, i);
            #endif
            return MAZE_SOLVER_E_ERROR;
        }

        while ((way_tab[i].x != END_OF_LIST)
                && way_tab[i].y == (positionZhonx->coordinate_robot.y + additionY)
                && way_tab[i].x == (positionZhonx->coordinate_robot.x + additionX))
        {
            positionZhonx->coordinate_robot.x = way_tab[i].x;
            positionZhonx->coordinate_robot.y = way_tab[i].y;
            #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
                bluetoothWaitReady();
                bluetoothPrintf("%d;%d\ti:%d \t\t%d;%d\n",way_tab[i].x, way_tab[i].y, i,way_tab[19].x, way_tab[19].y);
            #endif
            i++;
            nb_move++;
        }
        if (way_tab[i].x == END_OF_LIST)
            chain = FALSE;
        else
            chain = TRUE;
        move_zhonx(directionToGo, positionZhonx, nb_move, FALSE, TRUE, max_speed_rotation, max_speed_translation, min_speed_translation);
        cell_state = getCellState();

        #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
                bluetoothWaitReady();
                bluetoothPrintf("befor move %d;%d\ti:%d \t\t%d;%d\n",way_tab[i].x, way_tab[i].y, i,way_tab[19].x, way_tab[19].y);
        #endif
        newCell(cell_state, maze, *positionZhonx);
        #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
                bluetoothWaitReady();
                bluetoothPrintf("after move %d;%d\ti:%d \t\t%d;%d\n",way_tab[i].x, way_tab[i].y, i,way_tab[19].x, way_tab[19].y);
        #endif

    }

    #ifdef PRINT_BLUETOOTH_ADVANCED_DEBUG
        bluetoothWaitReady();
        bluetoothPrintf("end coordinate : %d;%d\ti:%d\n",way_tab[i].x, way_tab[i].y, i);
    #endif
    return MAZE_SOLVER_E_SUCCESS;
}

void computeCellWeight(labyrinthe *maze, coordinate end_coordinate, char wallNoKnow,
                       char contournKnownCell)
{
    int i1 = 0, i2 = 0;
    int length = 0;
    int x = end_coordinate.x;
    int y = end_coordinate.y;
    maze->cell[x][y].length = length;
    coordinate dotes_to_verify_tab[MAZE_SIZE * MAZE_SIZE];
    dotes_to_verify_tab[0].x = x;
    dotes_to_verify_tab[0].y = y;
    dotes_to_verify_tab[1].x = END_OF_LIST;
    coordinate new_dotes_to_verify_tab[MAZE_SIZE * MAZE_SIZE];
    coordinate *dotes_to_verify = dotes_to_verify_tab;
    coordinate *new_dotes_to_verify = new_dotes_to_verify_tab;
    coordinate *pt = NULL;

    while (dotes_to_verify[0].x != END_OF_LIST)
    {
        while (dotes_to_verify[i1].x != END_OF_LIST)
        {
            x = dotes_to_verify[i1].x;
            y = dotes_to_verify[i1].y;
            int part_length = 0;
            if (contournKnownCell)
            {
                if (maze->cell[x][y].wall_north != NO_KNOWN)
                {
                    part_length += zhonxSettings.wall_know_cost;
                }
                if (maze->cell[x][y].wall_east != NO_KNOWN)
                {
                    part_length += zhonxSettings.wall_know_cost;
                }
                if (maze->cell[x][y].wall_south != NO_KNOWN)
                {
                    part_length += zhonxSettings.wall_know_cost;
                }
                if (maze->cell[x][y].wall_west != NO_KNOWN)
                {
                    part_length += zhonxSettings.wall_know_cost;
                }
            }
            maze->cell[x][y].length += part_length;
            length = maze->cell[x][y].length + zhonxSettings.cell_cost;

            if ((maze->cell[x][y].wall_north == NO_WALL
                    || (wallNoKnow == TRUE
                            && maze->cell[x][y].wall_north == NO_KNOWN))
                    && maze->cell[x][y - 1].length > length - 1 && y > 0)
            {
                new_dotes_to_verify[i2].x = x;
                new_dotes_to_verify[i2].y = y - 1;
                i2++;
                maze->cell[x][y - 1].length = length;
            }
            if ((maze->cell[x][y].wall_east == NO_WALL
                    || (wallNoKnow == TRUE
                            && maze->cell[x][y].wall_east == NO_KNOWN))
                    && maze->cell[x + 1][y].length > length&& x+1<MAZE_SIZE)
            {
                new_dotes_to_verify[i2].x = x + 1;
                new_dotes_to_verify[i2].y = y;
                i2++;
                maze->cell[x + 1][y].length = length;
            }
            if ((maze->cell[x][y].wall_south == NO_WALL
                    || (wallNoKnow == TRUE
                            && maze->cell[x][y].wall_south == NO_KNOWN))
                    && maze->cell[x][y + 1].length > length&& y+1<MAZE_SIZE)
            {
                new_dotes_to_verify[i2].x = x;
                new_dotes_to_verify[i2].y = y + 1;
                i2++;
                maze->cell[x][y + 1].length = length;
            }
            if ((maze->cell[x][y].wall_west == NO_WALL
                    || (wallNoKnow == TRUE
                            && maze->cell[x][y].wall_west == NO_KNOWN))
                    && maze->cell[x - 1][y].length > length && x > 0)
            {
                new_dotes_to_verify[i2].x = x - 1;
                new_dotes_to_verify[i2].y = y;
                i2++;
                maze->cell[x - 1][y].length = length;
            }
            i1++;
        }
        new_dotes_to_verify[i2].x = END_OF_LIST;
        pt = dotes_to_verify;
        dotes_to_verify = new_dotes_to_verify;
        new_dotes_to_verify = pt;
        i2 = 0;
        i1 = 0;
    }
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
            maze->cell[i][y].length = INFINITY_WEIGHT;
        }
    }
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        maze->cell[i][0].wall_north = WALL_PRESENCE;
        maze->cell[i][MAZE_SIZE - 1].wall_south = WALL_PRESENCE;
        maze->cell[0][i].wall_west = WALL_PRESENCE;
        maze->cell[MAZE_SIZE - 1][i].wall_east = WALL_PRESENCE;
    }

}

void printMaze(labyrinthe maze, coordinate robot_coordinate)
{
    ssd1306ClearRect(0, HEAD_MARGIN, 54, 54);
    int size_cell_on_oled = ((51) / (MAZE_SIZE));
    int x, y;

    for (y = 0; y < MAZE_SIZE; y++)
    {
        for (x = 0; x < MAZE_SIZE; x++)
        {
            if (maze.cell[x][y].wall_north == WALL_PRESENCE)
            {
                ssd1306DrawLine(x * size_cell_on_oled,
                                y * size_cell_on_oled + HEAD_MARGIN,
                                x * size_cell_on_oled + size_cell_on_oled + 1,
                                y * size_cell_on_oled + HEAD_MARGIN);
            }
            if (maze.cell[x][y].wall_west == WALL_PRESENCE)
            {
                ssd1306DrawLine(x * size_cell_on_oled,
                                y * size_cell_on_oled + HEAD_MARGIN,
                                x * size_cell_on_oled,
                                (y + 1) * size_cell_on_oled
                                + HEAD_MARGIN + 1);
            }
            if (maze.cell[x][y].wall_south == WALL_PRESENCE)
            {
                ssd1306DrawLine(x * size_cell_on_oled,
                                (y+ 1) * size_cell_on_oled+ HEAD_MARGIN,
                                size_cell_on_oled + x * size_cell_on_oled,
                                (y+ 1) * size_cell_on_oled + HEAD_MARGIN);
            }
            if (maze.cell[x][y].wall_east == WALL_PRESENCE)
            {
                ssd1306DrawLine((x + 1) * size_cell_on_oled,
                                y * size_cell_on_oled + HEAD_MARGIN,
                                (x + 1) * size_cell_on_oled,
                                (y + 1) * size_cell_on_oled
                                + HEAD_MARGIN + 1);
            }
        }
    }
    //    ssd1306DrawRect((robot_coordinate.x * size_cell_on_oled) + 1,
    //                    (robot_coordinate.y * size_cell_on_oled + HEAD_MARGIN) + 1, 2,
    //                    2);
    ssd1306Refresh();
}

void printLength(const labyrinthe maze, const int x_robot, const int y_robot)
{
    bluetoothPrintf("zhonx : %d; %d\n  ", x_robot, y_robot);
    for (int x = 0; x < MAZE_SIZE; x++)
	{
    	bluetoothPrintf("%6d", x);
	}
    bluetoothPrintf("\n");
	for (int y = 0; y < MAZE_SIZE; y++)
	{
		bluetoothPrintf("    ");
		for (int x = 0; x < MAZE_SIZE; x++)
		{
		  switch (maze.cell[x][y].wall_north) {
			case NO_KNOWN:
				  bluetoothPrintf("+- - +");
				break;
			case WALL_PRESENCE:
				  bluetoothPrintf("+----+");
				break;
			default:
				  bluetoothPrintf("+    +");
				break;
		}
	  }
	  bluetoothPrintf("\n %2d ", y);
	  for (int x = 0; x < MAZE_SIZE; x++)
	  {
		  switch (maze.cell[x][y].wall_west)
		  {
		  	  case NO_KNOWN:
				  bluetoothPrintf("!");
		  		  break;
		  	  case WALL_PRESENCE:
		  		  bluetoothPrintf("|");
		  		  break;
		  	  default:
				  bluetoothPrintf(" ");
				  break;

		  }
		  if (maze.cell[x][y].length != INFINITY_WEIGHT)
		  {
			  bluetoothPrintf("%4d", maze.cell[x][y].length);
		  }
		  else
		  {
			  bluetoothPrintf("+OO ");
		  }
		  switch (maze.cell[x][y].wall_east)
		  {
		  	  case NO_KNOWN:
				  bluetoothPrintf("!");
		  		  break;
		  	  case WALL_PRESENCE:
		  		  bluetoothPrintf("|");
		  		  break;
		  	  default:
				  bluetoothPrintf(" ");
				  break;

		  }
	  }
	  bluetoothPrintf("\n");
	  bluetoothPrintf("    ");
	  for (int x = 0; x < MAZE_SIZE; x++)
	  {
		  switch (maze.cell[x][y].wall_south) {
			case NO_KNOWN:
				  bluetoothPrintf("+- - +");
				break;
			case WALL_PRESENCE:
				  bluetoothPrintf("+----+");
				break;
			default:
				  bluetoothPrintf("+    +");
				break;
		}
	  }
	  bluetoothPrintf("\n");
	}
	bluetoothPrintf("\n");
    bluetoothWaitReady();
}

void clearMazelength(labyrinthe* maze)
{
    int x, y;
    for (y = 0; y < MAZE_SIZE; y++)
    {
        for (x = 0; x < MAZE_SIZE; x++)
        {
            maze->cell[x][y].length = INFINITY_WEIGHT;
        }
    }
}

char miniwayFind(labyrinthe *maze, coordinate start_coordinate,
                 coordinate end_coordinate)
{
    // TODO not find the shorter in distance way but the faster
    coordinate way1[MAZE_SIZE * MAZE_SIZE];
    coordinate way2[MAZE_SIZE * MAZE_SIZE];
    clearMazelength(maze);
    computeCellWeight(maze, end_coordinate, TRUE, FALSE);
    #ifdef PRINT_MAZE
        printMaze(*maze, (coordinate ) { -1, -1 });
    #endif
    positionRobot position;
    position.midOfCell = TRUE;
    position.coordinate_robot = start_coordinate;
    position.robot_direction = north;
    moveVirtualZhonx(*maze, position, way1, end_coordinate);
    clearMazelength(maze);
    computeCellWeight(maze, end_coordinate, FALSE, FALSE);
    #ifdef PRINT_MAZE
        printMaze(*maze, (coordinate ) { -1, -1 });
    #endif
    moveVirtualZhonx(*maze, position, way2, end_coordinate);
    ssd1306ClearScreen(MAIN_AREA);
    char waySame = diffway(way1, way2);
    switch (waySame)
    {
        case TRUE:
            ssd1306DrawString(0, 20, "2 way = : yes", &Font_5x8);
            break;
        case FALSE:
            ssd1306DrawString(0, 20, "2 way = : no", &Font_5x8);
            break;
    }
    ssd1306Refresh();
    HAL_Delay(3000);
    return (waySame);
}

char diffway(coordinate way1[], coordinate way2[])
{
    int i = 0;
    while ((way1[i].x != END_OF_LIST) && (way2[i].x != END_OF_LIST))
    {
        if (way1[i].x != way2[i].x || way1[i].y != way2[i].y)
        {
            return FALSE;
        }
        i++;
    }
    if (!NAND((way1[i].x != END_OF_LIST), (way2[i].x != END_OF_LIST)))
    {
        return FALSE;
    }
    return TRUE;
}
coordinate findEndCoordinate(coordinate coordinate_tab[])
{
	coordinate rv;
    int i = 0;
    while (coordinate_tab[i].x != END_OF_LIST)
    {
        i++;
    }
    if(i > 0)
    {
    	rv = coordinate_tab[i - 1];
    }
    else
    {
    	rv.x=-1;
    	rv.y=-1;
    }
    return rv;
}

int findArrival(labyrinthe maze, coordinate *end_coordinate)
{
    int possible_end_find_cost = INFINITY_WEIGHT;
    end_coordinate->x = -1;
    end_coordinate->y = -1;
    for (int x = 0; x < (MAZE_SIZE - 1); x++)
    {
        for (int y = 0; y < (MAZE_SIZE - 1); ++y)
        {
            if (maze.cell[x][y].wall_east == NO_WALL
                    && maze.cell[x][y].wall_south == NO_WALL
                    && maze.cell[x][y + 1].wall_east == NO_WALL
                    && maze.cell[x + 1][y].wall_south == NO_WALL
                    && maze.cell[x][y].length != INFINITY_WEIGHT)
            {
                end_coordinate->x = x;
                end_coordinate->y = y;
                possible_end_find_cost = maze.cell[x][y].length;
                if (maze.cell[x + 1][y].length > possible_end_find_cost)
                {
                    end_coordinate->x = x + 1;
                    end_coordinate->y = y;
                    possible_end_find_cost = maze.cell[x + 1][y].length;
                }
                if (maze.cell[x][y + 1].length > possible_end_find_cost)
                {
                    end_coordinate->x = x;
                    end_coordinate->y = y+1;
                    possible_end_find_cost = maze.cell[x][y + 1].length;
                }
                if (maze.cell[x + 1][y + 1].length > possible_end_find_cost)
                {
                    end_coordinate->x = x + 1;
                    end_coordinate->y = y + 1;
                    possible_end_find_cost = maze.cell[x][y + 1].length;
                }
                #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                    bluetoothPrintf("end find at : %i; %i\n", end_coordinate->x, end_coordinate->y);
                #endif
                return MAZE_SOLVER_E_SUCCESS;
            }
            if (possible_end_find_cost
                    > maze.cell[x][y].length&&
                    maze.cell[x][y].wall_east != WALL_PRESENCE && maze.cell[x][y].wall_south != WALL_PRESENCE
                    && maze.cell[x][y+1].wall_east != WALL_PRESENCE && maze.cell[x+1][y].wall_south != WALL_PRESENCE
                    && maze.cell[x][y].length != INFINITY_WEIGHT
                    && maze.cell[x][y].length != INFINITY_WEIGHT && maze.cell[x+1][y].length != INFINITY_WEIGHT
                    && maze.cell[x][y+1].length != INFINITY_WEIGHT && maze.cell[x+1][y+1].length != INFINITY_WEIGHT)
            {
                if ((maze.cell[x][y].wall_east != NO_WALL
                        || maze.cell[x][y].wall_south != NO_WALL)
                        && maze.cell[x][y].length != 0
                        && maze.cell[x][y].length < possible_end_find_cost)
                {
                    end_coordinate->x = x;
                    end_coordinate->y = y;
                    possible_end_find_cost =
                            maze.cell[end_coordinate->x][end_coordinate->y].length;
                }
                if ((maze.cell[x + 1][y].wall_west != NO_WALL
                        || maze.cell[x + 1][y].wall_south != NO_WALL)
                        && maze.cell[x+1][y].length != 0
                        && maze.cell[x+1][y].length < possible_end_find_cost)
                {
                    end_coordinate->x = x + 1;
                    end_coordinate->y = y;
                    possible_end_find_cost =
                            maze.cell[end_coordinate->x][end_coordinate->y].length;
                }
                if ((maze.cell[x][y + 1].wall_east != NO_WALL
                        || maze.cell[x][y + 1].wall_south != NO_WALL)
                        && maze.cell[x][y+1].length != 0
                        && maze.cell[x][y+1].length < possible_end_find_cost)
                {
                    end_coordinate->x = x;
                    end_coordinate->y = y + 1;
                    possible_end_find_cost =
                            maze.cell[end_coordinate->x][end_coordinate->y].length;
                }
                if ((maze.cell[x + 1][y + 1].wall_west != NO_WALL
                        || maze.cell[x + 1][y + 1].wall_south != NO_WALL)
                        && maze.cell[x+1][y+1].length != 0
                        && maze.cell[x+1][y+1].length < possible_end_find_cost)
                {
                    end_coordinate->x = x + 1;
                    end_coordinate->y = y + 1;
                    possible_end_find_cost =
                            maze.cell[end_coordinate->x][end_coordinate->y].length;
                }
                #ifdef PRINT_BLUETOOTH_BASIC_DEGUG
                    bluetoothPrintf("possible end find at : %i; %i go to position : %d; %d\n", x, y, end_coordinate->x, end_coordinate->y);
                #endif
            }

        }
    }
    return MAZE_SOLVER_E_ERROR;
}
