#include "stm32f4xx_hal.h"
#include "main.h"
#include "Drivers/ADXL345/adxl345_driver.h"
#include "Drivers/Servo/servo_driver.h"
#include "Logic/orientation.h"
#include "board.h"
#include <string.h>
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/
static void UART_Transmit(uint8_t* buffer, int len);
static void log_telemetry(orientation_t* orientation);
static void handle_error(HAL_StatusTypeDef status);

static accel_t wrist_accel;
static orientation_t wrist_orientation;
static actuator_t assist_actuator;
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  Board_Init();


  HAL_StatusTypeDef status = ADXL_Init(&hi2c1);
  if(status != HAL_OK) {
	  handle_error(status);
  }

  actuator_init(&assist_actuator, &htim2, TIM_CHANNEL_1);

  int len;

  while(1) {
	uint8_t i2c_buffer[6];
	if(ADXL_Read(&hi2c1, i2c_buffer, &wrist_accel)) {
		calculate_orientation(&wrist_accel, &wrist_orientation);
		actuator_set_level(&assist_actuator, &wrist_orientation);
		log_telemetry(&wrist_orientation);
		HAL_Delay(10);
	} else {

		handle_error(HAL_ERROR);
		UART_Transmit((uint8_t *) "Sensor Error\r\n", 14);
		HAL_Delay(100);


	}
  }
}

static void UART_Transmit(uint8_t* buffer, int len) {
	HAL_UART_Transmit(&huart2, buffer, len, 100);
}

static void log_telemetry(orientation_t* orientation) {
	char log_buf[32];
	int len = sprintf(log_buf, "P: %d\r\n", orientation->pitch);
	UART_Transmit((uint8_t*) log_buf, len);
}

static void handle_error(HAL_StatusTypeDef status) {
	char err_msg[32];
	int len = sprintf(err_msg, "Error: %d\r\n", (int) status);
	UART_Transmit((uint8_t*) err_msg, len);
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
