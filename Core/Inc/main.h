/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

void Error_Handler(void);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define I2C_SCL_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SDA_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define TIM2_CH1_Port GPIOA
#define TIM2_CH1_Pin GPIO_PIN_0

#define TIMING_ENABLED 1

#if TIMING_ENABLED
	#define MEASURE_START() uint32_t start = DWT->CYCCNT;
	#define MEASURE_END(out) out = (DWT->CYCCNT - start) / 16.0f;
	#define MEASURE_LATENCY() measure_latency()
#else
#define MEASURE_START()
#define MEASURE_END()
#define MEASURE_LATENCY()
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
