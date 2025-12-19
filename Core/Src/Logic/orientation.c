#include "orientation.h"
#include <math.h>

void calculate_orientation(accel_t* accel, orientation_t* orientation) {
	int16_t x = accel->data_x;
	int16_t y = accel->data_y;
	int16_t z = accel->data_z;

	orientation->roll = atan2(-y, sqrt(pow(x, 2) + pow(z, 2))) * (180.0f / 3.14);
	orientation->pitch = atan2(-x, sqrt(pow(y, 2) + pow(z, 2))) * (180.0f / 3.14);
}
