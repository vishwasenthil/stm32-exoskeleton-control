#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f4xx_hal.h"
#include "orientation.h"

typedef struct {
	TIM_HandleTypeDef* pwm_htim;
	TIM_HandleTypeDef* encoder_htim;

	uint32_t pwm_max;
	uint32_t start_time;
	uint32_t end_time;
	float control_max;

	int8_t direction_multiplier;

	bool is_kicking;
	bool is_idle;
	bool is_tracking;
} motor_t;

#define PITCH_THRESHOLD 0
#define PWM_SAFETY_DUTY_CYCLE 0.7f



void motor_init(motor_t* motor, TIM_HandleTypeDef* pwm_htim,TIM_HandleTypeDef* encoder_htim);
float read_encoder(motor_t* motor);
void motor_set_control(motor_t* motor, float control);
uint16_t get_current_sense(ADC_HandleTypeDef* current_sense_adc);

#endif
