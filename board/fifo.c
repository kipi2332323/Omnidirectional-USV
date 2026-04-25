#include "fifo.h"

/**********************************************************
*** ZDT_X42_V2.0步进闭环控制例程
*** 编写作者：ZHANGDATOU
*** 技术支持：张大头闭环伺服
*** 淘宝店铺：https://zhangdatou.taobao.com
*** CSDN博客：http s://blog.csdn.net/zhangdatou666
*** qq交流群：262438510
**********************************************************/

__IO FIFO_t rxFIFO = {0};

/**
	* @brief   初始化队列
	* @param   无
	* @retval  无
	*/
void fifo_initQueue(void)
{
	rxFIFO.ptrRead  = 0;
	rxFIFO.ptrWrite = 0;
}

/**
	* @brief   入队
	* @param   无
	* @retval  无
	*/
void fifo_enQueue(uint16_t data)
{
	rxFIFO.buffer[rxFIFO.ptrWrite] = data;
	
	++rxFIFO.ptrWrite;
	
	if(rxFIFO.ptrWrite >= FIFO_SIZE)
	{
		rxFIFO.ptrWrite = 0;
	}
}

/**
	* @brief   出队
	* @param   无
	* @retval  无
	*/
uint16_t fifo_deQueue(void)
{
	uint16_t element = 0;

	element = rxFIFO.buffer[rxFIFO.ptrRead];

	++rxFIFO.ptrRead;

	if(rxFIFO.ptrRead >= FIFO_SIZE)
	{
		rxFIFO.ptrRead = 0;
	}

	return element;
}

/**
	* @brief   判断空队列
	* @param   无
	* @retval  无
	*/
bool fifo_isEmpty(void)
{
	if(rxFIFO.ptrRead == rxFIFO.ptrWrite)
	{
		return true;
	}

	return false;
}

/**
	* @brief   计算队列长度
	* @param   无
	* @retval  无
	*/
uint16_t fifo_queueLength(void)
{
	if(rxFIFO.ptrRead <= rxFIFO.ptrWrite)
	{
		return (rxFIFO.ptrWrite - rxFIFO.ptrRead);
	}
	else
	{
		return (FIFO_SIZE - rxFIFO.ptrRead + rxFIFO.ptrWrite);
	}
}



////////////////////////以下为新的缓存区/////////////////////


#include "fifo.h"

/**********************************************************
*** ZDT_X42_V2.0步进闭环控制例程
*** 编写作者：ZHANGDATOU
*** 技术支持：张大头闭环伺服
*** 淘宝店铺：https://zhangdatou.taobao.com
*** CSDN博客：http s://blog.csdn.net/zhangdatou666
*** qq交流群：262438510
**********************************************************/

__IO FIFO_t_new rxFIFO_new = {0};

/**
	* @brief   初始化队列
	* @param   无
	* @retval  无
	*/
void fifo_initQueue_new(void)
{
	rxFIFO_new.ptrRead_new  = 0;
	rxFIFO_new.ptrWrite_new = 0;
}

/**
	* @brief   入队
	* @param   无
	* @retval  无
	*/
void fifo_enQueue_new(uint16_t data)
{
	rxFIFO_new.buffer_new[rxFIFO_new.ptrWrite_new] = data;
	
	++rxFIFO_new.ptrWrite_new;
	
	if(rxFIFO_new.ptrWrite_new >= FIFO_SIZE)
	{
		rxFIFO_new.ptrWrite_new = 0;
	}
}

/**
	* @brief   出队
	* @param   无
	* @retval  无
	*/
uint16_t fifo_deQueue_new(void)
{
	uint16_t element = 0;

	element = rxFIFO_new.buffer_new[rxFIFO_new.ptrRead_new];

	++rxFIFO_new.ptrRead_new;

	if(rxFIFO_new.ptrRead_new >= FIFO_SIZE)
	{
		rxFIFO_new.ptrRead_new = 0;
	}

	return element;
}

/**
	* @brief   判断空队列
	* @param   无
	* @retval  无
	*/
bool fifo_isEmpty_new(void)
{
	if(rxFIFO_new.ptrRead_new == rxFIFO_new.ptrWrite_new)
	{
		return true;
	}

	return false;
}

/**
	* @brief   计算队列长度
	* @param   无
	* @retval  无
	*/
uint16_t fifo_queueLength_new(void)
{
	if(rxFIFO_new.ptrRead_new <= rxFIFO_new.ptrWrite_new)
	{
		return (rxFIFO_new.ptrWrite_new - rxFIFO_new.ptrRead_new);
	}
	else
	{
		return (FIFO_SIZE - rxFIFO_new.ptrRead_new + rxFIFO_new.ptrWrite_new);
	}
}

