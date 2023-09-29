#ifndef __SERIAL2_H__
#define __SERIAL2_H__

#define MAX_RX_LEN2 (256U) // 一次性可以接受的数据字节长度，你可以自己定义。U是Unsigned的意思。
#define MAX_TX_LEN2 (512U) // 一次性可以发送的数据字节长度，你可以自己定义。
#include "stdio.h"
#include "string.h"
#include <stdarg.h>

extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

extern uint8_t *RxData2;
extern uint8_t *p_IsToReceive2;
extern uint8_t WhichBufIsReady2;
extern char RxDataStr_Port2[MAX_RX_LEN2];


void USART2_Init(void);
void DMA_USART2_Tx_Data(uint8_t *buffer, uint16_t size);//数组发送串口数据
void Myprintf_Port2(char *format, ...);//仿制printf发送串口数据
void USART2_TX_Wait(void);//发送等待函数
uint8_t GetRxFlag_Port2(void);
void StringTrans_Port2(void);

#endif
