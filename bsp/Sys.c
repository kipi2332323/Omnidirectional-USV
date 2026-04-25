#include "Sys.h"

volatile bool Self_left,Self_right,Move_Mode_flag,Motor_switch;//ºáÏò-0 ²îËÙ-1 
uint8_t Move_Mode;
int16_t x,y;
float Kp = 0 ,Ki = 0,Kd = 0;