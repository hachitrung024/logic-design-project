/*
 * global.c
 *
 *  Created on: Nov 16, 2024
 *      Author: hachi
 */

#include "global.h"
uint32_t status;
void global_init(){
	setTimer(0, 1000);
	status = INIT;
}
void global_fsm(){

	if(isFlagTimer(0)){
		setTimer(0, 1000);
		HAL_GPIO_TogglePin(LD0_GPIO_Port, LD0_Pin);
	}
}
