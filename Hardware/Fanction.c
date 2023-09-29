//
// Created by mulai on 2023/9/17.


#include "Fanction.h"
#include "OLED.h"
#include "Basic.h"
#include "Motor.h"
#include "Serial.h"
#include "Serial2.h"
#include "tim.h"
#include "HCSR04.h"

_Values values;

/*测试蓝牙串口接收数据是否正常*/
void BT_Test(void){
  if (GetRxFlag())//接收来自串口1
  {
    Myprintf_Port2("%d %d %d %d\r\n", (int8_t)RxOrder[0], (int8_t)RxOrder[1], (int8_t)RxOrder[2], (int8_t)RxOrder[3]);
  }
}

/*RGB灯的显示 compare值小于600*/
void RGBShow(uint8_t compare){
  static uint16_t counter=0;
  counter++;
  counter%=600;
  if (compare<counter)
  {
    switch (counter / 10)
    {
    case 0:
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGBB1_GPIO_Port, RGBB1_Pin, GPIO_PIN_RESET);
        break;
    case 1:
        HAL_GPIO_WritePin(RGBA2_GPIO_Port, RGBA2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGBB2_GPIO_Port, RGBB2_Pin, GPIO_PIN_RESET);
        break;
    case 2:
        HAL_GPIO_WritePin(RGBA3_GPIO_Port, RGBA3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGBB3_GPIO_Port, RGBB3_Pin, GPIO_PIN_RESET);
        break;
    default:
        break;
    }
  }else{
    switch (counter / 10)
    {
    case 0:
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RGBB1_GPIO_Port, RGBB1_Pin, GPIO_PIN_SET);
        break;
    case 1:
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RGBB2_GPIO_Port, RGBB2_Pin, GPIO_PIN_SET);
        break;
    case 2:
        HAL_GPIO_WritePin(RGBA3_GPIO_Port, RGBA3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RGBB3_GPIO_Port, RGBB3_Pin, GPIO_PIN_SET);
        break;
    default:
        break;
    }
  }
}

/*测试电机*/
void Motor_Test(void){
  W1_Control(100);
  W2_Control(100);
  HAL_Delay(1000);
}

/*初始化*/
void Setup(void){
  USART1_Init();
  USART2_Init();
  Motor_Init();
  OLED_Init();
  Motor_Init();
  OLED_ShowString(0,0,"Welcome to use",16);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//每100ms进一次中断
{
  if(htim==(&htim4))
  {
    Encode_CallBack(&_motor);
    time++;//驱动超声波测距模块
  }
}
