/* 预留：如需在头文件中直接访问系统状态，可重新包含 Sys.h */

int Left_Position_PID (int position,int target);
int Right_Position_PID (int position,int target);
int Left_Velocity_PI (int Encoder,int Target);
int Right_Velocity_PI (int Encoder,int Target);
void TIM7_Init(void);
