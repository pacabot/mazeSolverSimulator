/*
 * resolution_maze.h
 *
 *  Created on: 27 sept. 2014
 *      Author: Colin
 */

#ifndef RESOLUTION_MAZE_H_
#define RESOLUTION_MAZE_H_

#define NAND(a,b) (!a && !b)

/* Error codes */
#define MAZE_SOLVER_E_SUCCESS  0
#define MAZE_SOLVER_E_ERROR    1

//Define for the maze

#define MAZE_SIZE 16

//orientation define
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
//action define
#define FORWARD 0
#define RIGHT 1
#define UTURN 2
#define LEFT 3
//wall state define
#define NO_KNOWN 0
#define WALL_PRESENCE 1
#define NO_WALL 2

#define MAX_SPEED_ROTATION		(300)
#define MAX_SPEED_TRANSLATION   (400)
#define END_SPEED_TRANSLATION	(400)

#define DEBUG 3

#ifdef DEBUG
#undef END_SPEED_TRANSLATION
#define END_SPEED_TRANSLATION 0
#endif
#define END_OF_LIST 255
#include <stdlib.h>

//Structures typedef
typedef struct
{
  char wall_north;
  char wall_south;
  char wall_east;
  char wall_west;
  int length;
}one_cell;

typedef struct
{
  one_cell cell[MAZE_SIZE][MAZE_SIZE];
}labyrinthe;

typedef struct
{
	char next_front;
	char front;
	char left;
	char right;
}walls;

typedef struct coordinate
{
  int x;
  int y;
}coordinate;
typedef struct
{
    coordinate cordinate;// todo renplace x and y by coordinate
    char orientation;
    char midOfCell;
} positionRobot;


// fonctions
extern int maze(void);
void exploration(labyrinthe *maze, positionRobot* positionZhonx,  coordinate end_coordinate);
void moveVirtualZhonx(labyrinthe maze, positionRobot positionZhonxVirtuel,
		coordinate way[], coordinate end_coordinate);
void poids(labyrinthe *maze, coordinate end_coordinate, char wallNoKnow);
void mazeInit (labyrinthe *maze);
void* calloc_s (size_t nombre, size_t taille);
void printMaze(const labyrinthe maze, coordinate robot_coordinate);
void printLength(const labyrinthe maze,const int x_robot, const int y_robot);
void clearMazelength(labyrinthe* maze);
char miniwayFind(labyrinthe *maze, coordinate start_coordinate, coordinate end_coordinate);
void moveRealZhonxArc(labyrinthe *maze, positionRobot *positionZhonx, coordinate way[]);
void waitStart(void);
char diffway(coordinate *way1,coordinate *way2);
#endif /* RESOLUTION_MAZE_H_ */
