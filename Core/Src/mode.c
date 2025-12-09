#include "stm32f1xx_hal.h"                     // Device header
#include "stdlib.h"
#include "LED.h"
#include "main.h"
#include "OLED.h"
#include "Servo.h"
#include "PWM_HAL.h"
#include "Movement.h"
#include "usart.h"
#include "usart2_hal.h"
#include "OLED_Data.h"
#include "syn6288.h"
#include "stdio.h"
//***各种状态执行函数***//
//想要让动作执行一次后停止，就在末尾加上	move_mode = '0';否则动作持续重复执行，
typedef unsigned char u8; // 补全u8的typedef定义
extern uint8_t move_mode1 ;//用于接收蓝牙串口数据的变量
extern uint8_t move_mode3 ;//用于接收语音识别串口数据的变量
extern uint8_t move_mode ;//决定状态变量
extern uint8_t previous_mode ;//用于保存上一次接收串口数据的变量
extern uint16_t bz_flag;//避障标志位 ON/OFF 默认off
extern uint16_t hw_flag;//红外标志位 ON/OFF 默认off
extern int happiness; //开心指数
extern int stamina;  //体力值
char read[8]={0};  //读取开心值或体力值的字符串变量
extern uint16_t ff; //连续前进计数
extern uint16_t bb; //连续后退计数
extern uint16_t ll; //连续左转计数
extern uint16_t rr; //连续右转计数

// 辅助函数：显示数字
void OLED_PrintNumber(uint8_t x, uint8_t y, int num, uint8_t digits, OLED_ColorMode color) {
    char buffer[10];
    sprintf(buffer, "%d", num);
    OLED_PrintString(x, y, buffer, &font16x16, color);
}

// 辅助函数：清除指定区域
void OLED_ClearArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    for (uint8_t i = 0; i < w; i++) {
        for (uint8_t j = 0; j < h; j++) {
            OLED_SetPixel(x + i, y + j, OLED_COLOR_REVERSED);
        }
    }
}

/**************语音合成模块设置命令*********************/
//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
//m[0~16]:0背景音乐为静音，16背景音乐声音最大
//v[0~16]:0朗读音量为静音，16朗读音量最大
//t[0~5]:0朗读语速最慢，5朗读语速最快

void mode_forward(void)//前进
{

			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP2_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_forward();
			LED13_Turn();//LED13闪烁；用于指示动作正在执行
			if(previous_mode == 'f')
			{
				ff++;
			}
			else
			{
				ff=0;
			}
			if(ff>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;

}
void mode_behind(void)//后退
{			
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP4_Image, OLED_COLOR_NORMAL);
			/*OLED_ShowImage(0, 0, 128, 64, BMP4); //后退图片*/
			OLED_ShowFrame();
			move_behind();
			LED13_Turn();
			if(previous_mode == 'b')
			{
				bb++;
			}
			else
			{
				bb=0;
			}
			if(bb>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;

}
void mode_left(void)//左转
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP3_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_left();
			LED13_Turn();
			if(previous_mode == 'l')
			{
				ll++;
			}
			else
			{
				ll=0;
			}
			if(ll>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;


}
void mode_right(void)//右转
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_right();
			LED13_Turn();
			if(previous_mode == 'r')
			{
				rr++;
			}
			else
			{
				rr=0;
			}
			if(rr>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;
}

void mode_swing_qianhou(void)//前后摇摆
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP11_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_shake_qianhou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
}
void mode_swing_zuoyou(void)//左右摇摆
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP11_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_shake_zuoyou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
}

void mode_dance(void)//跳舞
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP5_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_dance();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;

}
void mode_stand(void)//站立
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			move_stand();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]谢谢");
			HAL_Delay(1000);
			stamina -=1;


}
void mode_slowstand(void)//起身
{
			if (move_mode3 == 'q'){//如果是语音识别模块发送的命令，则播放对应的语音提示
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]早上好");HAL_Delay(2000);
			}
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //立正脸
			OLED_ShowFrame();
			move_slow_stand(previous_mode);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=1;
}

void mode_strech(void)//坐下
{
			if (move_mode3 == 's'){//如果是语音识别模块发送的命令，则播放对应的语音提示
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]坐下");HAL_Delay(500);
			}
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //普通表情
			OLED_ShowFrame();
			move_slow_stand(previous_mode);
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP2_Image, OLED_COLOR_NORMAL);//前进脸
			OLED_ShowFrame();
			move_stretch();
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP12_Image, OLED_COLOR_NORMAL);//猫猫脸
			OLED_ShowFrame();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina +=5;
			happiness +=2;
}
void mode_hello(void)//打招呼
{
			if (previous_mode != '5' && previous_mode != 'D') {
				OLED_NewFrame();
				OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //站立图片
				OLED_ShowFrame();
				move_slow_stand(previous_mode);
			}
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP12_Image, OLED_COLOR_NORMAL);//猫猫脸
	OLED_ShowFrame();
			//move_hello();
		int i;
	for(i=0;i<20;i++)//
	{
	Servo_SetAngle1(90-i);
	Servo_SetAngle2(90+i);
	//Servo_SetAngle3(90+i);
	Servo_SetAngle4(90-i);
		HAL_Delay(7);
	}
	for(i=0;i<40;i++)//
	{
	Servo_SetAngle2(110+i);
	Servo_SetAngle4(70-i);
		HAL_Delay(7);
	}
	for(i=0;i<60;i++)//左右移动头部
	{
		Servo_SetAngle1(70+i);
		HAL_Delay(4);
	}
	HAL_Delay(50);
	//播放打招呼语音并移动头部
	SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5] 你好");
	Servo_SetAngle1(180);
	HAL_Delay(400);
	Servo_SetAngle1(130);
	HAL_Delay(400);
	Servo_SetAngle1(180);
	HAL_Delay(400);
	Servo_SetAngle1(130);
	HAL_Delay(400);
	Servo_SetAngle1(70);
	HAL_Delay(500);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=2;
			happiness +=2;
}
void mode_twohands(void)//交替抬手
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //站立图片
			OLED_ShowFrame();
			move_stand();
			move_two_hands();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=5;
			happiness +=2;

}
void mode_lanyao(void)//伸懒腰
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //站立图片
			OLED_ShowFrame();
			move_slow_stand(previous_mode);
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP9_Image, OLED_COLOR_NORMAL);//开心表情
			OLED_ShowFrame();
			lan_yao();
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL);//立正脸
			OLED_ShowFrame();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina +=5;
			happiness +=1;

}
void mode_headup(void)//抬头
{
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //立正脸
			OLED_ShowFrame();
			move_slow_stand(previous_mode);
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP10_Image, OLED_COLOR_NORMAL);//调皮脸
			OLED_ShowFrame();
			move_head_up();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=5;

}
void mode_sleeppa(void)//趴睡模式
{
	if (previous_mode != '5' && previous_mode != 'q') {
		OLED_NewFrame();
		OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //普通表情
		OLED_ShowFrame();
		move_slow_stand(previous_mode);
	}
	OLED_NewFrame();
	if (rand()%2) {//随机选择表情
		OLED_DrawImageInverted(0, 0, &BMP6_Image, OLED_COLOR_NORMAL); //张嘴睡觉表情
	}
	else{
		OLED_DrawImageInverted(0, 0, &BMP8_Image, OLED_COLOR_NORMAL); //闭嘴睡觉表情
	}
	OLED_ShowFrame();
	move_sleep_p();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]晚安");HAL_Delay(1000);
			previous_mode = move_mode;
			move_mode = '0';
			stamina =100;
			happiness +=15;
}
void mode_sleepwo(void)//侧睡模式
{
		if (previous_mode != '5' && previous_mode != 'q') {
			OLED_NewFrame();
			OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //普通表情
			OLED_ShowFrame();
			move_slow_stand(previous_mode);
		HAL_Delay(1000);
		}
		OLED_NewFrame();
		if (rand()%2) {//随机选择表情
			OLED_DrawImageInverted(0, 0, &BMP6_Image, OLED_COLOR_NORMAL); //张嘴睡觉表情
		}
		else{
			OLED_DrawImageInverted(0, 0, &BMP8_Image, OLED_COLOR_NORMAL); //闭嘴睡觉表情
		}
		OLED_ShowFrame();
		move_sleep_w();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]晚安");HAL_Delay(1000);
			previous_mode = move_mode;
			move_mode = '0';
			stamina =100;
			happiness +=15;
}
void mode_nanshou(void)//难受模式
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP2_Image, OLED_COLOR_NORMAL); //悲伤表情
	OLED_ShowFrame();
	move_sleep_w();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]呜呜");HAL_Delay(1000);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_biaobai(void)//表白模式
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP7_Image, OLED_COLOR_NORMAL); //爱心表情
	OLED_ShowFrame();
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]你是杭州西湖的美景，是无需掩饰的烂漫");HAL_Delay(8000);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP11_Image, OLED_COLOR_NORMAL); //开心迷糊
	OLED_ShowFrame();
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]拥你入怀，就是春风和煦");HAL_Delay(5000);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP9_Image, OLED_COLOR_NORMAL); //开心表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]望你眼眸，便是鸟语花香");HAL_Delay(5000);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP7_Image, OLED_COLOR_NORMAL); //爱心表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]爱你的，小呆");
	move_shake_qianhou();
	move_shake_qianhou();
	HAL_Delay(5000);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=5;
}
void mode_yuansu(void)//元素周期表
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP5_Image, OLED_COLOR_NORMAL); //疑问表情
	OLED_ShowFrame();
	Servo_SetAngle1(135);//调整左右摆臂角度
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]氢氦锂铍硼");HAL_Delay(2500);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP10_Image, OLED_COLOR_NORMAL); //生气表情
	OLED_ShowFrame();
	Servo_SetAngle3(45);//调整上下摆臂角度
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]碳氮氧氟氖");HAL_Delay(2500);
	Servo_SetAngle2(45);//调整手臂角度
	Servo_SetAngle4(135);//调整手臂角度
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP12_Image, OLED_COLOR_NORMAL); //眨眼表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]钠镁铝硅磷");HAL_Delay(2500);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP11_Image, OLED_COLOR_NORMAL); //眨眼表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5] 小呆最可爱");
	move_shake_qianhou();
	move_shake_qianhou();
	move_shake_qianhou();
	HAL_Delay(2000);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=2;
}
void mode_xiaoxun(void)//校训
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP5_Image, OLED_COLOR_NORMAL); //嘿嘿表情
	OLED_ShowFrame();
	Servo_SetAngle1(135);//调整左右摆臂角度
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]自强不息");HAL_Delay(2000);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP10_Image, OLED_COLOR_NORMAL); //生气表情
	OLED_ShowFrame();
	Servo_SetAngle3(45);//调整上下摆臂角度
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]知行合一");HAL_Delay(2000);
	Servo_SetAngle2(45);//调整手臂角度
	Servo_SetAngle4(135);//调整手臂角度
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP12_Image, OLED_COLOR_NORMAL); //眨眼表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]规格严格");HAL_Delay(2000);
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP11_Image, OLED_COLOR_NORMAL); //眨眼表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]功夫到家");
	move_shake_qianhou();
	move_shake_qianhou();
	HAL_Delay(2000);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
}
void mode_world(void)//世界之光
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP5_Image, OLED_COLOR_NORMAL); //疑问表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]为世界之光");
	Servo_SetAngle2(135);//左后脚向前抬
	Servo_SetAngle4(45);//右后脚向前抬
	HAL_Delay(2000);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_xiaodai(void)//语音唤醒小呆
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 0, &BMP1_Image, OLED_COLOR_NORMAL); //普通表情
	OLED_ShowFrame();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]我在");
	HAL_Delay(1000);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_happiness(void)//OLED显示开心值
{
	OLED_NewFrame();
	OLED_PrintString(0, 0, "开心值：", &font16x16, OLED_COLOR_NORMAL);
	if(happiness==100)
	{OLED_PrintNumber(64, 0, 100, 3, OLED_COLOR_NORMAL);}
	else {OLED_PrintNumber(64, 0, happiness, 2, OLED_COLOR_NORMAL);}

	OLED_ShowFrame();
}
void mode_happiness(void)//展示开心值
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 8, &BMP1_Image, OLED_COLOR_NORMAL); //普通表情
	OLED_ShowFrame();
	OLED_happiness();


	sprintf(read,"[v12][m0][t5]当前开心值是%d",happiness);
	SYN_FrameInfo(2,(u8*)read);HAL_Delay(4000);

	if(happiness>=60){
		OLED_NewFrame();
		OLED_DrawImageInverted(0, 8, &BMP9_Image, OLED_COLOR_NORMAL); //开心表情
		OLED_ShowFrame();
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]我现在很开心");
		move_shake_qianhou();
		move_shake_qianhou();HAL_Delay(4000);
	}
	else if(happiness<=10){
		OLED_NewFrame();
		OLED_DrawImageInverted(0, 8, &BMP2_Image, OLED_COLOR_NORMAL);
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]陪小呆玩一会叭");
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}HAL_Delay(3000);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_stamina(void)//OLED显示体力值
{
	OLED_NewFrame();
	OLED_PrintString(0, 0, "体力值：", &font16x16, OLED_COLOR_NORMAL);
	if(stamina==100)
	{OLED_PrintNumber(64, 0, 100, 3, OLED_COLOR_NORMAL);}
	else {OLED_PrintNumber(64, 0, stamina, 2, OLED_COLOR_NORMAL);}

	OLED_ShowFrame();
}
void mode_stamina(void)//展示体力值
{
	OLED_NewFrame();
	OLED_DrawImageInverted(0, 8, &BMP1_Image, OLED_COLOR_NORMAL); //普通表情
	OLED_ShowFrame();
	OLED_stamina();
	sprintf(read,"[v12][m0][t5]当前体力值是%d",stamina);
	SYN_FrameInfo(2,(u8*)read);HAL_Delay(4000);
	if(stamina>=60){
		OLED_NewFrame();
		OLED_DrawImageInverted(0, 8, &BMP9_Image, OLED_COLOR_NORMAL); //开心表情
		OLED_ShowFrame();
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]小呆精力满满，和我一起玩叭");
		move_shake_qianhou();
		move_shake_qianhou();HAL_Delay(6000);
	}
	else if(stamina<=10){
		OLED_NewFrame();
		OLED_DrawImageInverted(0, 8, &BMP2_Image, OLED_COLOR_NORMAL);
		OLED_ShowFrame();
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]让小呆休息一下叭");
		if (rand()%2) //随机选择睡眠动作
			{move_sleep_p();}
		else{move_sleep_w();}
	}HAL_Delay(4000);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_index(void)//展示开心值和体力值
{
	OLED_NewFrame();
	OLED_PrintString(0, 0, "开心值：", &font16x16, OLED_COLOR_NORMAL);
	if(happiness==100)
	{OLED_PrintNumber(64, 0, 100, 3, OLED_COLOR_NORMAL);}
	else {OLED_PrintNumber(64, 0, happiness, 2, OLED_COLOR_NORMAL);}
	OLED_PrintString(0, 16, "体力值：", &font16x16, OLED_COLOR_NORMAL);
	if(stamina==100)
	{OLED_PrintNumber(64, 16, 100, 3, OLED_COLOR_NORMAL);}
	else {OLED_PrintNumber(64, 16, stamina, 2, OLED_COLOR_NORMAL);}
	OLED_ShowFrame();
	sprintf(read,"[v12][m0][t5]当前的开心值为%d体力值为%d",happiness,stamina);
	SYN_FrameInfo(2,(u8*)read);HAL_Delay(6000);
	previous_mode = move_mode;
	move_mode = '0';
}

