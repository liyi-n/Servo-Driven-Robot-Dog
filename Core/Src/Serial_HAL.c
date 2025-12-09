#include "stm32f1xx_hal.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
// 已有的头文件（比如#include "main.h"等）
#include "main.h"                  // 基础HAL库头文件，必须包含
#include "usart.h"
#include "gpio.h"
#include "string.h"// 包含GPIO配置（UART的TX/RX引脚需要GPIO初始化）
// 其他头文件（如Delay.h，按需保留）
#include <stdlib.h>

UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

uint8_t Serial_RxData1;		//定义串口接收的数据变量
uint8_t Serial_RxFlag1;		//定义串口接收的标志位变量

uint8_t Serial_RxData3;
uint8_t Serial_RxFlag3;

extern uint8_t USART2_RxData;
extern uint8_t USART2_RxFlag;

/**
  * 函    数：串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void Serial_Init(void)
{
    /* USART1初始化（蓝牙通信） */
    __HAL_RCC_GPIOA_CLK_ENABLE();    // 开启GPIOA时钟
    __HAL_RCC_USART1_CLK_ENABLE();   // 开启USART1时钟

    /* GPIO配置 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_9;    // TX引脚
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_10;   // RX引脚
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1配置 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);

    /* 开启USART1接收中断 */
    HAL_UART_Receive_IT(&huart1, &Serial_RxData1, 1);

    /* NVIC配置 */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    /* USART3初始化（语音识别） */
    __HAL_RCC_GPIOB_CLK_ENABLE();    // 开启GPIOB时钟
    __HAL_RCC_USART3_CLK_ENABLE();   // 开启USART3时钟

    /* GPIO配置 */
    GPIO_InitStructure.Pin = GPIO_PIN_10;   // TX引脚
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_11;   // RX引脚
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* USART3配置 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);

    /* 开启USART3接收中断 */
    HAL_UART_Receive_IT(&huart3, &Serial_RxData3, 1);

    /* NVIC配置 */
    HAL_NVIC_SetPriority(USART3_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte1(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    HAL_UART_Transmit(&huart1, Array, Length, HAL_MAX_DELAY);
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString1(char *String)
{
    uint16_t Length = 0;
    while (String[Length] != '\0')
    {
        Length++;
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)String, Length, HAL_MAX_DELAY);
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;  // 设置结果初值为1
    while (Y--)
    {
        Result *= X;      // 将X累乘到结果
    }
    return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte1(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
    Serial_SendByte1(ch);
    return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString1(String);
}

/**
  * 函    数：获取串口接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t Serial_GetRxFlag1(void)
{
    if (Serial_RxFlag1 == 1)
    {
        Serial_RxFlag1 = 0;
        return 1;
    }
    return 0;
}

/**
  * 函    数：获取串口接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t Serial_GetRxData1(void)
{
    return Serial_RxData1;
}

/**
  * 函    数：获取串口接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t Serial_GetRxFlag3(void)
{
    if (Serial_RxFlag3 == 1)
    {
        Serial_RxFlag3 = 0;
        return 1;
    }
    return 0;
}

/**
  * 函    数：获取串口接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t Serial_GetRxData3(void)
{
    return Serial_RxData3;
}

/**
  * 函    数：USART1中断回调函数
  * 参    数：huart UART句柄指针
  * 返 回 值：无
  * 注    意：该函数已在usart_hal.c中实现，此处不再重复定义
  */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        Serial_RxFlag1 = 1;
        HAL_UART_Receive_IT(&huart1, &Serial_RxData1, 1);
    }
    else if (huart == &huart3)
    {
        Serial_RxFlag3 = 1;
        HAL_UART_Receive_IT(&huart3, &Serial_RxData3, 1);
    }

    else if (huart->Instance == USART2)
       {
           USART2_RxFlag = 1;
           HAL_UART_Receive_IT(&huart2, &USART2_RxData, 1);
       }
   /* if (huart->Instance == USART1)
    {
        USART1_RxFlag = 1;
        HAL_UART_Receive_IT(&huart1, &USART1_RxData, 1);
    }
    else if (huart->Instance == USART3)
    {
        USART3_RxFlag = 1;
        HAL_UART_Receive_IT(&huart3, &USART3_RxData, 1);
    }*/
}

/**
  * 函    数：USART1中断处理函数
  * 参    数：无
  * 返 回 值：无
  * 注    意：该函数已在usart_hal.c中实现，此处不再重复定义
  */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

/**
  * 函    数：USART3中断处理函数
  * 参    数：无
  * 返 回 值：无
  */
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart3);
}
#endif
// 确保 Serial_HAL.c 中有这个回调函数（处理USART3接收逻辑）
