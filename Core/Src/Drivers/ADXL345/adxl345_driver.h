#ifndef ADXL345_DRIVER_H
#define ADXL345_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

#define DEVICE_ADDRESS 0x53
#define POWERCTL_ADDRESS 0x2D
#define DATAX0_ADDRESS 0x32
#define ADXL_MEASURE_MODE 0x08

typedef struct {
	int16_t data_x;
	int16_t data_y;
	int16_t data_z;
} accel_t;

HAL_StatusTypeDef ADXL_Init(I2C_HandleTypeDef* hi2c1);
bool ADXL_Read(I2C_HandleTypeDef* hi2c1, uint8_t* data, accel_t* accel);
bool ADXL_Write();

#endif
