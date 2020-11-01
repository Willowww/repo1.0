/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stdio.h"
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


uint8_t aRxBuffer[10];
int uart1_rx_cnt=0;
int UART1_Rx_flg=0;
uint8_t UART1_Rx_Buf[256];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   if(GPIO_Pin==GPIO_PIN_0){
		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

uint16_t pwmVal=0;   //PWM占空比
uint8_t dir=1;

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
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_TIM14_Init(); 
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
  /* USER CODE BEGIN 2 */
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


   /*串口数字回文用
	 HAL_UART_Receive_IT(&huart1,(uint8_t*)&aRxBuffer,1);
		while(1)
		{
			if(UART1_Rx_flg==1)
    {
      HAL_UART_Transmit(&huart1,UART1_Rx_Buf,uart1_rx_cnt,0xffff);    //发送接收到的数据
      for(int i = 0;i<uart1_rx_cnt;i++)
        UART1_Rx_Buf[i] = 0;
      uart1_rx_cnt = 0;
      UART1_Rx_flg = 0;
    } 
	}		*/
	
	
	while (1)
{

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
while (pwmVal< 500){
	pwmVal++;
	__HAL_TIM_SetCompare(&htim14, TIM_CHANNEL_1, pwmVal);    //修改比较值，修改占空比
	//   TIM3->CCR1 = pwmVal;    与上方相同
	HAL_Delay(1);}
while (pwmVal)
{
	pwmVal--;
	__HAL_TIM_SetCompare(&htim14, TIM_CHANNEL_1, pwmVal);    //修改比较值，修改占空比
	//   TIM3->CCR1 = pwmVal;     与上方相同
	HAL_Delay(1);}
HAL_Delay(200);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
	if(huart->Instance==USART1)
	{
		
		if(uart1_rx_cnt==0)
			HAL_TIM_Base_Start_IT(&htim7);
	
    UART1_Rx_Buf[uart1_rx_cnt] = aRxBuffer[0];
    uart1_rx_cnt++;
		memset(&aRxBuffer,0x00,sizeof(aRxBuffer));//清空
    HAL_UART_Receive_IT(&huart1,(uint8_t *)&aRxBuffer,1);
//		HAL_UART_Transmit(&huart1,(uint8_t *)&aRxBuffer,10,0xFFFF);
	//  memset(&aRxBuffer,0x00,sizeof(aRxBuffer));//清空
	  //HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);}
 }
}

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance==USART1)
  {
    __HAL_TIM_SET_COUNTER(&htim7,0);
    if(uart1_rx_cnt==0)//如果是第一个字符，则开启定时器
    {
    __HAL_TIM_CLEAR_FLAG(&htim7,TIM_FLAG_UPDATE);
      HAL_TIM_Base_Start_IT(&htim7);
    }
    UART1_Rx_Buf[uart1_rx_cnt] = aRxBuffer[0];
    uart1_rx_cnt++;
    HAL_UART_Receive_IT(&huart1,(uint8_t *)&aRxBuffer,1);
  }
}*/
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
