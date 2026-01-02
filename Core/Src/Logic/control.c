#include "control.h"
#include <math.h>

void calculate_duty_cycle(motor_t* motor, orientation_t* orientation) {
	float pitch_change = orientation->pitch - motor->last_position;

	float duty_cycle = fabsf(((pitch_change) / 90.0f)) * 249.0f * PWM_SAFETY_DUTY_CYCLE;
	motor->duty_cycle = duty_cycle;
}
