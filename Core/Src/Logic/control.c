#include "motor_driver.h"
#include "logger.h"
#include <math.h>

static const float Kp = 0.07f;
static const float Kd = 0.001f;
static const float Ki = 0.02f;
static const float Kv = 0.03f; // 80% power at target velocity 30

#define INITIAL_KICK_POWER 0.3
#define ERROR_DEADBAND 0.2f
#define KICK_PERIOD 20
#define MOVEMENT_DEADBAND 3.0f
#define MOTOR_MOVING_THRESHOLD 2000
#define ARM_MOVING_THRESHOLD 50
#define MOTOR_STALL_CURRENT 7 // TODO: Change motor stall current

#define ENCODER_COUNTS_PER_REV 2797
#define NUM_EDGES 4
#define GEAR_RATIO 43.8f

#define DEGREES_PER_ENCODER_COUNT 360.0f / (ENCODER_COUNTS_PER_REV)

static const float FOREARM_LENGTH = 0.3f;
static const float PULLEY_RADIUS = 0.008f;
static const float CABLE_PULLED_PER_REV = 2 * M_PI * PULLEY_RADIUS;
static const float ANGLES_PER_REV = CABLE_PULLED_PER_REV / FOREARM_LENGTH;
static const float ANGLES_PER_REV_DEGREES = ANGLES_PER_REV * (180.0f / M_PI);


// Debugging globals, change to static
float error;
float last_error;
float control = 0.0f;
float current;
float motor_rotations;
float angle;
float target;
float last_target;

float raw_control;

float integral_error = 0.0f;

static void motor_stalled(ADC_HandleTypeDef* current_sense_adc, motor_t* motor);
void calculate_control(motor_t* motor, orientation_t* orientation);
static float counts_to_angle(int16_t counts);

float simulate_target() {
	static float current_position = 0.0f;
	float sim_target = 90.0f;
	float degrees_per_second = 30.0f;
	float ramp_step = degrees_per_second * 0.005;

	if(current_position < sim_target) {
		current_position += ramp_step;
	}
	return current_position;
}
/*
static void handle_stall(motor_t* motor, float current_pos) {
	if(fabsf(last_current - current) < MOVEMENT_DEADBAND && fabsf(control) > MIN_CONTROL) {
		verify_move_counter++;

		if(verify_move_counter > 20) {
			motor->check_stall = true;
		}
	} else {
		verify_move_counter = 0;
		motor->check_stall = false;
	}

	if(motor->check_stall) {
		if(motor->current_sense_reading > MOTOR_STALL_CURRENT) {
			motor->is_stalled = true;
			motor->check_stall = false;
			control = 0.0f;
		}
	}
}
*/
void calculate_control(motor_t* motor, orientation_t* orientation) {

	// float target = orientation->pitch;
	target = simulate_target();
	current = counts_to_angle(read_encoder(motor));
	error = (target - current);

	logger_trigger(target, current);

	if(fabsf(control) < 1.0f) {
		integral_error += (error * 0.005);
	}


	if(fabsf(error) < ERROR_DEADBAND) {
		control = 0.0f;
		motor->is_idle = true;
	} else {
		control = (Kp * error) + (Ki * integral_error) + (Kd * ((error - last_error) / 0.005)) + (Kv * ((target - last_target) / 0.005)); // TODO: Get rid of magic number
		motor->is_idle = false;
	}

	raw_control = control;

	if(control < -1.0f) {
		control = -1.0f;
	}
	else if(control > 1.0f) {
		control = 1.0f;
	}

	// handle_stall(motor, current);

	last_error = error;
	last_target = target;
	/*
	if(motor->is_stalled) {
		stop_motor(motor);
	} else {
		motor_set_control(motor, control);
	}
	*/
	motor_set_control(motor, control);

}

static float counts_to_angle(int16_t counts) {
	// float angle = counts * DEGREES_PER_ENCODER_COUNT;
	motor_rotations = (float) counts / ENCODER_COUNTS_PER_REV;
	angle = motor_rotations * ANGLES_PER_REV_DEGREES;
	return angle;
}
