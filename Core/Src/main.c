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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "math.h"
#include "printf.h"
#include "duoji.h"
#include "count_theta.h"
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

uint16_t quare[19]= {150,157,150,150,155,143,128,
                         157,150,150,155,143,128,
                         157,150,150,155,143,128};
//quare1-6������Ҫת����λ��
//quare7-12��ʾ�ϴ����ڵ�λ��
//quare13-18��ʾ���˲ʱ����λ��
uint16_t jiaodu = 0;

uint8_t RxFlag=0;
uint8_t RxBuffer[20];

#define LENGTH 100
uint8_t USART1_RxBuffer[LENGTH];
uint8_t RecCount = 0;
uint8_t Usart_RxFlag = 0;
uint8_t delay_time = 0;

double x0 = 0;
double y0 = 0;
double m = 0;
#define PI 3.14159265
double a1,a2,a3,angle1,angle2,angle3;
												 
double theta1,theta2,theta3,theta4;
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
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Init(&htim3);
    __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(&htim3);


//    HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer, 5 );

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA (&huart1, (uint8_t*)USART1_RxBuffer, LENGTH);



    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_9);
        HAL_Delay(100);
        /*����4*/
        if(Usart_RxFlag == 1 )  //���ݽ������
        {
            if (USART1_RxBuffer[0] == '7')//7�Ǳ�ʶ����˵�������ȡ����
            {
                //�����theta
							  thetacount();
							
                quare[1]=162+theta4*50/45;
                quare[2]=150+theta1*50/45;
                quare[3]=150+theta2*50/45;
                quare[4]=155+theta3*50/45;
                quare[6]=150+34*50/45;
							
								control_duoji(4);
                control_duoji(3);
                control_duoji(1);
                control_duoji(2);
                control_duoji(6);
                HAL_Delay(1000);
                //�������
                quare[2]=150+(theta1+45)*50/45;
                control_duoji(2);
                Usart_RxFlag = 0;//���ô��ڽ��ձ�־
            }

            if (USART1_RxBuffer[0] == '8')//8�Ǳ�ʶ����˵��������ÿ���
            {
                thetacount();
							
                quare[1]=162+theta4*50/45;
                quare[2]=150+theta1*50/45;
                quare[3]=150+theta2*50/45;
                quare[4]=155+theta3*50/45;
								
                control_duoji(4);
                control_duoji(3);
                control_duoji(1);
                control_duoji(2);
								HAL_Delay(1000);
								//���·���
								quare[6]=150-20*50/45;
                control_duoji(6);
								//�����е��
                quare[2]=150+(theta1+45)*50/45;
								control_duoji(2);
                Usart_RxFlag = 0;//���ô��ڽ��ձ�־
            }

        }
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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
//           HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);  //����PB8�����������
//           HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);  //˳��PB5
//           HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);//����PD2
//           HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);//�︺PC12
//           HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);//����PC11
//           HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);//˳��PC10����һ�����
int count=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//��ʱ���жϻص�����
{
    if(htim->Instance == TIM3)
    {
        count++;
        //��һ�����
        if (count<quare[13])//������ʱ����ȡֵ��Χ0.5ms~2.5ms����ʱ��0.01ms���жϣ���ȡֵ50~250
        {   //50->-90��,100->-45��,150->0,200->45��,250->90��
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);//˳��PC10
        }
        else if(count >= quare[13] && count <= 2000)
        {
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
        }
        //�ڶ������
        else if(count > 2000 && count < quare[14]+2000)
        {
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);//����PC11
        }
        else if(count >= quare[14]+2000 && count <= 4000)
        {
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
        }
        //���������
        else if(count > 4000 && count < quare[15]+4000)
        {
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);//�︺PC12
        }
        else if(count >= quare[15]+4000 && count <= 6000)
        {
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
        }
        //���ĸ����
        else if(count > 6000 && count < quare[16]+6000)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);//����PD2
        }
        else if(count >= quare[16]+6000 && count <= 8000)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
        }
        //��������
        else if(count > 8000 && count < quare[17]+8000)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);  //˳��PB5
        }
        else if(count >= quare[17]+8000 && count <= 10000)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
        }
        //���������
        else if(count > 10000 && count < quare[18]+10000)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);  //����PB8
        }
        else if(count >= quare[18]+10000 && count <= 12000)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
        }
        else if(count > 12000)
        {
            count = 0;
        }
        __HAL_TIM_CLEAR_IT(&htim3,TIM_IT_UPDATE);
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
