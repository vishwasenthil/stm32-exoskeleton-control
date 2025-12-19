#include "adxl345_driver.h"
#include "orientation.h"

typedef struct {
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	float tick_period;
} actuator_t;

void actuator_init(actuator_t* servo, TIM_HandleTypeDef* htim, uint32_t channel);
void move_to_angle(actuator_t* servo, const int8_t angle);
void actuator_set_level(actuator_t* servo, orientation_t* orientatin);
