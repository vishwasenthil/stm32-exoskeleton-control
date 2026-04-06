#include "logger.h"
#include <stdbool.h>;

static UART_HandleTypeDef* log_uart;
static float motor_setpoint;
static float motor_actual;
static uint32_t time;
static bool data_ready = false;

void logger_init(UART_HandleTypeDef *huart) {
	log_uart = huart;
}
void logger_trigger(float setpoint, float current) {
	motor_setpoint = setpoint;
	motor_actual = current;
	time = HAL_GetTick();
	data_ready = true;
}
void log_telemetry(void) {
	if(data_ready) {
		data_ready = false;
		char buf[128];
		int len = sprintf(buf, "%d, %.2f, %.2f\r\n", time, motor_setpoint, motor_actual);
		HAL_UART_Transmit(log_uart, (uint8_t*) buf, len, 10);
	}
}

void log_error(HAL_StatusTypeDef status) {
	char err_msg[32];
	int len = sprintf(err_msg, "Error: %d\r\n", (int) status);
	HAL_UART_Transmit(log_uart, (uint8_t*) err_msg, len, 10);
}
