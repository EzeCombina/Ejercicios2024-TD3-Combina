/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "spi.h"
#include "tim.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define HALFBUFFERSIZE		50 						// Mitad del tamaño del buffer para DMA
#define FULLBUFFERSIZE  	HALFBUFFERSIZE*2 		// Tamaño del buffer para DMA
#define MUESTRASMAX 		10 						// Cantidad máxima de muestras
#define MULTIPLICADOR		1.0/(float)MUESTRASMAX	// Multiplicador

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Buffers para ADC y DAC a través de DMA
uint16_t adc_val [FULLBUFFERSIZE];
uint16_t dac_val [FULLBUFFERSIZE] = {0};

static volatile uint16_t* adcBufPtr; 				// Puntero a buffer ADC
static volatile uint16_t* dacBufPtr; 				// Puntero a buffer DAC
static volatile uint16_t muestrasPtr[MUESTRASMAX];	// Puntero a buffer Muestras
static volatile uint16_t Valor;						// Valor obtenido luego de aplicar el Filtro

//bandera de fin de conversión, 1 buffer listo para ser procesado
volatile uint8_t flagProcess = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void ProcessDSP();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	MX_TIM8_Init();

	HAL_TIM_Base_Start(&htim8);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_val, FULLBUFFERSIZE);

	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,(uint32_t*) dac_val, FULLBUFFERSIZE, DAC_ALIGN_12B_R);

	/* Infinite loop */

	for(int j = 0; j < (MUESTRASMAX - 1); j++)
	{
		muestrasPtr[j] = 0;
	}

	while (1)
	{
		if (flagProcess == 1) //Si hay datos para procesar, ingresa
		{
			HAL_GPIO_WritePin(GPIOD, LD4_Pin,1); //PD12, para medir tiempo de ejecución
			ProcessDSP();
			HAL_GPIO_WritePin(GPIOD, LD4_Pin,0);
			flagProcess = 0; // Limpiamos bandera
		}
	}
}

/**
 * Función para el procesamiento DSP. La duración de esta función debe ser menor
 * al tiempo que demora completarse mitad del buffer DMA
 */
void ProcessDSP()
{
	//Copiamos los datos del ADC al DAC sin procesar
	//(la mitad del buffer ya convertido)
	//En este punto se puede aplicar las operaciones o filtros deseados

	// FILTRO DE 10 MUESTRAS
	for(int n = 0; n < HALFBUFFERSIZE; n++)
	{
		Valor = MULTIPLICADOR*adcBufPtr[n];
		for(int h = 0; h < (MUESTRASMAX -1); h++)
		{
			Valor += MULTIPLICADOR*muestrasPtr[h];
		}

		for(int i = 8; i >= MUESTRASMAX - (MUESTRASMAX - 1); i--)
		{
			muestrasPtr[i] = muestrasPtr[i-1];
		}
		muestrasPtr[0] = adcBufPtr[n];

		dacBufPtr[n] = Valor;
	}

	/*for(int n = 0; n < HALFBUFFERSIZE; n++)
		{
			dacBufPtr[n] = adcBufPtr[n];
		}*/
}

/**
 * Cuando se llena mitad del buffer de DMA, actualizamos la dirección
 * de los punteros al inicio de dicho buffer, de tal forma de
 * procesar los datos al mismo tiempo que se carga la segunda
 * mitad del buffer DMA
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	HAL_GPIO_TogglePin(GPIOD, LD5_Pin);
	adcBufPtr =  &adc_val[0];
	dacBufPtr = &dac_val[0];
	flagProcess = 1; //Esta bandera indica que hay datos para procesar
}

/**
 * Cuando se llena completo el buffer de DMA, actualizamos la dirección
 * de los punteros a la mitad de dicho buffer, de tal forma de
 * procesar los datos al mismo tiempo que se carga la primera
 * mitad del buffer DMA
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adcBufPtr =  &adc_val[HALFBUFFERSIZE];
	dacBufPtr = &dac_val[HALFBUFFERSIZE];
	flagProcess = 1;  //Esta bandera indica que hay datos para procesar
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
