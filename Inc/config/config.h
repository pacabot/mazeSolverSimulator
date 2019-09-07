/*
 * config.h
 *
 *  Created on: May 27, 2019
 *      Author: colin
 */

#ifndef INC_CONFIG_CONFIG_H_
#define INC_CONFIG_CONFIG_H_

/**
 * @def MAZE_SIZE
 *
 * define the maze side length
 */
#define MAZE_SIZE 18

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#define motorsDriverSleep(x)
#include <time.h>
#define HAL_GetTick() time(NULL)
#define mainControlSetFollowType(x)
#define positionControlSetPositionType(x)
#define telemetersStart()
#define telemetersStop()


#endif /* INC_CONFIG_CONFIG_H_ */
