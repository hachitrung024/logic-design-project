/*
 * global.h
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "dht20.h"
#include "i2c-lcd.h"
#include "main.h"
#include "software_timer.h"
#include <math.h>
#include <stdio.h>
I2C_HandleTypeDef hi2c1;
typedef enum{
	INIT,
	CHECK_CONNECTION,
	CHECK_READY,
    REQUEST_DATA,
    READ_DATA,
    CONVERT_DATA,
	SEND_DATA,
	DONE,
	ERROR_STATE
} state;
typedef enum{
	GLOBAL_TIMER,
	LCD_TIMER,
} timerID;
extern state status;
void global_init();
void global_fsm();


#endif /* INC_GLOBAL_H_ */
