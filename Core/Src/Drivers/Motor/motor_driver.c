#include "motor_driver.h"
#include "control.h"
#include "main.h"
#include <math.h>

void motor_init(motor_t* motor, TIM_HandleTypeDef* htim) {
	motor->htim = htim;
}

static bool position_changed(motor_t* motor, orientation_t* orientation) {
	return fabsf(orientation->pitch - motor->last_position) > PITCH_THRESHOLD;
}

static void move_clockwise(motor_t* motor) {
	TIM_HandleTypeDef* timer = motor->htim;

	HAL_GPIO_WritePin(R_EN_GPIO_Port, R_EN_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L_EN_GPIO_Port, L_EN_GPIO_Pin, GPIO_PIN_SET);

	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, motor->duty_cycle);
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);
}

static void move_counterclockwise(motor_t* motor) {
	TIM_HandleTypeDef* timer = motor->htim;

	HAL_GPIO_WritePin(R_EN_GPIO_Port, R_EN_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L_EN_GPIO_Port, L_EN_GPIO_Pin, GPIO_PIN_SET);

	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, motor->duty_cycle);
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, 0);

}

void motor_move(motor_t* motor, orientation_t* orientation) {
	if(!position_changed(motor, orientation)) {
		return;
	}

	calculate_duty_cycle(motor, orientation);
	if(motor->last_position > orientation->pitch) {
		move_counterclockwise(motor);
	} else {
		move_clockwise(motor);
	}

	motor->last_position = orientation->pitch;
}
