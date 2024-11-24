/*
 * led_RGB.c
 *
 *  Created on: Nov 20, 2024
 *      Author: ASUS
 */
#include "led_RGB.h"

void resetAllLED(void) {
	for (int i = 0; i < 24 * NUM_OF_LED; i++) pwmData[i] = 1;
}
void setAllLED (void) {
	for (int i = 0; i < 24 * NUM_OF_LED; i++) pwmData[i] = 2;
}
void setLED (int LEDposition, int Red, int Green, int Blue) {
	for (int i = 7; i >= 0; i--) { // Set the first 8 out of 24 to green
	        pwmData[24*LEDposition + 7 - i] = ((Green >> i) & 1) + 1;
	    }
	for (int i = 7; i >= 0; i--) { // Set the second 8 out of 24 to red
	        pwmData[24*LEDposition + 15 - i] = ((Red >> i) & 1) + 1;
	}
	for (int i = 7; i >= 0; i--) { // Set the third 8 out of 24 to blue
	        pwmData[24*LEDposition + 23 - i] = ((Blue >> i) & 1) + 1;
	}
}

void ws2812Send()
{
    if (HAL_DMA_GetState(htim3.hdma[TIM_DMA_ID_CC1]) == HAL_DMA_STATE_READY)
    {
        HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)pwmData, 24 * NUM_OF_LED);
    }
}

void interpolateColor(int startValue, int endValue, int currentValue,
                      uint8_t startRed, uint8_t startGreen, uint8_t startBlue,
                      uint8_t endRed, uint8_t endGreen, uint8_t endBlue,
                      uint8_t *red, uint8_t *green, uint8_t *blue) {
    float factor = (float)(currentValue - startValue) / (endValue - startValue);

    *red = (uint8_t)(startRed + factor * (endRed - startRed));
    *green = (uint8_t)(startGreen + factor * (endGreen - startGreen));
    *blue = (uint8_t)(startBlue + factor * (endBlue - startBlue));
}

void setTemperatureColor(int temperature, uint8_t *red, uint8_t *green, uint8_t *blue) {
    if (temperature < 30 && temperature > 20) {
        // Xanh dương nhạt chuyển sang đậm
        interpolateColor(20, 30, temperature, 0, 0, 255, 0, 0, 128, red, green, blue);
    } else if (temperature >= 30 && temperature < 35) {
        // Vàng sang cam
        interpolateColor(30, 35, temperature, 255, 255, 0, 255, 128, 0, red, green, blue);
    } else if (temperature >= 35 && temperature <= 40) {
        // Cam sang đỏ
        interpolateColor(35, 40, temperature, 255, 128, 0, 255, 0, 0, red, green, blue);
    } else if (temperature > 40) {
    	*red = 255;
    	*green = 0;
    	*blue = 0;
    } else {
        *red = 0;
        *green = 0;
        *blue = 0;
    }
}

void setHumidityColor(int humidity, uint8_t *red, uint8_t *green, uint8_t *blue) {
    if (humidity < 50) {
        // Chuyển từ vàng sang cam sang đỏ khi độ ẩm < 50
        interpolateColor(0, 50, humidity, 255, 0, 0, 255, 255, 0, red, green, blue);
    } else if (humidity >= 50 && humidity < 70) {
        // Chuyển từ xanh lá sang xanh dương
        interpolateColor(50, 70, humidity, 0, 0, 0, 0, 0, 255, red, green, blue);
    } else if (humidity >= 70 && humidity < 80) {
        // Chuyển từ vàng sang cam
        interpolateColor(70, 80, humidity, 255, 255, 0, 255, 128, 0, red, green, blue);
    } else if (humidity >= 80 && humidity < 90) {
        // Chuyển từ cam sang đỏ
        interpolateColor(80, 90, humidity, 255, 128, 0, 255, 0, 0, red, green, blue);
    } else if (humidity >= 90 && humidity <= 100) {
        // Đỏ rực
        *red = 255;
        *green = 0;
        *blue = 0;
    } else {
        *red = 0;
        *green = 0;
        *blue = 0;
    }
}

void setting_led_RGB(int temperature, int humidity) {
    resetAllLED();

    uint8_t red1 = 0, green1 = 0, blue1 = 0; // LED 1 - Nhiệt độ
    uint8_t red3 = 0, green3 = 0, blue3 = 0; // LED 3 - Độ ẩm

    setTemperatureColor(temperature, &red1, &green1, &blue1);
    setHumidityColor(humidity, &red3, &green3, &blue3);

    setLED(0, red1, green1, blue1);
    setLED(2, red3, green3, blue3);

    ws2812Send();
}


