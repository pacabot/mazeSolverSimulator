/*
 * setting.h
 *
 *  Created on: 3 avr. 2015
 *      Author: Colin
 */

#ifndef SETTING_H_
#define SETTING_H_
/* dependencies */
#define SIMULATOR
#define HAL_Delay SDL_Delay
#define multiplicateur 5
#define debug
#define MAIN_AREA

/* Error codes */
#define SETTING_MODULE_E_SUCCESS    0
#define SETTING_MODULE_E_ERROR	    1

#include <solverMaze.h>

/* structure of settings */
typedef struct
{
	char calibration_enabled;
	char color_sensor_enabled;
	int cell_cost;
	int wall_know_cost;
	coordinate maze_end_coordinate;
	unsigned long threshold_color;
	unsigned long threshold_greater;
	int start_orientation;
}settings;
extern settings zhonxSettings;
int settingsInit (void);
#endif /* SETTING_H_ */
