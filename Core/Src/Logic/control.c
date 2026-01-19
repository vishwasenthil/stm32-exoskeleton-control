#include "control.h"
#include <math.h>

static const float Kp = 1.0f;

float calculate_control(motor_t* motor, float error) {
	float control = Kp * error;
	return control;
}

float counts_to_angle(int16_t counts) {
	float angle = counts * DEGREES_PER_ENCODER_COUNT;
	return angle;
}
