#include "control.h"
#include "motor_driver.h";
#include <math.h>

static const float Kp = 1.0f;

#define INITIAL_KICK_POWER 0.3
#define ERROR_DEADBAND 1.5f
#define MOVEMENT_DEADBAND 5.0f
#define KICK_PERIOD 8
#define MOVEMENT_DEADBAND 3.0f
#define MOTOR_MOVING_THRESHOLD 2000
#define ARM_MOVING_THRESHOLD 50

typedef enum {
	STATE_IDLE,
	STATE_INITIAL_PUSH,
	STATE_TRACKING
} MotorState_t;

MotorState_t motor_state = STATE_IDLE;

float last_pitch;
float last_encoder;
float current;

bool motor_stalled(float current_encoder, float last_encoder, float current_pitch, float last_pitch) {
	float motor_vel = (current_encoder - last_encoder) / 0.005f;
	float pitch_vel = (current_pitch - last_pitch) / 0.005f;

	return fabs(motor_vel) > MOTOR_MOVING_THRESHOLD && fabs(pitch_vel) < ARM_MOVING_THRESHOLD; // If the motor shaft's moving but the arm's not moving, then the gearbox shaft is stuck
}

float calculate_control(motor_t* motor, orientation_t* orientation) {
	/*
	Move the output shaft to the target angle.
	Every cycle, compute position er	ror.
	If the system is not moving when it should be, apply a strong torque to break stiction.
	Once motion is proven, reduce torque and track the target proportionally.
	If motion stops again before reaching the target, re-apply the stiction-breaking torque.
	Stop when close enough.
	*/

	float target = orientation->pitch;
	current = read_encoder(motor);
	float error = target - current;

	float control = 0.0f;

	if(fabs(last_pitch - target) > MOVEMENT_DEADBAND && !motor->is_kicking && !motor->is_tracking) { // Define MOVEMENT_DEADBAND
		motor->is_kicking = true;
		motor->start_time = HAL_GetTick();
	}

	if(motor->is_kicking) {
		control = error > 0 ? INITIAL_KICK_POWER : -INITIAL_KICK_POWER;

		if(HAL_GetTick() - motor->start_time > KICK_PERIOD) { // Define KICK_PERIOD
			motor->is_kicking = false;

			if(fabs(error) > ERROR_DEADBAND) {
				motor->is_tracking = true;
			}
		}
	}

	else if(motor->is_tracking) {
		control = (Kp * error) / motor->control_max;

		if(motor_stalled(current, last_encoder, target, last_pitch)) {
			motor->is_idle = true;
			motor->is_tracking = false;
		}
	}

	if(fabs(error) < ERROR_DEADBAND) {
		control = 0.0f;
		motor->is_idle = true;
	}

	if(!motor->is_idle) {
		last_pitch = target;
	}
	last_encoder = current;

	motor_set_control(motor, control);
}

float counts_to_angle(int16_t counts) {
	float angle = counts * DEGREES_PER_ENCODER_COUNT;
	return angle;
}
