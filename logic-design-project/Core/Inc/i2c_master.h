/*
 * i2c_master.h
 *
 *  Created on: Oct 12, 2024
 *      Author: thuyh
 */

#ifndef INS_I2C_MASTER_H_
#define INS_I2C_MASTER_H_
#include "main.h"
HAL_StatusTypeDef I2C_Transmit(I2C_HandleTypeDef* hi2c, uint16_t Slave_adress, uint8_t* pData_transmit,uint16_t size);
HAL_StatusTypeDef I2C_Receive(I2C_HandleTypeDef* hi2c,  uint16_t Slave_adress, uint8_t* pData_recive, uint16_t size);
HAL_StatusTypeDef I2C_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
		uint16_t MemAddSize, uint8_t *pData, uint16_t Size) ;
#endif /* INS_I2C_MASTER_H_ */
