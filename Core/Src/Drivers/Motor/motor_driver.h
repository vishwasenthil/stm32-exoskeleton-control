#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include "orientation.h"

typedef struct {
	TIM_HandleTypeDef* pwm_htim;
	TIM_HandleTypeDef* encoder_htim;

	uint32_t pwm_max;
	float control_max;
	uint32_t min_pwm;

	int8_t direction_multiplier;

	bool simulation_mode;
} motor_t;

#define PITCH_THRESHOLD 0
#define PWM_SAFETY_DUTY_CYCLE 0.7



void motor_init(motor_t* motor, TIM_HandleTypeDef* pwm_htim,TIM_HandleTypeDef* encoder_htim);
void motor_move(motor_t* motor, orientation_t* orientation);

#endif
