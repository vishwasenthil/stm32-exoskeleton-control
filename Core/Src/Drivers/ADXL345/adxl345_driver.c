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

bool ADXL_Read(I2C_HandleTypeDef* hi2c, uint8_t* data, accel_t* accel) {

	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Read(hi2c, DEVICE_ADDRESS << 1, DATAX0_ADDRESS, I2C_MEMADD_SIZE_8BIT, data, 6, 100);

	if(status != HAL_OK) return false;

	uint16_t x0 = *(data);
	uint16_t x1 = *(data + 1);
	uint16_t y0 = *(data + 2);
	uint16_t y1 = *(data + 3);
	uint16_t z0 = *(data + 4);
	uint16_t z1 = *(data + 5);


	accel->data_x = (x1 << 8) | x0;
	accel->data_y = (y1 << 8) | y0;
	accel->data_z = (z1 << 8) | z0;

	return true;
}
