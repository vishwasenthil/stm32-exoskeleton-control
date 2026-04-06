#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "adxl345_driver.h"

HAL_StatusTypeDef ADXL_Init(I2C_HandleTypeDef* hi2c) {
	uint8_t measure_mode = ADXL_MEASURE_MODE;
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, DEVICE_ADDRESS << 1, 3, 100);

	if(status != HAL_OK) return status;

	status = HAL_I2C_Mem_Write(hi2c, DEVICE_ADDRESS << 1, POWERCTL_ADDRESS, I2C_MEMADD_SIZE_8BIT, &measure_mode, 1, 100);

	if(status != HAL_OK) return status;

	return status;
}

bool ADXL_Read(I2C_HandleTypeDef* hi2c, accel_t* accel) {

	HAL_StatusTypeDef status;
	uint8_t i2c_buffer[6];

	status = HAL_I2C_Mem_Read(hi2c, DEVICE_ADDRESS << 1, DATAX0_ADDRESS, I2C_MEMADD_SIZE_8BIT, i2c_buffer, 6, 100);

	if(status != HAL_OK) return false;

	accel->data_x = i2c_buffer[1] << 8 | i2c_buffer[0];
	accel->data_y = i2c_buffer[3] << 8 | i2c_buffer[2];
	accel->data_z = i2c_buffer[5] << 8 | i2c_buffer[4];

	return true;
}
