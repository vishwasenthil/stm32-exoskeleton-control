#ifndef CONTROL_H
#define CONTROL_H

#include "orientation.h"
#include "motor_driver.h"

#define ENCODER_COUNTS_PER_REV 2797
#define NUM_EDGES 4
#define GEAR_RATIO 43.8f

#define DEGREES_PER_ENCODER_COUNT 360.0f / (ENCODER_COUNTS_PER_REV)

void calculate_control(motor_t* motor, orientation_t* orientation);
float counts_to_angle(int16_t counts);

#endif
