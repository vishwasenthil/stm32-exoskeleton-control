#include "stm32f4xx_hal.h"

#define HARDWARE_MODE_LED

#ifdef HARDWARE_MODE_LED
	#define PWM_FREQ 1000
#else
	#define PWM_FREQ 50
#endif

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
extern TIM_OC_InitTypeDef sConfig;
extern I2C_HandleTypeDef hi2c1;

void Board_Init(void);
