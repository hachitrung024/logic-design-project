/*
 * global.c
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */

#include "global.h"
state status;
DHT20_t dht20;   // Định nghĩa cấu trúc DHT20
status_active active;      // Định nghĩa biến trạng thái hoạt động
static char lcd_buffer_1[17];
static char lcd_buffer_2[17];
static void lcd_send_buffer(){
	lcd_goto_XY(1, 0);
	lcd_send_string(lcd_buffer_1);
	lcd_goto_XY(2, 0);
	lcd_send_string(lcd_buffer_2);
}
void global_init(){
	lcd_init();
	DHT20_Init(&dht20, &hi2c1);
	lcd_send_string("Loading...");
	setTimer(GLOBAL_TIMER, 100); // Cho khoi dong cac thiet bi
	status = INIT;
}
void global_fsm(){
	switch(status){
	case INIT:
		if(isFlagTimer(GLOBAL_TIMER)){
			status = CHECK_CONNECTION;
		}
	 	 break;
	case CHECK_CONNECTION:
		if(DHT20_IsConnected(&dht20) ){
			active = DHT20_OK;
			status = CHECK_READY;
		}
		else{
			active = DHT20_ERROR_CONNECT;
			status = ERROR_STATE;
		}
		break;
	case CHECK_READY:
		if((DHT20_ReadStatus(&dht20) & 0x18) != 0x18){
			DHT20_ResetSensor(&dht20);
		 	HAL_Delay(1000);
			status = CHECK_CONNECTION;
		}
		else {
			HAL_Delay(10);
			status = REQUEST_DATA;
		}
		break;
	case REQUEST_DATA:
		if(HAL_GetTick() - dht20.lastRead >= 1000){
			active = DHT20_RequestData(&dht20);
			if (active == DHT20_OK){
				HAL_Delay(80);
				status = READ_DATA;
			}
		}
		else{
			active = DHT20_ERROR_LASTREAD;
		}
		break;
	case READ_DATA:
		if(HAL_GetTick() - dht20.lastRequest < 1000){
			active = DHT20_ReadData(&dht20);
			if(active == DHT20_OK){
				status = CONVERT_DATA;
			}
			else{
				status = REQUEST_DATA;
			}
		}
		else{
			active = DHT20_ERROR_READ_TIMEOUT;
		}
		break;
	case CONVERT_DATA:
		active = DHT20_Convert(&dht20);
		if(active == DHT20_OK){
			status = SEND_DATA;
		}
		break;
	case SEND_DATA:
		snprintf(lcd_buffer_1,17,"Temp: %.2f",dht20.temperature);
		snprintf(lcd_buffer_2,17,"Humi: %.2f",dht20.humidity);
		lcd_send_buffer();
		status = DONE;
		break;
	case DONE:
	{
		status = CHECK_READY;
		break;
	}
	case ERROR_STATE:
			if(active == DHT20_ERROR_CONNECT){
	/*
	 * 		do{
				  HAL_Delay(100);
			  }while(!DHT20_IsConnected(&dht20));
			  active = DHT20_OK;
			 */
			  status = INIT;
		  }
		else{
		  }
	    break;
	default :
		break;
	}

}
