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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "oled.h"
#include "OLED_Data.h"
#include "stdio.h"
#include "Movement.h"
#include "Servo.h"
#include "PWM_HAL.h"
#include "Serial_HAL.h"
#include "LED.h"
#include "OLED_Data.h"
#include "mode.h"
#include "Timer_HAL.h"
#include "Hongwai.h"
#include "usart2_hal.h"
#include "usart_hal.h"
#include "UltrasonicWave.h"
#include "syn6288.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

	  uint8_t RxData;			//串口接收数据临时存储变量
	  uint8_t move_mode1 = '0';//遥控器串口接收的模式变量
	  uint8_t move_mode3 = '0';//语音识别串口接收的模式变量
	  uint8_t move_mode = '0';//当前状态模式变量
	  uint8_t previous_mode = '0';//当前模式的上一个模式变量
	   uint16_t T=100; // 超声波距离 距离值对应10cm(单位厘米)
	  uint16_t bz_flag;//避障标志位 ON/OFF 默认为off
	  uint16_t hw_flag;//红外检测标志位 ON/OFF 默认为off
	  int happiness; //快乐指数
	  int stamina;  //体力值
	  uint8_t t1=0 ; //存储红外检测结果 前
	  uint8_t t2=0 ; //存储红外检测结果 后
	  uint8_t t3=0 ; //存储红外检测结果 左
	  uint8_t t4=0 ; //存储红外检测结果 右
	  uint16_t ff=0; //前进前腿摆动角度
	  uint16_t bb=0; //前进后腿摆动角度
	  uint16_t ll=0; //左转摆动角度
	  uint16_t rr=0; //右转摆动角度
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 不需要在这里为每个图像定义结构体了，已经在OLED_Data.h中声明好了
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
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
  HAL_Delay(200); // 单片机启动比OLED上电快,需要延迟等待一下*/
  OLED_Init();
  PWM_Init();
  Serial_Init();
  LED_Init();
  hongwai_init();
  Servo_Init();
  USART2_Init();

	// 机器人初始化为站立姿态
	move_stand();

	happiness=200;//初始快乐指数
	stamina=500;//初始体力值
	bz_flag=0;//避障标志位 ON/OFF 默认为off
	hw_flag=0;//红外检测标志位 ON/OFF 默认为off
	/*开机默认状态*/
	  OLED_NewFrame();

	  // 1. 使用新添加的OLED_DrawImageInverted函数显示BMP1图像（上下倒置）
	  // 该函数用于解决OLED屏幕倒置的问题
	  OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL);

	  // 2. BMP1_Image结构体已经在OLED_Data.h中声明，在OLED_Data.c中定义
	  //    无需在main.c中重复定义

	  /*OLED_PrintString(0, 22, "Servo Test", &font16x16, OLED_COLOR_NORMAL);*/
	 OLED_ShowFrame();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  // 示例：若实际使用的是无符号字符型
	  if(stamina>0)//体力值大于0时正常执行动作
	  	{
	  		move_mode1 = USART1_GetRxData();		//获取遥控器串口接收到的模式
	  		move_mode3 = USART3_GetRxData();		//获取语音识别串口接收到的模式
	  		if (USART1_GetRxFlag()) {//当遥控器串口接收到数据时，使用遥控器模式
	  			move_mode = move_mode1;
	  		}
	  		else if (USART3_GetRxFlag()){
	  			move_mode = move_mode3;
	  		}
	  	}
	  		if(stamina<0)//体力值小于0时执行难受状态
	  	{
	  		mode_nanshou();
	  		stamina=0;
	  	}
	  		if(stamina==0)//体力值为0时只能执行睡觉指令
	  	{
	  		if(USART1_GetRxData()=='2'|USART3_GetRxData()=='2')
	  		{mode_sleepwo();}
	  		else if(USART1_GetRxData()=='p'|USART3_GetRxData()=='p')
	  		{mode_sleeppa();}
	  	}
	  		if(stamina>500)//体力值超过最大值500
	  		{stamina=500;}

	  		if(happiness<0){//快乐值小于0时执行难受状态，当快乐值或体力值为0时执行难受状态
	  		happiness=0;
	  		mode_nanshou();
	  		}
	  		if(happiness>200)//快乐值超过最大值200
	  		{happiness=200;}

	  		//***红外检测功能实现模块***//
	  		if(move_mode == 'H') //红外开
	  		{hw_flag=1;}
	  		if(move_mode == 'h') //红外关
	  		{hw_flag=0;
	  			//关闭红外检测时清除检测结果
	  			t1=0;t2=0;t3=0;t4=0;
	  		}
	  		if(hw_flag){
	  			Edge_detect();//开启红外检测功能
	  		}

	  	   //***避障模式功能模块***//
	  		if(move_mode == 'x') //避障开
	  		{
	  			bz_flag=1;
	  		}
	  		if(move_mode == 'c') //避障关
	  		{
	  			bz_flag=0;
	  			T=100;
	  		}
	  		if(bz_flag){
	  		Bizhang();//开启避障模式
	  		}


	  		//***运动模式功能模块***//
	  		if (move_mode == 'f') { //前进
	  			mode_forward();
	  		} else if (move_mode == 'b') { //后退
	  			mode_behind();
	  		} else if (move_mode == 'l') { //左转
	  			mode_left();
	  		} else if (move_mode == 'r') { //右转
	  			mode_right();
	  		} else if (move_mode == 'w') { //前后摇摆
	  			mode_swing_qianhou();
	  		} else if (move_mode == 'z') { //左右摇摆
	  			mode_swing_zuoyou();
	  		} else if (move_mode == 'd') { //跳舞
	  			mode_dance();
	  		} else if (move_mode == '5') { //站立
	  			mode_stand();
	  		} else if (move_mode == 'q' && previous_mode != '0') { //慢站立
	  			mode_slowstand();
	  		} else if (move_mode == 's' && previous_mode != 's') { //伸展
	  			mode_strech();
	  		} else if (move_mode == 'j') { //双手投降
	  			mode_twohands();
	  		} else if (move_mode == 'y') { //懒腰
	  			mode_lanyao();
	  		} else if (move_mode == '1') { //抬头
	  			mode_headup();
	  		} else if (move_mode == 'p' && previous_mode != 'p') { //趴着睡觉
	  			mode_sleeppa();
	  		} else if (move_mode == '2' && previous_mode != '2') { //躺着睡觉
	  			mode_sleepwo();
	  		} else if (move_mode == 'n') { //难受
	  			mode_nanshou();
	  		} else if (move_mode == 'B') { //表白
	  			mode_biaobai();
	  		} else if (move_mode == 'Y') { //元素战技
	  			mode_yuansu();
	  		} else if (move_mode == 'X') { //训练
	  			mode_xiaoxun();
	  		} else if (move_mode == 'W') { //世界之巅
	  			mode_world();
	  		} else if (move_mode == 'o') { //打招呼
	  			mode_hello();
	  		} else if (move_mode == 'U') { //跟随小代
	  			mode_xiaodai();
	  		} else if (move_mode == 'K') { //展示快乐值
	  			mode_happiness();
	  		} else if (move_mode == 'T') { //展示体力值
	  			mode_stamina();
	  		} else if (move_mode == 'Z') { //展示快乐值和体力值
	  			mode_index();
	  		}


	  // 如果OLED正常工作，可以显示简单信息表示程序运行中


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
