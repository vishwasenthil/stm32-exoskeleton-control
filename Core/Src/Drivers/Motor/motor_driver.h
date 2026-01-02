#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include "orientation.h"

typedef struct {
	TIM_HandleTypeDef* htim;
	uint32_t duty_cycle;

	uint32_t last_position;
} motor_t;

#define PITCH_THRESHOLD 0

void motor_init(motor_t* motor, TIM_HandleTypeDef* htim);
void motor_move(motor_t* motor, orientation_t* orientation);

#endif
