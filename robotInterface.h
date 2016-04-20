/*
 * robotInterface.h
 *
 *  Created on: 4 juin 2015
 *      Author: zhonx
 */

#ifndef ROBOTINTERFACE_H_
#define ROBOTINTERFACE_H_

#include "solverMaze.h"
void goOrientation(char *orientationZhonx, char directionToGo);
void doUTurn(positionRobot *positionZhonx);
void move_zhonx (int direction_to_go, positionRobot *positionZhonx, int numberOfCase, char end_mid_of_case, char chain);
//int move(float angle, float radius_or_distance, float max_speed, float end_speed);
int waitValidation(unsigned long timeout);
void newCell(walls new_walls, labyrinthe *maze, positionRobot positionZhonx);
walls getCellState (void);
void move_zhonx_arc (int direction_to_go, positionRobot *positionZhonx, int numberOfCell, char end_mid_of_case, char chain);
extern positionRobot *pt_zhonx_position;

// hardwara layer
//#define ON true
//#define OFF false
//#define CW true
#define bluetoothPrintf printf
//#define CELL_LENGTH 178
//void telemetersStop();
//void telemetersStart();
//void telemetersInit();
//void mainControlInit ();
//void motorsSleepDriver (char is_on);
//int move(float angle, float radius_or_distance, float max_speed, float end_speed);
//int rotate90WithCal(int rotation_type, float max_speed, float end_speed);
//bool isEndMove();

#endif /* ROBOTINTERFACE_H_ */
