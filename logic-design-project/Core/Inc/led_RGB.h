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
void interpolateColor(int startValue, int endValue, int currentValue,
                      uint8_t startRed, uint8_t startGreen, uint8_t startBlue,
                      uint8_t endRed, uint8_t endGreen, uint8_t endBlue,
                      uint8_t *red, uint8_t *green, uint8_t *blue);
void setTemperatureColor(int temperature, uint8_t *red, uint8_t *green, uint8_t *blue);
void setHumidityColor(int humidity, uint8_t *red, uint8_t *green, uint8_t *blue);
void ws2812Send(void);


#endif /* INC_LED_RGB_H_ */
