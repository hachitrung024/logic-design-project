/*
 * lcd.c
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */
#include <lcd.h>
/*
 * [ D7 | D6 | D5 | D4 | X | EN | RW | RS ]
 * lcd doc bit tu trai qua
 * X  - Bo qua				0
 * RS - Register Select		0/1 ,0 Neu la lenh, 1 Neu la du lieu
 * RW - Read/Write 			0 (Write only)
 * EN - Enable 				0/1 ,1 Bat dau doc, 0 Ket thuc doc
 * D7 - D4 					4 bit (lenh / du lieu)
 */
// [4-bit ghi] | [4-bit dieu khien]
//define DIEU KHIEN		0b00001<EN>0<RS>
#define START_READ_CMD 	0b00001100	//EN = 1 RS = 0
#define START_READ_DATA	0b00001101	//EN = 1 RS = 1
#define STOP_READ_CMD 	0b00001000	//EN = 0 RS = 0
#define STOP_READ_DATA 	0b00001001	//EN = 0 RS = 1

// Gui du lieu dieu khien
void lcd_send_command(uint8_t command) {
    uint8_t buffer[4];
    // Doc 4-bit dau
    buffer[0] = (command & 0b11110000) | START_READ_CMD;
    buffer[1] = (command & 0b11110000) | STOP_READ_CMD;
    // Doc 4-bit sau
    buffer[2] = (command << 4) | START_READ_CMD;
    buffer[3] = (command << 4) | STOP_READ_CMD;
    // Gui qua i2c
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS, buffer, 4, 100);
}
// Gui du lieu hien thi
void lcd_send_data(uint8_t data) {
    uint8_t buffer[4];
    // Doc 4-bit dau
    buffer[0] = (data & 0b11110000) | START_READ_DATA;
    buffer[1] = (data & 0b11110000) | STOP_READ_DATA;
    // Doc 4-bit sau
    buffer[2] = (data << 4) | START_READ_DATA;
    buffer[3] = (data << 4) | STOP_READ_DATA;
    // Gui qua i2c
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS, buffer, 4, 100);
}

// Khoi tao lcd
void lcd_initialize(void) {
    // Chuyen sang che do doc 4-bit
    lcd_send_command(0x33); // Lenh thiet lap che do 8-bit (mac dinh)
    lcd_send_command(0x32); // Lenh chuyen qua che do 4-bit
    HAL_Delay(50);
    // Cau hinh
    lcd_send_command(0x28); // Lenh cau hinh 4-bit, 2 dong, ma tran 5x8
    HAL_Delay(50);

    lcd_send_command(0x01); // Lenh xoa man hinh
    HAL_Delay(50);

    lcd_send_command(0x06); // Lenh dat che do nhap du lieu
    HAL_Delay(50);

    lcd_send_command(0x0C); // Lenh bat con tro
    HAL_Delay(50);

    lcd_send_command(0x02); // Dua con tro ve ban dau
    HAL_Delay(50);

    lcd_send_command(0x80); // Vi tri DRAM = 0
}
// Gui chuoi den lcd
void lcd_send_string(const char *str) {
    while (*str)
    {
        lcd_send_data(*str++);
    }
}

// Xoa hien thi
void lcd_clear(void) {
    lcd_send_command(0x01); // Lệnh xóa màn hình
}

// Di chuyen con tro
void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address;
    if (row == 1) {
        address = 0x80 + col; // Hang 1
    }else {
        address = 0xC0 + col; // Hang 2
    }
    lcd_send_command(address);
}

