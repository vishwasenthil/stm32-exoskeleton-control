#include "board.h"
#include "main.h"
#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart2 = {0};
TIM_HandleTypeDef htim2 = {0};
TIM_HandleTypeDef htim3 = {0};
TIM_OC_InitTypeDef sConfig = {0};
I2C_HandleTypeDef hi2c1 = {0};

static void UART_Init();
static void TIM_Init();
static void GPIO_Init();
static void I2C_Init();

void Board_Init(void) {
	SystemClock_Config();
	GPIO_Init();
	UART_Init();
	TIM_Init();
	HAL_I2C_DeInit(&hi2c1);
	I2C_Init();
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  /*
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void UART_Init(void) {
	__HAL_RCC_USART2_CLK_ENABLE();

	huart2.Instance = USART2;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.BaudRate = 115200;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
}

static void TIM_Init(void) {
	__HAL_RCC_TIM2_CLK_ENABLE();
	htim2.Instance = TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = (2000000 / PWM_FREQ) - 1;
	//htim2.Init.Period = 39999;
	htim2.Init.Prescaler = 7;
	HAL_TIM_PWM_Init(&htim2);

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 999;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

static void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure LD2 (PA5) as output
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // Push-pull output
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USART_TX_Pin | USART_RX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = I2C_SCL_Pin | I2C_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(I2C_SCL_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TIM2_CH1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(TIM2_CH1_Port, &GPIO_InitStruct);

}

static void I2C_Init(void) {
	__HAL_RCC_I2C1_CLK_ENABLE();


	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	HAL_I2C_DeInit(&hi2c1);

	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2c1);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {

  }
  /* USER CODE END Error_Handler_Debug */
}
