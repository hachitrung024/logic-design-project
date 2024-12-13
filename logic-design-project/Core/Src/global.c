/*
 * global.c
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */

#include "global.h"
state status;
uint8_t lcd_error_flag = 0;
DHT20_t dht20;   // Định nghĩa cấu trúc DHT20
status_active active;      // Định nghĩa biến trạng thái hoạt động
static uint8_t lcd_status;
char lcd_buffer_1[17];
char lcd_buffer_2[17];
char uart_buffer [20];
static void lcd_send_buffer(){
	lcd_set_cursor(1, 0);
	lcd_send_string(lcd_buffer_1);
	lcd_set_cursor(2, 0);
	lcd_send_string(lcd_buffer_2);
}

void global_init(){
	lcd_initialize();
	DHT20_Init(&dht20, &hi2c1);
	active=DHT20_OK;
	setTimer(GLOBAL_TIMER, 1000); // Cho khoi dong cac thiet bi
	status = INIT;
	setTimer(ERROR_CHECK_TIMER, 1000);
}

void watchdog(){
	if(isFlagTimer(ERROR_CHECK_TIMER)){
		if(active == DHT20_ERROR_CONNECT){
			status = ERROR_STATE;
			setTimer(GLOBAL_TIMER, 10000);
			if(active == DHT20_ERROR_CONNECT){
				snprintf(lcd_buffer_1,17,"DHT20 E_CONNECT");
				snprintf(lcd_buffer_2,17,"RECONNECTING ");
				lcd_send_buffer();
			}
		}
		if(HAL_I2C_IsDeviceReady(&hi2c1, 0x21<<1, 1, HAL_MAX_DELAY)!=HAL_OK){
			lcd_error_flag = 1;
		}
		setTimer(ERROR_CHECK_TIMER, 1000);
	}
}

void global_fsm(){
	switch(status){
	case INIT:
		if(isFlagTimer(GLOBAL_TIMER)){
			setTimer(UPDATE_TIMER, UPDATE_CYCLE);
			setTimer(GLOBAL_TIMER, 100);
			status = CHECK_CONNECTION;
		}
	 	 break;
	case CHECK_CONNECTION:
		if(isFlagTimer(GLOBAL_TIMER)){
			if(DHT20_IsConnected(&dht20) ){
				active = DHT20_OK;
				status = CHECK_READY;
			}
			else{
				active = DHT20_ERROR_CONNECT;
			}
		}
		break;
	case CHECK_READY:
		if(!DHT20_IsMeasuring(&dht20)){
			setTimer(GLOBAL_TIMER, 20);
			status = REQUEST_DATA;
		}
		break;
	case REQUEST_DATA:
		if(isFlagTimer(GLOBAL_TIMER)){
			if(HAL_GetTick() - dht20.lastRead >= 1000){
				active = DHT20_RequestData(&dht20);
				if (active == DHT20_OK){
					setTimer(GLOBAL_TIMER, 80);
					status = READ_DATA;
				}
			}
			else{
				active = DHT20_ERROR_LASTREAD;
			}
		}
		break;
	case READ_DATA:
		if(HAL_GetTick() - dht20.lastRequest < 1000){
			active = DHT20_ReadData(&dht20);
			if(active == DHT20_OK){
				status = CONVERT_DATA;
			}
			else{
				status = CHECK_CONNECTION;
			}
		}
		else{
			active = DHT20_ERROR_READ_TIMEOUT;
			status = REQUEST_DATA;
		}
		break;
	case CONVERT_DATA:
		active = DHT20_Convert(&dht20);
		if(active == DHT20_OK){
			status = SEND_DATA_LCD;
		}
		break;
	case SEND_DATA_LCD:
		if(lcd_error_flag==1 && HAL_I2C_IsDeviceReady(&hi2c1, 0x21<<1, 1, HAL_MAX_DELAY)==HAL_OK){
			lcd_initialize();
			lcd_error_flag=0;
		}

	    if (isFlagTimer(UPDATE_TIMER)) {
	        setTimer(UPDATE_TIMER, UPDATE_CYCLE);
	        snprintf(lcd_buffer_1, 17, "Temp: %.2f %cC  ", dht20.temperature, 0xDF);
	        snprintf(lcd_buffer_2, 17, "Humi: %.2f %%   ", dht20.humidity);
	        lcd_send_buffer();
	        // Chuyen sang LED de dieu khien
	        status = SEND_DATA_RGBLED ;
	    }
	    break;
	case SEND_DATA_RGBLED:
	    // Dieu chinh LED 1 (nhiet do) và LED 2 (do am)
	    setting_led_RGB((int)dht20.temperature, (int)dht20.humidity);
	    status = SEND_DATA_ESP32;
	    break;
	case SEND_DATA_ESP32:
		snprintf(uart_buffer,17, "T: %.2f H: %.2f\n", dht20.temperature, dht20.humidity);
		HAL_UART_Transmit_IT(&huart2, (uint8_t*)uart_buffer, 18);
		status = CHECK_READY;
		break;
	case ERROR_STATE:
		if(active == DHT20_ERROR_CONNECT){
			while(!DHT20_IsConnected(&dht20)){
				if(isFlagTimer(GLOBAL_TIMER)){
					snprintf(lcd_buffer_1,17,"Time out! DHT20");
					snprintf(lcd_buffer_2,17,"Can't connect");
					lcd_send_buffer();
					setTimer(GLOBAL_TIMER, 100);
				}
			}
			status = CHECK_READY;
			active = DHT20_OK;
		  }
	default :
		break;
	}
}
//#ifdef LCD_FSM_INIT
enum{
	LCD_SEND,
	LCD_WAIT,
	LCD_DONE
};
static uint8_t init_status = LCD_SEND, cmd_index = 0;
static uint8_t init_cmds[]={0x33, 0x32, 0x28, 0x01, 0x06, 0x0C, 0x02, 0x80 };
void lcd_init_fsm(){
	switch (init_status) {
		case LCD_SEND:
			lcd_send_command(init_cmds[cmd_index]);
			cmd_index++;
			setTimer(LCD_TIMER, 50);
			init_status=LCD_WAIT;
			break;
		case LCD_WAIT:
			if(isFlagTimer(LCD_TIMER)){
				if(cmd_index > 7 ){
					lcd_send_string("Please wait...");
					lcd_status = 0;
					init_status = LCD_DONE;
				}
				else init_status = LCD_SEND;
			}
			break;

		default:
			break;
	}
}
//#endif //LCD_FSM_INIT
