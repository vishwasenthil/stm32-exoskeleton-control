#include <stdint.h>
#include <math.h>
#include "stm32f4xx_hal.h"

#include "servo_driver.h"
#include "adxl345_driver.h"
#include "orientation.h"

volatile float pulse;
volatile float filtered_pulse;
float prev_filtered = 0;

void actuator_init(actuator_t* servo, TIM_HandleTypeDef* htim, uint32_t channel) {
	servo->htim = htim;
	servo->channel = channel;

	uint32_t tim_clk;
	int bus_psc;
	uint32_t tim_psc = htim->Init.Prescaler;

	if(htim->Instance == TIM1 || htim->Instance == TIM8) {
		bus_psc = HAL_RCC_GetHCLKFreq() / HAL_RCC_GetPCLK2Freq();
		tim_clk = bus_psc == 1 ? HAL_RCC_GetPCLK2Freq() : 2 * HAL_RCC_GetPCLK2Freq();
	} else {
		bus_psc = HAL_RCC_GetHCLKFreq() / HAL_RCC_GetPCLK1Freq();
		tim_clk = bus_psc == 1 ? HAL_RCC_GetPCLK1Freq() : 2 * HAL_RCC_GetPCLK1Freq();
	}

	servo->tick_period = (tim_psc + 1.0f) / tim_clk;
}
void move_to_angle(actuator_t* actuator, const int8_t angle) {
	float pulse_width_ms = ((1.0/180.0) * angle) + 1.5;
	uint16_t pulse = (pulse_width_ms * 0.001 / actuator->tick_period);
	__HAL_TIM_SET_COMPARE(actuator->htim, actuator->channel, pulse);
}

void actuator_set_level(actuator_t* actuator, orientation_t* orientation) {
	const uint32_t arr = actuator->htim->Init.Period;
	pulse = ((arr / 90.0f) * fabsf(orientation->pitch));

	float alpha = 0.93;
	filtered_pulse = alpha * prev_filtered + (1 - alpha) * pulse;
	prev_filtered = filtered_pulse;


	__HAL_TIM_SET_COMPARE(actuator->htim, actuator->channel, (uint32_t) filtered_pulse);
}
