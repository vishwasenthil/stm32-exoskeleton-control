#include "control.h"
#include <math.h>

static const float Kp = 1.0f;
static const float Kd = 1.0f;

float calculate_control(motor_t* motor, float error) {
	if(fabs(error) < 1.5f) {
		return 0.0f;
	}
	float control = (Kp * error) /motor->control_max;

	if(control > 1.0) {
		control = 1.0;
	}
	if(control < -1.0) {
		control = -1.0;
	}

	return control;
}

float counts_to_angle(int16_t counts) {
	float angle = counts * DEGREES_PER_ENCODER_COUNT;
	return angle;
}
