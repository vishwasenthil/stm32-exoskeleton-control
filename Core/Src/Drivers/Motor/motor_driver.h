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

	uint32_t start_time;
	uint32_t end_time;
	bool is_kicking;
	bool is_idle;
	bool is_tracking;
} motor_t;

#define PITCH_THRESHOLD 0
#define PWM_SAFETY_DUTY_CYCLE 0.7



void motor_init(motor_t* motor, TIM_HandleTypeDef* pwm_htim,TIM_HandleTypeDef* encoder_htim);
float read_encoder(motor_t* motor);
void motor_move(motor_t* motor, orientation_t* orientation);

#endif
