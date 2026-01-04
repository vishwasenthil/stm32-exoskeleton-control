#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include "orientation.h"

typedef enum {
	ARM_RIGHT,
	ARM_LEFT
} joint_type_t;

typedef struct {
	TIM_HandleTypeDef* htim;
	uint32_t duty_cycle;

	float last_position;

	joint_type_t joint_type;
	int8_t direction_multiplier;
} motor_t;

#define PITCH_THRESHOLD 0

void motor_init(motor_t* motor, TIM_HandleTypeDef* htim, joint_type_t joint_type);
void motor_move(motor_t* motor, orientation_t* orientation);

#endif
