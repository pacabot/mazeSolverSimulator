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
#define multiplicateur 5
#define debug

/* Error codes */
#define SETTING_MODULE_E_SUCCESS    0
#define SETTING_MODULE_E_ERROR	    1

#include <config/config.h>


typedef struct
{
    int x;
    int y;
}coordinate;

typedef struct{
	int max_speed_rotation;
	int max_speed_traslation;
	int min_speed;
}speed_settings;

/* structure of settings */
typedef struct
{
	int cell_cost;
	int wall_know_cost;
	coordinate maze_end_coordinate;
	unsigned long threshold_color;
	unsigned long threshold_greater;
	int start_orientation;
	speed_settings speeds_scan;
}settings;
extern settings zhonxSettings;
int settingsInit (void);
#define HAL_Delay SDL_Delay
enum followType{
	GYRO,
	WALL_FOLLOW
};

#define ON 1
#define OFF 0

#define SAFE_SPEED_ROTATION 10
#define SAFE_SPEED_TRANSLATION 10

#endif /* SETTING_H_ */
