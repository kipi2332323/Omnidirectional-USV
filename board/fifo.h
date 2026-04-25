#ifndef __FIFO_H
#define __FIFO_H

#include "stm32f4xx.h" 
#include <stdbool.h>
/**********************************************************
*** ZDT_X42_V2.0步进闭环控制例程
*** 编写作者：ZHANGDATOU
*** 技术支持：张大头闭环伺服
*** 淘宝店铺：https://zhangdatou.taobao.com
*** CSDN博客：http s://blog.csdn.net/zhangdatou666
*** qq交流群：262438510
**********************************************************/

#define 	FIFO_SIZE   128
typedef struct {
	uint16_t buffer[FIFO_SIZE];
	__IO uint8_t ptrWrite;
	__IO uint8_t ptrRead;
}FIFO_t;

extern __IO FIFO_t rxFIFO;

void fifo_initQueue(void);
void fifo_enQueue(uint16_t data);
uint16_t fifo_deQueue(void);
bool fifo_isEmpty(void);
uint16_t fifo_queueLength(void);

/////////////////新的缓存//////////////////////

typedef struct {
	uint16_t buffer_new[FIFO_SIZE];
	__IO uint8_t ptrWrite_new;
	__IO uint8_t ptrRead_new;
}FIFO_t_new;

extern __IO FIFO_t_new rxFIFO_new;

void fifo_initQueue_new(void);
void fifo_enQueue_new(uint16_t data);
uint16_t fifo_deQueue_new(void);
bool fifo_isEmpty_new(void);
uint16_t fifo_queueLength_new(void);
#endif
