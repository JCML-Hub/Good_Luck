/**
  ************************************* Copyright ******************************
  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                     By(JCML)

  * FileName   : Key.c
  * Version    : v1.0
  * Author     : JCML
  * Date       : 2023-07-01
  * Description: 以PB14和PB15分别为按键1和按键2的按键读取函数
******************************************************************************
 */

#include "main.h"
#include "Basic.h"
#include "Delay.h"

/***********************************************************
*@fuction	:Key_GetNum();
*@brief		:获取被按下的键码
*@param		:void
*@return	:KeyNum
*@author	:JCML
*@date		:2023-07-01
***********************************************************/

uint8_t GetKeyNum(void)
{
  uint8_t KeyNum = 0;
  if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)==0)
    KeyNum=1;
//  if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)==0)
//    KeyNum=2;
  return KeyNum;
}
//void LED_ON(uint8_t Which)
//{
//  if (Which==1) {
//    HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
//  } else if (Which==2){
//    HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
//  }
//}

//void LED_OFF(uint8_t Which)
//{
//  if (Which==1) {
//    HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
//  } else if (Which==2){
//    HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
//  }
//}
//void LED_TogglePin(uint8_t Which)
//{
//  if (Which==1) {
//    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//  } else if (Which==2){
//    HAL_GPIO_TogglePin(LED2_GPIO_Port,LED1_Pin);
//  }
//}
void BEEP_Moment(void){
  HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
  Delay_ms(40);
  HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
}
