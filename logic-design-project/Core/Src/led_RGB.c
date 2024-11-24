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

void setting_led_RGB(int temperature, int humidity) {
	resetAllLED();

    uint8_t red1 = 0, green1 = 0, blue1 = 0; // LED 1 - Nhiệt độ
    uint8_t red3 = 0, green3 = 0, blue3 = 0; // LED 3 - Độ ẩm

    // Điều chỉnh LED 1 (Nhiệt độ: 30 - 40 độ)
    if (temperature >= 30 && temperature < 35) {
        red1 = 255;                              // Cam đậm dần
        green1 = 255 - (temperature - 30) * 51;
        blue1 = 0;
    } else if (temperature >= 35 && temperature <= 40) {
        red1 = 255;                              // Đỏ chuyển sang tím
        green1 = 0;
        blue1 = (temperature - 35) * 51;
    } else if (temperature > 40) {
        red1 = 255;                              // Tím giữ nguyên
        green1 = 0;
        blue1 = 255;
    }

    // Điều chỉnh LED 3 (Độ ẩm)
    if (humidity >= 40 && humidity < 70) {
        green3 = 255 - (humidity - 40) * 8;      // Xanh lá giảm
        blue3 = (humidity - 40) * 8;             // Xanh dương tăng
        red3 = 0;
    } else if (humidity >= 70 && humidity < 80) {
        green3 = 0;
        blue3 = 255 - (humidity - 70) * 25;      // Xanh dương giảm
        red3 = (humidity - 70) * 25;             // Đỏ tăng
    } else if (humidity >= 80 && humidity < 90) {
        green3 = 0;
        blue3 = 255 - (humidity - 80) * 26;      // Tím chuyển đỏ
        red3 = 255;
    } else if (humidity >= 90 && humidity <= 100) {
        green3 = 0;
        blue3 = 0;
        red3 = 255;                              // Đỏ rực
    }

    // Cập nhật LED 1
    setLED(0, red1, green1, blue1); // LED 1: 0 là vị trí đầu tiên
    // Cập nhật LED 3
    setLED(2, red3, green3, blue3); // LED 3: 2 là vị trí thứ ba

    // Gửi dữ liệu PWM đến LED
    ws2812Send();
}

