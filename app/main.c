#include "stm32f4xx.h"                  // Device header
#include "board.h"
#include <stdio.h>
#include "oled.h"
#include "Encoder.h"
#include "Sys.h"
#include "ADC.h"
#include <stdlib.h>
#include <math.h>
#include "UART_ALL.h"
#include "Motor.h"

void Motor_Control();
float Pid_Control_forward();
	
uint16_t pwmLeft,pwmRight;

uint32_t i = 0,Count;
float  Target,Actual,Out;
float Error0, Error1, ErrorInt;	
int16_t Out_Real;//实际输出值
uint8_t correct_data[]= {0x00,0x06,0x06,0x10,0x1c};//角度校准指令
uint8_t zero_data[]= {0xA4,0x06,0x06,0x01,0xB1};
int main(void)
{
	board_init();
	UART_ALL_Init();
	delay_ms(50);
	GY25T_ConfigStream();
	OLED_Init();  // 初始化OLED屏幕
	// Encoder_TIM3_Init(); 
  // Encoder_TIM4_Init(); 
	TIM7_Init();
	
	PWM_Throttle_Init();  // 初始化电机油门PWM
	// 电机测试时，注意先确认前进、后退、左转、右转方向

	// 初始化后让电机回到中位，便于上电检查
//	delay_ms(3000);
//	Set_PWM_All(1500, 1500, 1500, 1500);
//	delay_ms(3000);

	while(1) 
	{
		// OLED_Printf(1,1,"%1d,%1d,%1d,%4d%4d%1d", Self_left, Self_right,Move_Mode, x, y,Motor_switch);
		// OLED_Printf(2,1,"Yaw:%7.2f", yaw_angle_real);
		// OLED_Printf(4,1,"%.2f %.2f %.2f",Kp,Ki,Kd);//显示PID参数
		// OLED_Printf(3,1,"Gz :%7.2f", gyro_z_dps);
		// My_USART_Printf(USART1, "%.2f,%.2f\r\n", Target, yaw_angle_real);
	}
}

void TIM7_IRQHandler(void)//定时器7中断服务函数
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
			if(++Count>=40)
			{
				Count = 0;
				Motor_Control();
				//My_USART_Printf(USART1, "%.2f,%.2f\r\n", Target, yaw_angle_real);
			}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update); 	 
	}	
}

float Pid_Control_forward()
{
	Actual = yaw_angle_real;
	
	Error1 = Error0;
	Error0 = Target - Actual;
	
	if (Error0 > 180.0f)//将角度误差限制到 -180 到 180 度之间
		Error0 -= 360.0f;
	else if (Error0 < -180.0f)
		Error0 += 360.0f;

	if (fabs(Ki) > 0.01)
	{
		ErrorInt += Error0;
	}
	else
	{
		ErrorInt = 0;
	}

	Out = Kp * Error0 + Ki * ErrorInt + Kd * (Error0 - Error1);
	
	if (Out >  400) {Out =  400;}//限制输出幅值，避免控制量过大
	if (Out < -400) {Out = -400;}
	
	return Out;
}

void Motor_Control()
{
	if(Move_Mode_flag == 1)
	{
		//Target = yaw_angle_real;//锁定当前目标角度
		My_USART_SendBytes(USART2,correct_data,5);
	}
	
	if(Motor_switch == 0)
	{
		Out_Real = 0;
		ErrorInt = 0;
		Set_PWM_All(1500, 1500, 1500, 1500);
		return;
	}

	if(Self_left == 0 && Self_right == 0)Move_Mode = 0;
	if(Self_left == 1 && Self_right == 0)Move_Mode = 1;
	if(Self_left == 0 && Self_right == 1)Move_Mode = 2;

	switch(Move_Mode)
	{
		case 0://平移模式 -400 ~ +400
		{
			// 直接按摇杆量输出
//			Set_PWM_All(1500 + x, 1500 + x , 1500 + y, 1500 + y);
//			OLED_Printf(2,1,"%4d,%4d",1500 + x, 1500 + x);
//			OLED_Printf(3,1,"%4d,%4d",1500 + y, 1500 + y);

			// 当前只处理四个固定方向，后续如需全向控制可再补速度映射算法
			//y== 500   前
			//y==-500   后
			//x==-500   左
			//x== 500   右

			if(y==500)//前进，基础推力 300
			{
				Out_Real = (int16_t)Pid_Control_forward();//目标角度为 0 度，修正前进航向
				Set_PWM_All(1500, 1500, 1800 - Out_Real, 1800 + Out_Real);
			}
			else if(y==-500)//后退，基础推力 300
			{
				Out_Real = (int16_t)Pid_Control_forward();//目标角度为 0 度，修正后退航向
				Set_PWM_All(1500, 1500, 1200 - Out_Real, 1200 + Out_Real);
			}
			else if(x==-500)//左移，基础推力 300
			{
				Out_Real = (int16_t)Pid_Control_forward();//目标角度为 0 度，修正横移航向
				Set_PWM_All(1200 - Out_Real, 1200 + Out_Real,1500, 1500);
			}
			else if(x== 500)//右移，基础推力 300
			{
				Out_Real = (int16_t)Pid_Control_forward();//目标角度为 0 度，修正横移航向
				Set_PWM_All(1800 - Out_Real, 1800 + Out_Real,1500, 1500);
			}
			else//静止状态下保持航向不变
			{
				Out_Real = (int16_t)Pid_Control_forward();//目标角度为 0 度，原地修正姿态
				Set_PWM_All(1500 - Out_Real, 1500 + Out_Real, 1500 - Out_Real, 1500 + Out_Real);
			}
		}
		break;

		case 1://左转到目标角度
			Set_PWM_All(1000, 2000, 1000, 2000);
		break;

		case 2://右转到目标角度
			Set_PWM_All(2000, 1000, 2000, 1000);
		break;
	}
}


