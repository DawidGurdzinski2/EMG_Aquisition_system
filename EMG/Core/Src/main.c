/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "DEV_Config.h"
#include "LCD.h"
#include "menu.h"
#include "common.h"
#include "SD_card.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DMABUFSIZE 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t timeSD1,timeSD2;
uint32_t uartTIME1,uartTIME2;

uint8_t BLstatus=0;
uint8_t SDstatus=0;
uint8_t RSTstatus=0;
uint8_t RST=0;
//SC CARD
uint8_t clearSDflag;
extern DMA_HandleTypeDef hdma_adc1;
extern struct SD_Iterface SD;
volatile uint8_t firstconversion=0;
volatile uint8_t secondconversion = 0;
uint8_t	datatransmision =1;
uint16_t dma_buf[DMABUFSIZE];
char tempconv[4];

uint32_t t1,t2;
uint8_t check=0;
volatile uint8_t Bluetooth[125];

volatile uint32_t enc_time=0;
volatile int16_t enc_rot=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void farray(DMA_HandleTypeDef *hdma);
void sarray(DMA_HandleTypeDef *hdma);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void conversion(uint16_t range){
	for(int16_t i=range;i<range+500;i++){
		sprintf(tempconv,"%d",dma_buf[i]);
		if(dma_buf[i]>999){
			SD.buffer[4*i]=tempconv[0];
			SD.buffer[4*i+1]=tempconv[1];
			SD.buffer[4*i+2]=tempconv[2];
			SD.buffer[4*i+3]=tempconv[3];
		}
		else if(dma_buf[i]>99){
			SD.buffer[4*i]='0';
			SD.buffer[4*i+1]=tempconv[0];
			SD.buffer[4*i+2]=tempconv[1];
			SD.buffer[4*i+3]=tempconv[2];
		}
		else if(dma_buf[i]>99){
			SD.buffer[4*i]='0';
			SD.buffer[4*i+1]='0';
			SD.buffer[4*i+2]=tempconv[0];
			SD.buffer[4*i+3]=tempconv[1];
		}
		else{
			SD.buffer[4*i]='0';
			SD.buffer[4*i+1]='0';
			SD.buffer[4*i+2]='0';
			SD.buffer[4*i+3]=tempconv[0];
		}
	}
}
void save_data_to_SD(){
	if(datatransmision){
		SD.buffer[4000]='\n';
		RSTstatus=writeDataPacked("emg.txt", FA_OPEN_APPEND | FA_WRITE | FA_READ);
	}
}
void adcbufToBlbuf(){
	for(uint8_t i=0;i<25;i++){
		Bluetooth[5*i]='s';
		Bluetooth[5*i+1]=SD.buffer[160*i];
		Bluetooth[5*i+2]=SD.buffer[160*i+1];
		Bluetooth[5*i+3]=SD.buffer[160*i+2];
		Bluetooth[5*i+4]=SD.buffer[160*i+3];
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
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  LCD_SCAN_DIR Lcd_ScanDir = D2U_L2R;//SCAN_DIR_DFT = D2U_L2R
  LCD_Init( Lcd_ScanDir );
  Driver_Delay_ms(1000);
  LCD_Clear(GRAY);
  create_structarray();
  menu_refresh();
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start(&htim5);
  HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)dma_buf, DMABUFSIZE);
  hdma_adc1.XferHalfCpltCallback = farray;
  hdma_adc1.XferCpltCallback = sarray;
  mountSDcard();
  char pData[]="AT";
  HAL_UART_Transmit(&huart2,(uint8_t*) pData, sizeof(pData)-1, 100);
char pDATAr[10];
HAL_UART_Receive(&huart2, (uint8_t*) pDATAr, 10, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(clearSDflag){
		  deleteFile("emg.txt");
		  clearSDflag=0;
	  }
	  if(RST){
		  NVIC_SystemReset();
	  }
	  if(firstconversion){
		conversion(0);
		firstconversion=0;

	  }
	  if(secondconversion){
		  t1=TIM2->CNT;
		  conversion(500);
		  if(BLstatus){
		  		  adcbufToBlbuf();
		  		  	HAL_UART_Transmit(&huart2,(uint8_t *)Bluetooth , 125, 100);
		  		  }
		  if(SDstatus){save_data_to_SD();}

		  t2=TIM2->CNT-t1;
		  secondconversion=0;

	  }

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void farray(DMA_HandleTypeDef *hdma) {
	firstconversion=1;
}

void sarray(DMA_HandleTypeDef *hdma) {
	secondconversion = 1;
}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM3){

		if((TIM5->CNT-enc_time)>20){
			enc_time=TIM5->CNT;
			if(enc_rot<(int16_t)__HAL_TIM_GET_COUNTER(htim)){
				key_prev_press();
			}
			else if(enc_rot>(int16_t)__HAL_TIM_GET_COUNTER(htim)){
				key_next_press();
			}
			enc_rot=(int16_t)__HAL_TIM_GET_COUNTER(&htim3);
		}
		if(enc_time>HAL_MAX_DELAY){
			enc_time=0;
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==SW_Pin){
		static uint32_t prev_time=0;
		if(!HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin)&& (HAL_GetTick()-prev_time)>20){
			key_enter_press();
			prev_time=HAL_GetTick();
		}
		if(prev_time>HAL_GetTick()){
			prev_time=0;
		}
	}
}


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
