#ifndef CONTROL_H
#define CONTROL_H

#include "orientation.h"
#include "motor_driver.h"

#define ENCODER_COUNTS_PER_REV 64
#define DEGREES_PER_ENCODER_COUNT 360.0f / ENCODER_COUNTS_PER_REV

float calculate_control(motor_t* motor, float error);
float counts_to_angle(int16_t counts);

#endif
