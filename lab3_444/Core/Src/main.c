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
DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac_ch1;

DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel2;
DMA_HandleTypeDef hdma_dfsdm1_flt0;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
////part 3
//#define AUDIO_REC	12000
//int32_t RecBuff[AUDIO_REC];
//uint32_t PlayBuff[AUDIO_REC];

//part 4
#define AUDIO_REC	9000
int32_t RecBuff[AUDIO_REC];
uint32_t PlayBuff[AUDIO_REC];
//
uint8_t pressed = 0;
uint8_t DmaRecHalfBuffCplt = 0;
uint8_t DmaRecBuffCplt = 0;

uint32_t allnotes[256];



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_DAC1_Init(void);
static void MX_DFSDM1_Init(void);
/* USER CODE BEGIN PFP */
////part 1 pushbutton
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	if(GPIO_Pin == PushButton_Pin){
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//	}
//
//}
////part 1 DAC
//float pi=3.14;
//uint32_t sine[30];
//int counter = 0;
//float sinval =0;
//void get_sine(){
//	for(int i =0; i<30; i++){
//		sine[i] = (1 + arm_sin_f32(2*pi*i/30))*2048;
//	}
//}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
//	if(htim == &htim2){
//		if(counter == 30){
//			counter = 0;
//		}
//		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sine[counter]);
//		counter++;
//	}
//}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

  uint32_t C6[42];
  uint32_t E6[34];
  uint32_t G6[28];
  uint32_t A6[25];
  uint32_t B6[22];
  uint32_t B5[45];
  //uint16_t *notes[] = {C6, E6, G6, A6, B6, B5};
  // Initialize each note array
  //sample n = 44.1k/1046.5 = 42
 void get_C6(){
  for(int i =0; i<42; i++){
  	C6[i] = 0.33*i*98;
  	}
  }
 void get_E6(){
  for(int i = 0; i < 34; i++){
  	E6[i] = 0.33*i*120;
  	}
 }
 void get_G6(){
  for(int i = 0; i < 28; i++){
  	G6[i] = 0.33*i*146;
  	}
 }
void get_A6(){
	for(int i = 0; i < 25; i++){
  	A6[i] = 0.33*i*164;
  	}
}

void get_B6(){
  for(int i = 0; i < 22; i++){
  	B6[i] = 0.33*186*i;
  	}
}
void get_B5(){
  for(int i = 0; i < 45; i++){
  	B5[i] = 0.33*i*91;
  	}
}


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
  MX_TIM2_Init();
  MX_DAC1_Init();
  MX_DFSDM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  get_B5();
  get_C6();
  get_E6();
  get_G6();
  get_A6();
  get_B6();
  //part 4

  //C6 1046.5
  //E6 1318.5 Hz
  //sample n = 44.1k/1318.5 = 34
  //G6 1568.0 Hz
  //sample n = 44.1k/1568 = 28

  //A6 1760.0 Hz
  // sample n = 44.1k/1760 = 25


  //B6 1975.53 Hz
  //sample n = 44.1k/1975.5 = 22

  //B5 987.78 Hz

  ////E6 1318.5 Hz
  ////sample n = 44.1k/1318.5 = 34
  //uint8_t E6[34];


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DFSDM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DFSDM1_Init(void)
{

  /* USER CODE BEGIN DFSDM1_Init 0 */

  /* USER CODE END DFSDM1_Init 0 */

  /* USER CODE BEGIN DFSDM1_Init 1 */

  /* USER CODE END DFSDM1_Init 1 */
  hdfsdm1_filter0.Instance = DFSDM1_Filter0;
  hdfsdm1_filter0.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter0.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter0.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter0.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC3_ORDER;
  hdfsdm1_filter0.Init.FilterParam.Oversampling = 70;
  hdfsdm1_filter0.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter0) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel2.Instance = DFSDM1_Channel2;
  hdfsdm1_channel2.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel2.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel2.Init.OutputClock.Divider = 33;
  hdfsdm1_channel2.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel2.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel2.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel2.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel2.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel2.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel2.Init.Awd.Oversampling = 1;
  hdfsdm1_channel2.Init.Offset = 0;
  hdfsdm1_channel2.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter0, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DFSDM1_Init 2 */

  /* USER CODE END DFSDM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1860;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PushButton_Pin */
  GPIO_InitStruct.Pin = PushButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PushButton_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//part 3

//pressed-->start recording (LED Blink) --> recording over, start playing LED stay on
//Pressed again, start recording

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
//	if(GPIO_PIN == GPIO_PIN_13){
//		if(pressed == 0){
//			//(filter, data, length)
//			HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, RecBuff, AUDIO_REC);

//			pressed = 1;
//		}else{
//			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
//			for(uint16_t i = 0; i < AUDIO_REC; i++){
//				PlayBuff[i] = 0;
//				RecBuff[i] = 0;
//			}
//			pressed = 0;
//		}
//	}
//}
//
//void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter){
//	DmaRecHalfBuffCplt = 1;
//	for(int i = 0; i < AUDIO_REC / 2; i++){
//		PlayBuff[i] = (RecBuff[i] >> 8);
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//
//	}
//}
//
//void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter){
//	DmaRecBuffCplt = 1;
//	for(int i = AUDIO_REC / 2; i < AUDIO_REC; i++){
//		PlayBuff[i] = (RecBuff[i] >> 8);//shift 8
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//
//	}
//		float max = -838809;
//		float min = 838808;
//		for(int i = 0; i < AUDIO_REC; i++){
//		if(PlayBuff[i] > max){
//			max = PlayBuff[i];
//		}
//		if(PlayBuff[i] < min){
//			min = PlayBuff[i];
//			}
//		}
//		for(int i =0; i< AUDIO_REC; i++){
//			PlayBuff[i] = ((PlayBuff[i] - min)/(max-min))*4000;

//		}
//	HAL_DFSDM_FilterRegularStop_DMA(&hdfsdm1_filter0);
//	//recording over. playing start
//
//	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//
//	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, PlayBuff, AUDIO_REC, DAC_ALIGN_12B_R);
//}

////part 3 end
//part 4 start

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
	if(GPIO_PIN == GPIO_PIN_13){
		if(pressed == 0){
			//(filter, data, length)
			//start recording a fixed length
			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
			//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, RecBuff, AUDIO_REC);
			pressed = 1;
		}else if (pressed == 1){
			// Stop any ongoing playback
			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
			// LED to indicate that playback is starting
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, PlayBuff, AUDIO_REC, DAC_ALIGN_12B_R);
			pressed = 2;
		}else if (pressed == 2){
			//C6
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, C6, 42, DAC_ALIGN_12B_R);
			pressed = 3;
		}else if (pressed == 3){
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, E6, 34, DAC_ALIGN_12B_R);
			pressed = 4;
		}else if (pressed == 4){
		   HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
 		   HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		   HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, G6, 28, DAC_ALIGN_12B_R);
		   pressed = 5;
		}else if (pressed == 5){
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, A6, 25, DAC_ALIGN_12B_R);
			pressed = 6;
		}else if (pressed == 6){
		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		  HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, B6, 22, DAC_ALIGN_12B_R);
		  pressed = 7;
		}else{
		 HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		 HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		 HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, B5, 45, DAC_ALIGN_12B_R);
		 pressed =0;
		}
	}
}

//void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter){
//	DmaRecHalfBuffCplt = 1;
//	for(int i = 0; i < AUDIO_REC / 2; i++){
//		PlayBuff[i] = RecBuff[i]>>8;
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//	}
//}

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter){
	DmaRecBuffCplt = 1;
	for(int i = AUDIO_REC; i < AUDIO_REC; i++){
		PlayBuff[i] = RecBuff[i]>>8;
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
	float max = -838809;
				float min = 838808;
				for(int i = 0; i < AUDIO_REC; i++){
					if(PlayBuff[i] > max){
						max = PlayBuff[i];
					}
					if(PlayBuff[i] < min){
						min = PlayBuff[i];
					}
				}
				for(int i =0; i< AUDIO_REC; i++){
					PlayBuff[i] = ((PlayBuff[i] - min)/(max-min))*4000;
				}
}
//part 4 end


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
