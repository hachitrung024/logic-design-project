/*
 * software_timer.c
 *
 *  Created on: Sep 25, 2024
 *      Author: hachi
 */

#include "software_timer.h"

static uint32_t TIMER_CYCLE  = 10;

static uint32_t timer_counters[MAX_TIMERS] ={0,0};
static uint8_t timer_flags[] = {0,0};


uint8_t isFlagTimer(uint8_t index){
	return timer_flags[index];
}
void setTimer(uint8_t index, uint32_t duration){
	timer_flags[index] = 0;
	timer_counters[index] = duration / TIMER_CYCLE;
}
void timerInit(uint32_t prescaler, uint32_t period){
	uint32_t frequency = HAL_RCC_GetHCLKFreq();
	TIMER_CYCLE  = (prescaler+1)*(period+1)*1000/frequency;
}
void timerRun(){
	for(uint8_t i = 0 ; i < MAX_TIMERS; i++ ){
		if(timer_counters[i] > 0){
			timer_counters[i] --;
			if(timer_counters[i] <=0){
				timer_flags[i] = 1;
			}
		}
	}

}
