#include "control.h"
#include "motor_driver.h";
#include <math.h>

static const float Kp = 0.05f;

#define INITIAL_KICK_POWER 0.3
#define ERROR_DEADBAND 1.5f
#define KICK_PERIOD 20
#define MOVEMENT_DEADBAND 3.0f
#define MOTOR_MOVING_THRESHOLD 2000
#define ARM_MOVING_THRESHOLD 50

float last_pitch;
float last_encoder;
float current;
float error;
float control = 0.0f;

bool motor_stalled(float current_encoder, float last_encoder, float current_pitch, float last_pitch) {
	float motor_vel = (current_encoder - last_encoder) / 0.005f;
	float pitch_vel = (current_pitch - last_pitch) / 0.005f;

	return fabs(motor_vel) > MOTOR_MOVING_THRESHOLD && fabs(pitch_vel) < ARM_MOVING_THRESHOLD; // If the motor shaft's moving but the arm's not moving, then the gearbox shaft is stuck
}

void calculate_control(motor_t* motor, orientation_t* orientation) {

	float target = orientation->pitch;
	current = read_encoder(motor);
	error = (target - current);

	if(fabsf(error) < ERROR_DEADBAND) {
		control = 0.0f;
		motor->is_idle = true;
	} else {
		control = Kp * error;
		motor->is_idle = false;
	}

	if(control < -1.0f) {
		control = -1.0f;
	}
	else if(control > 1.0f) {
		control = 1.0f;
	}

	motor_set_control(motor, control);

}

float counts_to_angle(int16_t counts) {
	float angle = counts * DEGREES_PER_ENCODER_COUNT;
	return angle;
}
