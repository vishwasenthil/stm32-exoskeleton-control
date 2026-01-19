#include "motor_driver.h"
#include "control.h"
#include "main.h"
#include <math.h>

static volatile float measured_position;
static uint16_t position;

void motor_init(motor_t* motor, TIM_HandleTypeDef* pwm_htim,TIM_HandleTypeDef* encoder_htim, joint_type_t joint_type) {
	motor->pwm_htim = pwm_htim;
	motor->encoder_htim = encoder_htim;
	motor->joint_type = joint_type;

	motor->pwm_max = __HAL_TIM_GET_AUTORELOAD(motor->pwm_htim);
	motor->control_max = 90.0f;

	switch(joint_type) {
	case ARM_RIGHT:
		motor->direction_multiplier = 1;
		break;
	case ARM_LEFT:
		motor->direction_multiplier = -1;
		break;
	}
}

/*
static bool position_changed(motor_t* motor, orientation_t* orientation) {
	return fabsf(orientation->pitch - motor->last_position) > PITCH_THRESHOLD;
}
*/

static void move_clockwise(motor_t* motor, uint32_t duty_cycle) {
	TIM_HandleTypeDef* timer = motor->pwm_htim;

	HAL_GPIO_WritePin(R_EN_GPIO_Port, R_EN_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L_EN_GPIO_Port, L_EN_GPIO_Pin, GPIO_PIN_SET);

	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, duty_cycle);
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);
}

static void move_counterclockwise(motor_t* motor, uint32_t duty_cycle) {
	TIM_HandleTypeDef* timer = motor->pwm_htim;

	HAL_GPIO_WritePin(R_EN_GPIO_Port, R_EN_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L_EN_GPIO_Port, L_EN_GPIO_Pin, GPIO_PIN_SET);

	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, duty_cycle);
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, 0);

}

static float read_encoder(motor_t* motor) {
	position = __HAL_TIM_GET_COUNTER(motor->encoder_htim);

	float angle = counts_to_angle(position);

	return angle;

}

void motor_move(motor_t* motor, orientation_t* orientation) {

	measured_position = read_encoder(motor);

	uint32_t duty_cycle = 0.3 * 249;

	move_clockwise(motor, duty_cycle);

	/*
	float target = orientation->pitch;
	float current = measured_position;
	float error = target - current;

	float control = calculate_control(motor, error);

	uint32_t duty_cycle = (fabs(control) / motor->control_max) * motor->pwm_max * PWM_SAFETY_DUTY_CYCLE;

	if(error < 0) {
		move_clockwise(motor, duty_cycle);
	} else {
		move_counterclockwise(motor, duty_cycle);
	}
	*/

	/*
	if(!position_changed(motor, orientation)) {
		return;
	}
	*/

	/*
	float target = orientation->pitch;
	float current = measured_position;
	float error = target - current;

	float control = calculate_control(motor, error);

	uint32_t duty_cycle = (fabs(control) / 90.0f) * 249.0f * PWM_SAFETY_DUTY_CYCLE;

	if(error * motor->direction_multiplier < 0) {
		move_clockwise(motor, duty_cycle);
	} else {
		move_counterclockwise(motor, duty_cycle);
	}
	*/
}

