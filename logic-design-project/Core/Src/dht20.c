/*
 * dht20.c
 *
 *  Created on: Oct 12, 2024
 *      Author: thuyh
 */
#include <dht20.h>

#include "i2c_master.h"
#define Slave_address_dht20 0x38

void DHT20_Init(DHT20_t *dht20, I2C_HandleTypeDef *hi2c) {
    dht20->hi2c = hi2c;
    dht20->humidity = 0;
    dht20->temperature = 0;
    dht20->humOffset = 0;
    dht20->tempOffset = 0;
    dht20->status = DHT20_OK;
    dht20->lastRequest = 0;
    dht20->lastRead = 0;
}



uint8_t DHT20_IsConnected(DHT20_t *dht20) {
    uint8_t dummy = 0;
    if (HAL_I2C_Master_Transmit(dht20->hi2c, Slave_address_dht20 << 1, &dummy, 1, HAL_MAX_DELAY) == HAL_OK) {
        return 1;
    }
    return 0;
}

uint8_t DHT20_RequestData(DHT20_t *dht20) {
    uint8_t data[3] = {0xAC, 0x33, 0x00};
    if (HAL_I2C_Master_Transmit(dht20->hi2c, Slave_address_dht20 << 1, data, 3, HAL_MAX_DELAY) != HAL_OK) {
        return DHT20_ERROR_CONNECT;
    }
    dht20->lastRequest = HAL_GetTick();
    return DHT20_OK;
}


uint8_t DHT20_ReadData(DHT20_t *dht20) {
    uint8_t length = 7;
    if (HAL_I2C_Master_Receive(dht20->hi2c, Slave_address_dht20 << 1, dht20->bytes, length, HAL_MAX_DELAY) != HAL_OK) {
        return DHT20_ERROR_CONNECT;
    }

    // Kiểm tra nếu tất cả các byte đều bằng 0
    uint8_t allZero = 1;
    for (int i = 0; i < length; i++) {
        if (dht20->bytes[i] != 0) {
            allZero = 0;
            break;
        }
    }
    if (allZero) return DHT20_ERROR_BYTES_ALL_ZERO;

    dht20->lastRead = HAL_GetTick();
    return DHT20_OK;
}


uint8_t DHT20_Convert(DHT20_t *dht20) {
    dht20->status = dht20->bytes[0];

    // Chuyển đổi độ ẩm
    uint32_t rawHumidity = (dht20->bytes[1] << 12) | (dht20->bytes[2] << 4) | (dht20->bytes[3] >> 4);
    dht20->humidity = rawHumidity * 9.5367431640625e-5; // Chuyển đổi theo công thức datasheet

    // Chuyển đổi nhiệt độ
    uint32_t rawTemperature = ((dht20->bytes[3] & 0x0F) << 16) | (dht20->bytes[4] << 8) | dht20->bytes[5];
    dht20->temperature = rawTemperature * 1.9073486328125e-4 - 50; // Chuyển đổi theo công thức datasheet

    // Kiểm tra CRC
    uint8_t crc = DHT20_CRC8(dht20->bytes, 6);
    if (crc != dht20->bytes[6]) {
        return DHT20_ERROR_CHECKSUM;
    }

    return DHT20_OK;
}

uint8_t DHT20_Read(DHT20_t *dht20) {
    if (HAL_GetTick() - dht20->lastRead < 1000) {
        return DHT20_ERROR_LASTREAD;
    }

    int status = DHT20_RequestData(dht20);
    if (status < 0) return status;

    while (DHT20_IsMeasuring(dht20)) {
        if (HAL_GetTick() - dht20->lastRequest >= 1000) {
            return DHT20_ERROR_READ_TIMEOUT;
        }
    }

    status = DHT20_ReadData(dht20);
    if (status < 0) return status;

    return DHT20_Convert(dht20);
}

uint8_t DHT20_ReadStatus(DHT20_t *dht20) {
    uint8_t status = 0;
    HAL_I2C_Master_Receive(dht20->hi2c, Slave_address_dht20 << 1, &status, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
    return status;
}

uint8_t DHT20_IsCalibrated(DHT20_t *dht20) {
    return (DHT20_ReadStatus(dht20) & 0x08) == 0x08;
}

uint8_t DHT20_IsMeasuring(DHT20_t *dht20) {
    return (DHT20_ReadStatus(dht20) & 0x80) == 0x80;
}

uint8_t DHT20_CRC8(uint8_t *ptr, uint8_t len) {
    uint8_t crc = 0xFF;
    while (len--) {
        crc ^= *ptr++;
        for (uint8_t i = 0; i < 8; i++) {    if (crc & 0x80) {
            crc = (crc << 1) ^ 0x31;
        } else {
            crc <<= 1;
        }
    }
}
return crc;
}

