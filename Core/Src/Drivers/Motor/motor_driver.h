#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include "orientation.h"

typedef enum {
	ARM_RIGHT,
	ARM_LEFT
} joint_type_t;

typedef struct {
	TIM_HandleTypeDef* pwm_htim;
	TIM_HandleTypeDef* encoder_htim;

	uint32_t pwm_max;
	float control_max;

	joint_type_t joint_type;
	int8_t direction_multiplier;

	bool simulation_mode;
} motor_t;

#define PITCH_THRESHOLD 0
#define PWM_SAFETY_DUTY_CYCLE 0.3 * 249



void motor_init(motor_t* motor, TIM_HandleTypeDef* pwm_htim,TIM_HandleTypeDef* encoder_htim, joint_type_t joint_type);
void motor_move(motor_t* motor, orientation_t* orientation);

#endif
