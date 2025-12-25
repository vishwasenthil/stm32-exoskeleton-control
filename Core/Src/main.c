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
static void log_telemetry(orientation_t* orientation, accel_t* accel);
static void handle_error(HAL_StatusTypeDef status);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

volatile static accel_t wrist_accel;
volatile static orientation_t wrist_orientation;
static actuator_t assist_actuator;

volatile float execution_time;
float total_latency;

volatile bool sensor_error_flag = false;

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

  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  uint32_t start, end;
  uint32_t total_cycles;

  while(1) {
	if(sensor_error_flag) {
		handle_error(HAL_ERROR);
		HAL_Delay(100);
	} else {
		MEASURE_LATENCY();
		log_telemetry(&wrist_orientation, &wrist_accel);
		HAL_Delay(10);

		uint32_t cnt = TIM3->CNT;
		char buf[32];
		int len = sprintf(buf, "CNT=%lu\r\n", cnt);
		UART_Transmit((uint8_t*)buf, len);
		HAL_Delay(100); // 10Hz print
	}
  }
}

static void UART_Transmit(uint8_t* buffer, int len) {
	HAL_UART_Transmit(&huart2, buffer, len, 100);
}

static void log_telemetry(orientation_t* orientation, accel_t* accel) {
	char log_buf[32];
	int len = sprintf(log_buf, "P: %d\r\n", accel->data_x);
	UART_Transmit((uint8_t*) log_buf, len);
}

static void handle_error(HAL_StatusTypeDef status) {
	char err_msg[32];
	int len = sprintf(err_msg, "Error: %d\r\n", (int) status);
	UART_Transmit((uint8_t*) err_msg, len);
}

void measure_latency() {

	static uint32_t latency_start, latency_end;
	static bool movement_detected = false;
	static bool ready_for_test = true;

	if(ready_for_test && !movement_detected && pulse > (filtered_pulse + 100)) {
		latency_start = DWT->CYCCNT;
		movement_detected = true;
		ready_for_test = false;
	}

	if(movement_detected && filtered_pulse >= (pulse * 0.9f)){
		latency_end = DWT->CYCCNT;

		total_latency = (latency_end - latency_start) / 16000.0f;

		movement_detected = false;
	}

	if(!movement_detected && pulse < 20) {
		ready_for_test = true;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM3) {
		uint8_t i2c_buffer[6];
		MEASURE_START();
		if(ADXL_Read(&hi2c1, i2c_buffer, &wrist_accel)) {
			sensor_error_flag = false;
			calculate_orientation(&wrist_accel, &wrist_orientation);
			actuator_set_level(&assist_actuator, &wrist_orientation);
			MEASURE_END(execution_time);
		} else {
			sensor_error_flag = true;
		}
	}
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
