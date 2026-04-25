#include "Sys.h"

volatile bool Self_left,Self_right,Move_Mode_flag,Motor_switch; // 遥控状态位：左旋/右旋/模式切换/电机使能
uint8_t Move_Mode;
int16_t x,y;
float Kp = 0 ,Ki = 0,Kd = 0;
