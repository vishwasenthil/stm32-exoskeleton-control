#ifndef CONTROL_H
#define CONTROL_H

#include "orientation.h"
#include "motor_driver.h"

#define PWM_SAFETY_DUTY_CYCLE 0.3

void calculate_duty_cycle(motor_t* motor, orientation_t* orientation);

#endif
