/*
 * led_RGB.h
 *
 *  Created on: Nov 20, 2024
 *      Author: ASUS
 */

#ifndef INC_LED_RGB_H_
#define INC_LED_RGB_H_
//-------------------- INCLUDE --------------------//
#include "main.h"

//-------------------- DEFINE --------------------//
#define NUM_OF_LED 4

//-------------------- DECLEARE --------------------//
uint16_t pwmData[24*NUM_OF_LED]; // Data PWM for LEDs
extern TIM_HandleTypeDef htim3;  // PWM with STM

//-------------------- FUNCTION --------------------//
void resetAllLED(void);
void setAllLED (void);
void setLED (int LEDposition, int Red, int Green, int Blue);
void setting_led_RGB(int temperature, int humidity);
void ws2812Send(void);


#endif /* INC_LED_RGB_H_ */
