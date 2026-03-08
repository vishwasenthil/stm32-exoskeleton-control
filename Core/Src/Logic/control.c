#include "control.h"
#include "motor_driver.h"
#include <math.h>

static const float Kp = 0.05f;

#define INITIAL_KICK_POWER 0.3
#define ERROR_DEADBAND 1.5f
#define KICK_PERIOD 20
#define MOVEMENT_DEADBAND 3.0f
#define MOTOR_MOVING_THRESHOLD 2000
#define ARM_MOVING_THRESHOLD 50
#define MOTOR_STALL_CURRENT 7 // TODO: Change motor stall current

static const float FOREARM_LENGTH = 0.3f;
static const float PULLEY_RADIUS = 0.008f;
static const float CABLE_PULLED_PER_REV = 2 * M_PI * PULLEY_RADIUS;
static const float ANGLES_PER_REV = CABLE_PULLED_PER_REV / FOREARM_LENGTH;
static const float ANGLES_PER_REV_DEGREES = ANGLES_PER_REV * (180.0f / M_PI);

float last_pitch;
float last_encoder;
float error;
float control = 0.0f;

// Debugging globals, change to static
float current;
float motor_rotations;
float angle;

static float counts_to_angle(int16_t counts);

void motor_stalled(ADC_HandleTypeDef* current_sense_adc, motor_t* motor) {
	uint16_t motor_current = get_current_sense(current_sense_adc);

	if(motor_current > MOTOR_STALL_CURRENT) { // Change motor stall current
		stop_motor(motor);
		motor->is_stalled = true;
	}
}

void calculate_control(motor_t* motor, orientation_t* orientation) {

	float target = orientation->pitch;
	current = counts_to_angle(read_encoder(motor));
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

static float counts_to_angle(int16_t counts) {
	// float angle = counts * DEGREES_PER_ENCODER_COUNT;
	motor_rotations = (float) counts / ENCODER_COUNTS_PER_REV;
	angle = motor_rotations * ANGLES_PER_REV_DEGREES;
	return angle;
}
