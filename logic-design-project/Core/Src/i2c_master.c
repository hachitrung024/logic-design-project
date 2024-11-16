/*
 * i2c_master.c
 *
 *  Created on: Oct 12, 2024
 *      Author: thuyh
 */
#include "i2c_master.h"
HAL_StatusTypeDef I2C_Transmit(I2C_HandleTypeDef* hi2c, uint16_t Slave_adress, uint8_t* pData_transmit,uint16_t size){
	return HAL_I2C_Master_Transmit_IT(hi2c, Slave_adress, pData_transmit, size);
}
HAL_StatusTypeDef I2C_Receive(I2C_HandleTypeDef* hi2c,  uint16_t Slave_adress, uint8_t* pData_recive, uint16_t size){
	return HAL_I2C_Master_Receive_IT(hi2c, Slave_adress, pData_recive, size);
}
HAL_StatusTypeDef I2C_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
		uint16_t MemAddSize, uint8_t *pData, uint16_t Size) {
	return HAL_I2C_Mem_Read_IT(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size);
}
