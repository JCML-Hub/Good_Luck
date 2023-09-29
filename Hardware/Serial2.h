#ifndef __SERIAL2_H__
#define __SERIAL2_H__

#define MAX_RX_LEN2 (256U) // һ���Կ��Խ��ܵ������ֽڳ��ȣ�������Լ����塣U��Unsigned����˼��
#define MAX_TX_LEN2 (512U) // һ���Կ��Է��͵������ֽڳ��ȣ�������Լ����塣
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
void DMA_USART2_Tx_Data(uint8_t *buffer, uint16_t size);//���鷢�ʹ�������
void Myprintf_Port2(char *format, ...);//����printf���ʹ�������
void USART2_TX_Wait(void);//���͵ȴ�����
uint8_t GetRxFlag_Port2(void);
void StringTrans_Port2(void);

#endif
