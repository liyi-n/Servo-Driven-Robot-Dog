# 基于舵机驱动的四足机械狗
本项目使用HAL库开发，实现了一个由STMSTM32F103C8T6控制的舵机驱动机器狗，支持蓝牙连接和语音控制，能够实现自主避障。其中语音合成，识别和反馈功能由ASR—PRO核心板实现。  
外观如图
![layout-collage-1765355536891](https://github.com/user-attachments/assets/d5c2ae3f-c9dc-4f57-967a-19546988ef4f)


## 开发设备
控制器：STM32F103C8T6+ASR-PRO2M核心板  
本项目使用接口如下图所示：
<img width="3229" height="3058" alt="C8T6" src="https://github.com/user-attachments/assets/877514fc-06af-4393-9e07-47b0947da778" />  
注意：对于ASR-PRO可以不用官方下载器，采用USB-TTL同样可以烧录程序
红线5V 黑线GND 核心板PB5接TTL的RX PB6接TTL的TX
## 外设使用情况
### 主要功能
- **微控制器**：STM32F103C8T6（ARM Cortex-M3）
- **舵机**：4个用于腿部运动的舵机（MG90）
- **蓝牙遥控模块**：兼容USART1通信
- **语音识别模块**：兼容USART3通信
- **语音合成模块**：SYN6288（通过USART2连接）
- **OLED显示屏**：I2C接口（128x64像素）
- **红外传感器**：用于障碍物检测
- **超声波传感器**：HC-SR04（用于距离测量）
- **电源**：5V电源（用于舵机和模块），3.3V电源（用于STM32）

- ### 通信协议
- **USART1**：蓝牙遥控通信（115200波特率）
- **USART2**：语音合成模块通信（9600波特率）
- **USART3**：语音识别模块通信（9600波特率）
- **I2C1**：OLED显示屏通信

- ### GPIOA
| 引脚 | 功能 | 描述 |
|-----|------|------|
| PA0 | OUT | 超声波传感器TRIG |
| PA1 | IN | 超声波传感器ECHO |
| PA2 | USART2_TX | 语音合成模块TX |
| PA3 | USART2_RX | 语音合成模块RX |
| PA6 | TIM3_CH1 | 右前腿舵机PWM |
| PA7 | TIM3_CH2 | 右后腿舵机PWM |
| PA9 | USART1_TX | 遥控模块TX |
| PA10 | USART1_RX | 遥控模块RX |
| PA4, PA5, PA11, PA12 | IN | 红外避障传感器 |

### GPIOB
| 引脚 | 功能 | 描述 |
|-----|------|------|
| PB0 | TIM3_CH3 | 左前腿舵机PWM |
| PB1 | TIM3_CH4 | 左后腿舵机PWM |
| PB8 | I2C1_SCL | OLED显示屏SCL |
| PB9 | I2C1_SDA | OLED显示屏SDA |
| PB10 | USART3_TX | 语音识别模块TX |
| PB11 | USART3_RX | 语音识别模块RX |

### GPIOC
| 引脚 | 功能 | 描述 |
|-----|------|------|
| PC13 | OUT | 状态LED |

### 参考资料
【曼波机器狗手把手教程来了！】 https://www.bilibili.com/video/BV1TZWBzhEFv/?share_source=copy_web&vd_source=b369e2d421df47243943c3876c2ba6ee  
波特律动取模：https://led.baud-dance.com/
