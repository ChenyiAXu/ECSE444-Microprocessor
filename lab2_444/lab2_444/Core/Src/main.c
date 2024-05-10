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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

DAC_HandleTypeDef hdac1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC1_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC3_Init(void);
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

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC1_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t j =0;

  uint32_t saw[16];
  uint32_t sawG;

  uint32_t triangle[16];
  uint32_t triangleG;

  uint32_t sine;
  float pi=3.14;
  float tempCelsius;
  uint32_t sineWave[16];

  //part 4
  uint32_t a =0;
  //uint32_t waveformValue;
  uint32_t z = 0;
  GPIO_PinState lastButtonState = GPIO_PIN_SET; // Initialize to released state
 uint32_t button_counter =0;
 uint32_t iteration;


  for(uint32_t i =0; i<16; i++){
	  saw[i]= i*256; //4095/16
	  if (i < 8)
	  		  triangle[i] = i * 512; //4095/(16/2)
	  	  else if (i == 8)
	  		  // normally would be 256, but want it to be 8 bit
	  		  triangle[i] = 4095; //peak
	  	  else
	  		  triangle[i] = 4096 - ((i % 8) * 512);
	  sineWave[i] = (uint32_t) roundf(2047.0 * (1.0 + arm_sin_f32(pi * i / 8)));
  }


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //part 1 start
	  	  //active low configuration-->button press connect to ground, logic zero
	  	  //when the button is not pressed --> pulled high through resistor, logic 1
	  	  //read state of pushbutton

	  	  GPIO_PinState button = HAL_GPIO_ReadPin(PushButton_GPIO_Port,PushButton_Pin);
	  	  //if button is pressed (set the port pins)
	  	  if(button == GPIO_PIN_RESET)
	  		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	  	 //if not pressed (reset clear the port pins)
	  	  else
	  		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	  	  HAL_Delay(5);
	  	 //part 1 end

	  	  part 2 start
	  	  sawG = saw[j];
	  	  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sawG);

	  	  triangleG = triangle[j];
	      HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, triangleG);

	      sine = sineWave[j];
	      HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sine);
	  	  j = (j+1)%16;

	  	for (volatile uint32_t k = 0; k < 400; k++)
	  	    {
	  	        // This loop consumes CPU cycles and creates a delay

	  	    }
	  	part 2 end
	  	//part 3 start
	  	//polling stp: start-->polling-->getvalue
	  	float VREF;
	  	uint16_t V_Measured;
	  	uint16_t tempVoltage;

	  	//HAL_ADC_PollForConversion:ensure ADC conversion is completed, 10000S TIME OUT
	  	//HAL_ADC_GetValue: get the vale at hadc 1
	  	HAL_ADC_Start(&hadc1);

	  	if(HAL_ADC_PollForConversion(&hadc1, 10000) == HAL_OK) {
	  			V_Measured = HAL_ADC_GetValue(&hadc1);
	  			HAL_ADC_Stop(&hadc1);
	  			uint16_t VREFINT_CAL =  *((uint16_t*) (0x1FFF75AA));
	  			VREF = 3000 * (float) VREFINT_CAL / ((float) V_Measured);
	  	}

	  	// get tempVoltage
	  	// calculate temp in Celsious based on tempVoltage and VREF and calibrated values
	  	HAL_ADC_Start(&hadc3);
	  	if(HAL_ADC_PollForConversion(&hadc3, 10000) == HAL_OK) {
	  			tempVoltage = HAL_ADC_GetValue(&hadc3);
	  			HAL_ADC_Stop(&hadc3);
	  			uint16_t castedTempVoltage = (uint16_t) tempVoltage;

	  			uint16_t TS_CAL2 = *((uint16_t*) (0x1FFF75CA));
	  			uint16_t TS_CAL1 = *((uint16_t*) (0x1FFF75A8));

	  			int16_t TS_DATA= castedTempVoltage*VREF/3000;
	  			int16_t diff = TS_DATA - TS_CAL1;

	  			uint16_t cal_temp_difference = 110 - 30;
	  			uint16_t cal_difference = TS_CAL2 - TS_CAL1;

	  			// in here, we are also scaling the TS_CAL2/1 according to VREF
	  		    tempCelsius = ((((float) cal_temp_difference) / ((float) cal_difference))*diff)+30;

	  			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)tempCelsius);
	  			HAL_Delay(1);


	  				}
	  	//part 3 end

	  	//part 4 start

	  GPIO_PinState button = HAL_GPIO_ReadPin(PushButton_GPIO_Port, PushButton_Pin);

	      if (button == GPIO_PIN_RESET && lastButtonState == GPIO_PIN_SET) {
	          // Button was pressed
	          button_counter++;
	          if (button_counter > 6) {
	              button_counter = 1;
	          }
	      }

	      lastButtonState = button;

	      if (button_counter == 0) {
	          // Default saw waveform
	          sawG = saw[a];
	          HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sawG);
	          a = (a + 1) % 16;
	          for (volatile uint32_t k = 0; k < 400; k++) {
	          	    	          // This loop consumes CPU cycles and creates a delay
	          	  }
	      } else {

	          if (button_counter == 1 || button_counter == 3 || button_counter == 5) {
	              // Temperature-related operations...
	        	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//	        	  while (button == GPIO_PIN_SET) {
//	        	   // Wait for the button to be released
//	        		  button = HAL_GPIO_ReadPin(PushButton_GPIO_Port, PushButton_Pin);
//
//	        	  }
	        	  sawG = saw[z];
	        	  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sawG);
	        	  z = (z + 1) % 16;
	        	  	  	//polling stp: start-->polling-->getvalue
	        	  	  	float VREF;
	        	  	  	uint16_t V_Measured;
	        	  	  	uint16_t tempVoltage;

	        	  	  	//HAL_ADC_PollForConversion:ensure ADC conversion is completed, 10000S TIME OUT
	        	  	  	//HAL_ADC_GetValue: get the vale at hadc 1
	        	  	  	HAL_ADC_Start(&hadc1);

	        	  	  	if(HAL_ADC_PollForConversion(&hadc1, 10000) == HAL_OK) {
	        	  	  			V_Measured = HAL_ADC_GetValue(&hadc1);
	        	  	  			HAL_ADC_Stop(&hadc1);
	        	  	  			uint16_t VREFINT_CAL =  *((uint16_t*) (0x1FFF75AA));
	        	  	  			VREF = 3000 * (float) VREFINT_CAL / ((float) V_Measured);
	        	  	  	}

	        	  	  	// get tempVoltage
	        	  	  	// calculate temp in Celsious based on tempVoltage and VREF and calibrated values
	        	  	  	HAL_ADC_Start(&hadc3);
	        	  	  	if(HAL_ADC_PollForConversion(&hadc3, 10000) == HAL_OK) {
	        	  	  			tempVoltage = HAL_ADC_GetValue(&hadc3);
	        	  	  			HAL_ADC_Stop(&hadc3);
	        	  	  			uint16_t castedTempVoltage = (uint16_t) tempVoltage;

	        	  	  			uint16_t TS_CAL2 = *((uint16_t*) (0x1FFF75CA));
	        	  	  			uint16_t TS_CAL1 = *((uint16_t*) (0x1FFF75A8));

	        	  	  			int16_t TS_DATA= castedTempVoltage*VREF/3000;
	        	  	  			int16_t diff = TS_DATA - TS_CAL1;

	        	  	  			uint16_t cal_temp_difference = 110 - 30;
	        	  	  			uint16_t cal_difference = TS_CAL2 - TS_CAL1;

	        	  	  			// in here, we are also scaling the TS_CAL2/1 according to VREF
	        	  	  		    tempCelsius = ((((float) cal_temp_difference) / ((float) cal_difference))*diff)+30;


	        	  	  		    iteration = roundf(tempCelsius*(100)-1700);
	        	  	  			HAL_ADC_Stop(&hadc3);
	        	  	  			HAL_ADC_Stop(&hadc1);

	        	  	  				}
	        	  for (volatile uint32_t k = 0; k < iteration; k++) {
	        	  	    	          // This loop consumes CPU cycles and creates a delay
	        	  }

	          } else if (button_counter == 2) {
	        	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	              // Triangle waveform
	              triangleG = triangle[z];
	              HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, triangleG);
	              z = (z + 1) % 16;
	    	      // Delay for a short period
	    	      for (volatile uint32_t k = 0; k < 400; k++) {
	    	          // This loop consumes CPU cycles and creates a delay
	    	      }
	          } else if (button_counter == 4) {
	        	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	              // Saw waveform
	              sawG = saw[z];
	              HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sawG);
	              z = (z + 1) % 16;
	    	      // Delay for a short period
	    	      for (volatile uint32_t k = 0; k < 400; k++) {
	    	          // This loop consumes CPU cycles and creates a delay
	    	      }
	          } else if (button_counter == 6) {
	        	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	              // Sine waveform
	              sine = sineWave[z];
	              HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sine);
	              z = (z + 1) % 16;
	    	      // Delay for a short period
	    	      for (volatile uint32_t k = 0; k < 400; k++) {
	    	          // This loop consumes CPU cycles and creates a delay
	    	      }
	          }
	      }



	  	// part 4 end

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
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

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
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

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
