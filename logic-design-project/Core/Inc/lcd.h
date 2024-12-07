/*
 * lcd.h
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "stm32f1xx_hal.h"

#define LCD_I2C_ADDRESS (0x21 << 1)
extern I2C_HandleTypeDef hi2c1;


void lcd_send_command(uint8_t command);
void lcd_send_data(uint8_t data);
void lcd_initialize(void);
void lcd_send_string(const char *str);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif /* INC_LCD_H_ */
