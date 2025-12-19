#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "adxl345_driver.h"

typedef struct {
	float roll;
	float pitch;
} orientation_t;

void calculate_orientation(accel_t* accel, orientation_t* orientation);

#endif
