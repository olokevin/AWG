/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int16_t DAC_Value; 
int16_t ADC_Value;  
uint16_t keyIO[5];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM15_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	//User Init
  LCD_Init();
  LCD_Clear(BLACK);
//	TFT_ShowString(20,20,"ADC Reading:", 8, 16, 0, BLACK, WHITE);
//	TFT_ShowString(20,40,"DAC Voltage:", 8, 16, 0, BLACK, WHITE);
//	TFT_ShowString(20,60,"Key_1:", 8, 16, 0, BLACK, WHITE);
//	TFT_ShowString(20,80,"Key_2:", 8, 16, 0, BLACK, WHITE);
//	TFT_ShowString(20,100,"Key_R:", 8, 16, 0, BLACK, WHITE);
//	TFT_ShowString(20,120,"Key_O:", 8, 16, 0, BLACK, WHITE);
//	TFT_ShowString(20,140,"Key_L:", 8, 16, 0, BLACK, WHITE);
	
	AWG_dataInit();
	AWG_cellInit();
	
	PWM_dataInit(&pwm);
	PWM_cellInit(&pwm);
	
	AWG_interfaceInit();
	
	HAL_TIM_Base_Start(&htim3);
	
	__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim7);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
		
	HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)sine_data,SAMPLE_POINTS,DAC_ALIGN_12B_R);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		read_button(&button_state);
		modeCheck();
		
//		keyIO[0] = HAL_GPIO_ReadPin(Key_1_GPIO_Port, Key_1_Pin);
//		keyIO[1] = HAL_GPIO_ReadPin(Key_2_GPIO_Port, Key_2_Pin);
//		keyIO[2] = HAL_GPIO_ReadPin(Key_R_GPIO_Port, Key_R_Pin);
//		keyIO[3] = HAL_GPIO_ReadPin(Key_O_GPIO_Port, Key_O_Pin);
//		keyIO[4] = HAL_GPIO_ReadPin(Key_L_GPIO_Port, Key_L_Pin);
//		
//		TFT_ShowNum(0,10,Key_1.button_flag,1,8,16, BLACK, WHITE);
//		TFT_ShowNum(20,10,Key_2.button_flag,1,8,16, BLACK, WHITE);
//		TFT_ShowNum(40,10,Key_R.button_flag,1,8,16, BLACK, WHITE);
//		TFT_ShowNum(60,10,Key_O.button_flag,1,8,16, BLACK, WHITE);
//		TFT_ShowNum(80,10,Key_L.button_flag,1,8,16, BLACK, WHITE);
//		
//		TFT_ShowNum(10,30,cursor_x,1,8,16, BLACK, WHITE);
//		TFT_ShowNum(30,30,cursor_y,1,8,16, BLACK, WHITE);
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
