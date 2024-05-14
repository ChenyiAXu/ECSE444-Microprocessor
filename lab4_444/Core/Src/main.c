/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_qspi.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_magneto.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define WRITE_READ_ADDR 0x20
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

QSPI_HandleTypeDef hqspi;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_QUADSPI_Init(void);
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
	float temperature = 0;
	float pressure = 0;
	float humidity = 0;

	char message[100] = "";
	char message1[100] = "";
	char message2[100] = "";
	char message3[100] = "";
	char message4[100] = "";
	char message5[100]= "";
	char message6[100]= "";
	char message7[100]="";

	int16_t pDataXYZ[3];
	int16_t pDataXYZMagnet[3];
	float pfData=0;
	//part 3

	uint8_t example_arr[5] = {0, 1, 2, 3, 4};
	uint8_t example_copy[5] = {0, 0, 0, 0, 0};

	uint8_t example_arr2[3] = {5, 6, 7};
	uint8_t example_copy2[8];
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
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
  BSP_TSENSOR_Init();
  BSP_PSENSOR_Init();
  BSP_ACCELERO_Init();
  BSP_HSENSOR_Init();
  BSP_GYRO_Init();
  BSP_MAGNETO_Init();
  BSP_QSPI_Init();


  //part 1 pushbutton
  GPIO_PinState lastButtonState = GPIO_PIN_SET; // Initialize to released state
  uint32_t button_counter =0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 temperature = BSP_TSENSOR_ReadTemp();
	 pressure = BSP_PSENSOR_ReadPressure();
	 humidity = BSP_HSENSOR_ReadHumidity();
	 BSP_ACCELERO_AccGetXYZ(pDataXYZ);
	 BSP_GYRO_GetXYZ(&pfData);
	 BSP_MAGNETO_GetXYZ(pDataXYZMagnet);


	 //part 1
	 //read push button state
	 GPIO_PinState button = HAL_GPIO_ReadPin(PushButton_GPIO_Port, PushButton_Pin);
	 if (button == GPIO_PIN_RESET && lastButtonState == GPIO_PIN_SET) {
		          // Button was pressed
		          button_counter++;
		          if (button_counter > 1) {
		              button_counter = 1;
		          }
		      }

	lastButtonState = button;
	if(button_counter ==1){
//		 snprintf(message, 50, "Temperature = %f\r\n", temperature);
//		 HAL_UART_Transmit(&huart1, (uint8_t *)message, sizeof(message), 10);
		 snprintf(message3, 50, "%d, %d, %d\r\n", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
		 HAL_UART_Transmit(&huart1, (uint8_t *)message3, sizeof(message3), 10);
	}
//	else if (button_counter ==2){
//
//		 snprintf(message1, 50, "Pressure = %f\r\n", pressure);
//		 HAL_UART_Transmit(&huart1, (uint8_t *)message1, sizeof(message1), 10);
//	}else if(button_counter ==3){
//
//		 snprintf(message2, 50, "Humidity = %f\r\n", humidity);
//		 HAL_UART_Transmit(&huart1, (uint8_t *)message2, sizeof(message2), 10);
//	}else if(button_counter == 4){
//		 snprintf(message3, 50, "Accelero X = %d, Y = %d, Z = %d\r\n", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
//		 HAL_UART_Transmit(&huart1, (uint8_t *)message3, sizeof(message3), 10);
//	}else if(button_counter == 5){
//		  snprintf(message4, sizeof(message4), "Gyro = %f\r\n", pfData);
//		  HAL_UART_Transmit(&huart1, (uint8_t *)message4, sizeof(message4), 10);
//	}else{
//		snprintf(message5, sizeof(message5), "Magnet X = %d, Y = %d, Z = %d\r\n", pDataXYZMagnet[0], pDataXYZMagnet[1], pDataXYZMagnet[2]);
//		HAL_UART_Transmit(&huart1, (uint8_t *)message5, sizeof(message5), 10);
//	}
	HAL_Delay(38);

	  //part 3
//	HAL_GPIO_WritePin(LEDError_GPIO_Port, LEDError_Pin, GPIO_PIN_SET);
//	if(BSP_QSPI_Erase_Block((uint8_t)0x00) != QSPI_OK)
//		Error_Handler();
//	if(BSP_QSPI_Write(example_arr, (uint32_t) WRITE_READ_ADDR, sizeof(example_arr)) != QSPI_OK)
//		Error_Handler();
//	if(BSP_QSPI_Read(example_copy, (uint32_t) WRITE_READ_ADDR, sizeof(example_copy)) != QSPI_OK){
//		Error_Handler();
//	}
////	snprintf(message6, sizeof(message6), "copy[0] = %d, copy[1] = %d, copy[2] = %d, copy[3] = %d, copy[4] = %d\r\n", example_copy[0], example_copy[1], example_copy[2],example_copy[3],example_copy[4]);
////	HAL_UART_Transmit(&huart1, (uint8_t *)message6, sizeof(message6), 10);
//
//	if(BSP_QSPI_Write(example_arr2, (uint32_t) WRITE_READ_ADDR + sizeof(example_arr) , sizeof(example_arr2)) != QSPI_OK)
//		Error_Handler();
//	if(BSP_QSPI_Read(example_copy2, (uint32_t) WRITE_READ_ADDR, sizeof(example_copy2)) != QSPI_OK)
//		Error_Handler();
////	snprintf(message7, sizeof(message7), "copy2[0] = %d, copy[1] = %d, copy[2] = %d\r\n", example_copy2[5], example_copy2[6], example_copy2[7]);
////	HAL_UART_Transmit(&huart1, (uint8_t *)message7, sizeof(message7), 10);
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10909CEC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LEDError_GPIO_Port, LEDError_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LEDError_Pin */
  GPIO_InitStruct.Pin = LEDError_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LEDError_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PushButton_Pin */
  GPIO_InitStruct.Pin = PushButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PushButton_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
	HAL_GPIO_WritePin(LEDError_GPIO_Port, LEDError_Pin, GPIO_PIN_RESET);
	__BKPT();

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
