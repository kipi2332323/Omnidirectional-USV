#include <stm32f4xx.h>
#include <stdbool.h>
#include "oled.h"
#include "Encoder.h"
#include "Control.h"
#include "board.h"

extern volatile bool Self_left,Self_right,Move_Mode_flag,Motor_switch; // ң��״̬λ
extern int16_t x,y;
extern uint8_t Move_Mode;
extern float Kp,Ki,Kd;
