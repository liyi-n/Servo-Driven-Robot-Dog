#include "usart2_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
UART_HandleTypeDef huart2;

uint8_t USART2_RxData;
uint8_t USART2_RxFlag;

/**
  * @brief USART2初始化函数
  * @param 无
  * @retval 无
  */
void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 启用GPIOA和USART2时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    // 配置PA2为USART2_TX
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置PA3为USART2_RX
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置USART2
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);

    // 启用USART2接收中断
    HAL_UART_Receive_IT(&huart2, &USART2_RxData, 1);
}

/**
  * @brief 发送一个字节
  * @param Byte 要发送的字节
  * @retval 无
  */
void USART2_SendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart2, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * @brief 发送字符串
  * @param String 要发送的字符串
  * @param Length 字符串长度
  * @retval 无
  */
void USART2_SendString(uint8_t *String, uint16_t Length)
{
    HAL_UART_Transmit(&huart2, String, Length, HAL_MAX_DELAY);
}

/**
  * @brief 发送数字
  * @param Number 要发送的数字
  * @param Length 数字的位数
  * @retval 无
  */
void USART2_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t Data[10];
    for (uint8_t i = 0; i < Length; i++)
    {
        Data[Length - i - 1] = Number % 10 + '0';
        Number /= 10;
    }
    HAL_UART_Transmit(&huart2, Data, Length, HAL_MAX_DELAY);
}

/**
  * @brief USART2的printf函数
  * @param format 格式化字符串
  * @param ... 可变参数
  * @retval 无
  */
void USART2_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    HAL_UART_Transmit(&huart2, (uint8_t *)String, strlen(String), HAL_MAX_DELAY);
}

/**
  * @brief 获取USART2接收标志位
  * @param 无
  * @retval 接收标志位（1：有数据，0：无数据）
  */
uint8_t USART2_GetRxFlag(void)
{
    if (USART2_RxFlag == 1)
    {
        USART2_RxFlag = 0;
        return 1;
    }
    return 0;
}

/**
  * @brief 获取USART2接收的数据
  * @param 无
  * @retval 接收的数据
  */
uint8_t USART2_GetRxData(void)
{
    return USART2_RxData;
}

/**
  * @brief USART2接收中断回调函数
  * @param huart UART句柄
  * @retval 无
  * @note 该函数已在usart_hal.c中实现，包含了所有USART的中断处理
  */
