/*
 * global.h
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "dht20.h"
#include "main.h"
#include "software_timer.h"
#include <math.h>
#include <stdio.h>
#include <lcd.h>
#include "led_RGB.h"
I2C_HandleTypeDef hi2c1;
#define UPDATE_CYCLE 1000
typedef enum{
	INIT,
	CHECK_CONNECTION,
	CHECK_READY,
    REQUEST_DATA,
    READ_DATA,
    CONVERT_DATA,
	SEND_DATA_LCD,
	ERROR_STATE,
	SEND_DATA_RGBLED, // Thêm trạng thái
} state;
typedef enum{
	GLOBAL_TIMER,
	UPDATE_TIMER,
	LCD_TIMER,
} timerID;
extern state status;
void global_init();
void global_fsm();
void watchdog();
void lcd_init_fsm();
#endif /* INC_GLOBAL_H_ */
