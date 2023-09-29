/**
  ************************************* Copyright ****************************** 
  *

  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                              
  *                     By(JCML)
    *
  *    
  * FileName   : Serial.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-01-17         
  * Description: ����Hal��Ĵ����շ����庯��  
					PS��ʹ�øú���ǰ�뵽stm32f1xx_it.h�н�
									void USART1_IRQHandler(void)
									void DMA1_Channel5_IRQHandler(void)
							��������ע�͵�
							
							extern UART_HandleTypeDef huart2;
							extern DMA_HandleTypeDef hdma_usart1_rx;
							extern DMA_HandleTypeDef hdma_usart2_tx;
  * May Be Used Function List:  
1. Head file:
#include ".h" 

2. Function File:


******************************************************************************
 */

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "Serial2.h"
#include "stm32f1xx_it.h"


uint8_t USART2_TX_BUF[MAX_TX_LEN2];   // my_printf�ķ��ͻ��壬�������������á�
volatile uint8_t USART2_TX_FLAG = 0; // USART���ͱ�־����������ʱ��1����volatile���������Ż�
uint8_t u1rxbuf2[MAX_RX_LEN2];         // ���ݽ��ջ���1
uint8_t u2rxbuf2[MAX_RX_LEN2];         // ���ݽ��ջ���2
uint8_t WhichBufIsReady2 = 0;         // ˫����ָʾ����
// 0:u1rxbuf ��DMAռ�ý���,  u2rxbuf ���Զ�ȡ.
// 0:u2rxbuf ��DMAռ�ý���,  u1rxbuf ���Զ�ȡ.
uint8_t *RxData2 = u2rxbuf2;        // ָ�롪��ָ����Զ�ȡ���Ǹ�����
uint8_t *p_IsToReceive2 = u1rxbuf2; // ָ�롪��ָ��ռ�õ��Ǹ�����
char RxDataStr_Port2[MAX_RX_LEN2];
//ע�ⶨ���ʱ��Ҫ����������ָ�밴��WhichBufIsReady�ĳ�ʼ״̬�ȳ�ʼ��һ�¡�
uint8_t Rxflag2;
uint32_t data_length2;


/***********************************************************
*@fuction	:USART1_Init
*@brief		:���ڳ�ʼ��
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void USART2_Init(void)
{
  // ��������1�����ж�
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

  // ����DMA����ͨ���ķ�������жϣ�����ʵ�ַ�װ���ͺ�������ĵȴ�����
  __HAL_DMA_ENABLE_IT(&hdma_usart2_tx, DMA_IT_TC);

  // ������б�־λ����ֹ�ж�����
  __HAL_UART_CLEAR_IDLEFLAG(&huart2);

  // ������Ҫ��DMA����
  // ��ȻDMAû����ǰ׼������һ�ν��յ������Ƕ�ȡ��������
  HAL_UART_Receive_DMA(&huart2, p_IsToReceive2, MAX_RX_LEN2);
}

/***********************************************************
*@fuction	:StringTrans
*@brief		:���ַ�����ת��Ϊ�ַ���Ҳ���ڸú��������if���������ض���ͨ�ŷ�ʽ
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void StringTrans_Port2(void)
{
	uint32_t i=0;
	while (i < data_length2)
	{
		if (RxData2[i] == '\r' ||RxData2[i] == '\n' ||RxData2[i] == 0)
		{
			break;
		}
		else {RxDataStr_Port2[i]=RxData2[i];}
		i++;
	}
	//������������·�ʽ
//	for (i=0; i < data_length; i++)//
//	{
//		if (u1rxbuf[i] == '\r' ||u1rxbuf[i] == '\n')
//		RxDataStr[i]=u1rxbuf[i];
//	}
	RxDataStr_Port2[data_length2]='\0';
}

/***********************************************************
*@fuction	:DMA_USART1_Tx_Data
*@brief		:
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void DMA_USART2_Tx_Data(uint8_t *buffer, uint16_t size)
{
  USART2_TX_Wait();                             // �ȴ���һ�η�����ɣ�USART1_TX_FLAGΪ1�����ڷ������ݣ�
  USART2_TX_FLAG = 1;                           // USART1���ͱ�־���������ͣ�
  HAL_UART_Transmit_DMA(&huart2, buffer, size); // ����ָ�����ȵ�����
}
/***********************************************************
*@fuction	:Myprintf
*@brief		:printf�ض���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void Myprintf_Port2(char *format, ...)
{
  //VA_LIST ����C�����н����������һ��꣬
  //����ͷ�ļ���#include <stdarg.h>�����ڻ�ȡ��ȷ�������Ĳ�����
  va_list arg_ptr;//ʵ�����ɱ䳤�����б�

  USART2_TX_Wait(); //�ȴ���һ�η�����ɣ�USART1_TX_FLAGΪ1�����ڷ������ݣ�

  va_start(arg_ptr, format);//��ʼ���ɱ�����б�����formatΪ�ɱ䳤�б����ʼ�㣨��һ��Ԫ�أ�

  // MAX_TX_LEN+1�ɽ��ܵ�����ַ���(���ֽ�����UNICODEһ���ַ������ֽ�), ��ֹ��������Խ��
  vsnprintf((char *)USART2_TX_BUF, MAX_TX_LEN2 + 1, format, arg_ptr);
  //��USART1_TX_BUF���׵�ַ��ʼƴ�ϣ�ƴ��format���ݣ�MAX_TX_LEN+1���Ƴ��ȣ���ֹ��������Խ��

  va_end(arg_ptr); //ע�����ر�

  DMA_USART2_Tx_Data(USART2_TX_BUF, strlen((const char *)USART2_TX_BUF)); 
  // �ǵð�buf����Ķ�����HAL����ȥ 
}

void USART2_TX_Wait(void)
{
  uint16_t delay = 20000;
  while (USART2_TX_FLAG)
  {
    delay--;
    if (delay == 0)
      return;
  }
}

/***********************************************************
*@fuction	:GetRxFlag
*@brief		:���ؽ������ݵı�־ͬʱ����ת��Ϊ�ַ���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

uint8_t GetRxFlag_Port2(void)
{
	if (Rxflag2==1){StringTrans_Port2();Rxflag2=0;return 1;}
	else {return 0;}
}


/***********************************************************
*@fuction	:DMA1_Channel5_IRQHandler
*@brief		:DMA�жϴ���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void DMA1_Channel7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */
  if (__HAL_DMA_GET_FLAG(&hdma_usart2_tx, DMA_FLAG_TC4) != RESET) //���ݷ�������ж�
  {
    // __HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx, DMA_FLAG_TC4);
    // ��һ������ʵ�� HAL_DMA_IRQHandler(&hdma_usart2_tx) Ҳ����ˡ�

    __HAL_UART_CLEAR_IDLEFLAG(&huart2); //������ڿ����жϱ�־λ�����������ô����Ҳ�ǿ���̬Ŷ~

    USART2_TX_FLAG = 0; // ���÷��ͱ�־λ

    huart2.gState = HAL_UART_STATE_READY;
    hdma_usart2_tx.State = HAL_DMA_STATE_READY;
    __HAL_UNLOCK(&hdma_usart2_tx);
    // ����������HAL�⺯����bug��������Բο��Ҹ�������
    // huart2,hdma_usart2_tx ��״̬Ҫ�ֶ���λ��READY״̬
    // ��Ȼ���ͺ�����һֱ��Ϊͨ��æ���Ͳ��ٷ��������ˣ�
  }
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}


/***********************************************************
*@fuction	:USART1_IRQHandler
*@brief		:���ڽ��������жϴ���
*@param		:void
*@return	:void
*@author	:JCML
*@date		:2023-01-17
***********************************************************/

void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  if (RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
  { 
	  Rxflag2 = 1;	
    // __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    // ��һ������ʵ�� HAL_UART_IRQHandler(&huart2) Ҳ����ˡ�

    HAL_UART_DMAStop(&huart2); // ��DMA����ͣ������ֹ�ٶȹ��쵼���ж����룬���ݱ���д��
	
    data_length2 = MAX_RX_LEN2 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
    // �����ܳ���=���޽��ճ���-DMAʣ��Ľ��ճ���
    if (WhichBufIsReady2)	//WhichBufIsReady=1
    {
      RxData2 = u2rxbuf2;        // u2rxbuf ���Զ�ȡ������ָ��ָ������
      p_IsToReceive2 = u1rxbuf2; // u1rxbuf ��Ϊ��һ��DMA�洢�Ļ��壬ռ��ָ��ָ������
      WhichBufIsReady2 = 0;		//�л�һ��ָʾ��״̬
    }
    else				//WhichBufIsReady=0
    {
      RxData2 = u1rxbuf2;        // u1rxbuf ���Զ�ȡ������ָ��ָ������
      p_IsToReceive2 = u2rxbuf2; // u2rxbuf ��Ϊ��һ��DMA�洢�Ļ��壬ռ��ָ��ָ������
      WhichBufIsReady2 = 1;		//�л�һ��ָʾ��״̬
    }
    //�����濪ʼ���Դ�������յ������������ٸ����ӣ������յ�������ԭԭ�����Ļ���ȥ
      
    //DMA_USART1_Tx_Data(RxData,data_length);//���ݴ��ȥ�����Ⱦ������ݳ���
      
    memset((uint8_t *)p_IsToReceive2, 0, MAX_RX_LEN2);	// �ѽ������ݵ�ָ��ָ��Ļ��������
  }
  HAL_UART_IRQHandler(&huart2);
  HAL_UART_Receive_DMA(&huart2, p_IsToReceive2, MAX_RX_LEN2); //���ݴ�����ϣ�������������
}



