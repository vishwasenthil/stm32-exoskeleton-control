#include "stm32f4xx_hal.h"

void logger_init(UART_HandleTypeDef *huart);
void logger_trigger(float setpoint, float current);
void log_data(void);
