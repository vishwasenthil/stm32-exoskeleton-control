#include "stm32f4xx_hal.h"
#include "main.h"
#include "Drivers/ADXL345/adxl345_driver.h"
#include "Drivers/Servo/servo_driver.h"
#include "Drivers/Motor/motor_driver.h"
#include "Logic/orientation.h"
#include "board.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/
void UART_Transmit(uint8_t* buffer, int len);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void measure_latency(void);

static accel_t wrist_accel;
static orientation_t wrist_orientation;
static motor_t assist_actuator;

static volatile bool wrist_data_ready = false;
static volatile bool recover_from_stall = false;
bool sensor_error_flag = false;

float execution_time;
float total_latency;

static int state;
static int state2;

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
	  log_error(status);
  }

  motor_init(&assist_actuator, &htim2, &htim4);

  //actuator_init(&assist_actuator, &htim2, TIM_CHANNEL_1);

  CoreDebug->DEMCR  |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  uint32_t start, end;
  uint32_t total_cycles;

  logger_init(&huart2);

  while(1) {
	  if(wrist_data_ready) {
		  wrist_data_ready = false;
		  uint16_t motor_current_reading = get_current_sense(&hadc1);
		  assist_actuator.current_sense_reading = motor_current_reading;
		  MEASURE_START();
		  if(ADXL_Read(&hi2c1, &wrist_accel)) {
			  sensor_error_flag = false;
			  calculate_orientation(&wrist_accel, &wrist_orientation);
			  calculate_control(&assist_actuator, &wrist_orientation);

			  /*
			  char buf[20];
			  state = HAL_GPIO_ReadPin(ENCODER_CH1_Port, ENCODER_CH1_Pin);
			  state2 = HAL_GPIO_ReadPin(ENCODER_CH2_Port, ENCODER_CH2_Pin);
			  int len = sprintf(buf, "Pin State: %d", state);
			  UART_Transmit((uint8_t*) buf, len);
			  */

			  //actuator_set_level(&assist_actuator, &wrist_orientation);
			  MEASURE_END(execution_time);
		  } else {
			  sensor_error_flag = true;
		  }
	  }

	if(sensor_error_flag) {
		log_error(HAL_ERROR);
		HAL_Delay(100);
	} else {
		MEASURE_LATENCY();
		log_telemetry();
	}
	}
}

void measure_latency(void) {

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
		if(assist_actuator.is_stalled) {
			wrist_data_ready = false;
			recover_from_stall = true;
		} else {
			recover_from_stall = false;
			wrist_data_ready = true;
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
