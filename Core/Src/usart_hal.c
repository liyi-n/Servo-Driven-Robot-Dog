#include "usart_hal.h"
#include "stm32f1xx_hal.h"
// USART1相关变量
uint8_t USART1_RxData;     // 定义串口接收的数据变量
uint8_t USART1_RxFlag;     // 定义串口接收的标志位变量

// USART3相关变量
uint8_t USART3_RxData;     // 定义串口接收的数据变量
uint8_t USART3_RxFlag;     // 定义串口接收的标志位变量

// USART2相关变量（外部声明）
extern uint8_t USART2_RxData;
extern uint8_t USART2_RxFlag;



// USART句柄
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;

/**
  * 函    数：USART1初始化
  * 参    数：无
  * 返 回 值：无
  */
void USART1_Init(void)
{
    // 使能时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();    // 开启GPIOA的时钟
    __HAL_RCC_USART1_CLK_ENABLE();   // 开启USART1的时钟

    // GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;

    // PA9 - TX
    GPIO_InitStructure.Pin = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10 - RX
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART初始化
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);

    // 开启接收中断
    HAL_UART_Receive_IT(&huart1, &USART1_RxData, 1);

    // 配置中断优先级
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/**
  * 函    数：USART3初始化
  * 参    数：无
  * 返 回 值：无
  */
void USART3_Init(void)
{
    // 使能时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();    // 开启GPIOB的时钟
    __HAL_RCC_USART3_CLK_ENABLE();   // 开启USART3的时钟

    // GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;

    // PB10 - TX
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    // PB11 - RX
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    // USART初始化
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);

    // 开启接收中断
    HAL_UART_Receive_IT(&huart3, &USART3_RxData, 1);

    // 配置中断优先级
    HAL_NVIC_SetPriority(USART3_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/**
  * 函    数：USART1发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void USART1_SendByte(uint8_t Byte)
{
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * 函    数：USART1发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void USART1_SendArray(uint8_t *Array, uint16_t Length)
{
    HAL_UART_Transmit(&huart1, Array, Length, HAL_MAX_DELAY);
}

/**
  * 函    数：USART1发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void USART1_SendString(char *String)
{
    while (*String != '\0')
    {
        USART1_SendByte(*String);
        String++;
    }
}

/**
  * 函    数：Serial_Pow（内部使用）
  * 参    数：X 底数
  * 参    数：Y 指数
  * 返 回 值：X的Y次方
  */
static uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;    // 定义结果变量并赋值为1
    while (Y --)
    {
        Result *= X;        // 将X累乘到结果变量
    }
    return Result;
}

/**
  * 函    数：USART1发送一个数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void USART1_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i ++)
    {
        USART1_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * 函    数：重定向c库函数printf到USART1
  * 参    数：ch 字符
  * 参    数：f 文件指针
  * 返 回 值：字符
  */
int USART1_fputc(int ch, FILE *f)
{
    USART1_SendByte(ch);
    return ch;
}

/**
  * 函    数：USART1_Printf（自定义printf函数）
  * 参    数：format 格式化字符串
  * 参    数：... 可变参数列表
  * 返 回 值：无
  */
void USART1_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    USART1_SendString(String);
}

/**
  * 函    数：USART3发送一个字符串
  * 参    数：DAT 要发送字符串的首地址
  * 参    数：len 要发送的长度
  * 返 回 值：无
  */
void USART3_SendString(uint8_t *DAT, uint8_t len)
{
    HAL_UART_Transmit(&huart3, DAT, len, HAL_MAX_DELAY);
}

/**
  * 函    数：获取USART1接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t USART1_GetRxFlag(void)
{
    if (USART1_RxFlag == 1)
    {
        USART1_RxFlag = 0;
        return 1;
    }
    return 0;
}

/**
  * 函    数：获取USART1接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t USART1_GetRxData(void)
{
    return USART1_RxData;
}

/**
  * 函    数：获取USART3接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t USART3_GetRxFlag(void)
{
    if (USART3_RxFlag == 1)
    {
        USART3_RxFlag = 0;
        return 1;
    }
    return 0;
}

/**
  * 函    数：获取USART3接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t USART3_GetRxData(void)
{
    return USART3_RxData;
}

/**
  * 函    数：USART中断回调函数
  * 参    数：huart UART句柄
  * 返 回 值：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        USART1_RxFlag = 1;
        HAL_UART_Receive_IT(&huart1, &USART1_RxData, 1);
    }
    else if (huart->Instance == USART3)
    {
        USART3_RxFlag = 1;
        HAL_UART_Receive_IT(&huart3, &USART3_RxData, 1);
    }
    else if (huart->Instance == USART2)
    {
        USART2_RxFlag = 1;
        HAL_UART_Receive_IT(&huart2, &USART2_RxData, 1);
    }
}

/**
  * 函    数：USART1中断处理函数
  * 参    数：无
  * 返 回 值：无
  */
/**
  * 函    数：USART1中断处理函数
  * 参    数：无
  * 返 回 值：无
  */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

/**
  * 函    数：USART2中断处理函数
  * 参    数：无
  * 返 回 值：无
  */
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}

/**
  * 函    数：USART3中断处理函数
  * 参    数：无
  * 返 回 值：无
  */

