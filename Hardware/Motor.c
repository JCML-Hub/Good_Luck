//
// Created by mulai on 2023/6/18.
//
#include "tim.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_gpio.h"
#include "Motor.h"

_Motor _motor;

/***********************************************************
 *@名称 :Motor_Init
 *@描述	:Motor_Init，包含驱动PWM的初始化以及旋转编码器定时器的初始化
 *@参数	:无
 *@返回值	:无
 *@作者	:JCML
 *@日期	:2023-04-06
 ***********************************************************/

void Motor_Init(void)
{
    HAL_TIM_Base_Start_IT(&Motor_Timer1);
    HAL_TIM_PWM_Start(&Motor_Timer1, Motor1);
    HAL_TIM_PWM_Start(&Motor_Timer1, Motor2);
    HAL_TIM_Encoder_Start(&Encoder_Timer1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&Encoder_Timer2, TIM_CHANNEL_ALL); // 开启定时器计数
    HAL_TIM_Base_Start_IT(&Encoder_TimeCounter);             // 开启用于获取计数的定时器
}

/***********************************************************
 *@名称 :W1_Control  &	W2_Control
 *@描述	:后轮1和2控制
 *@参数	:Speed：正负表方向 范围-100~100
 *@返回值	:无
 *@作者	:JCML
 *@日期	:2023-04-06
 ***********************************************************/

void W1_Control(int32_t Speed)
{
    if (Speed >= 0)
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SetCompare(&Motor_Timer1, Motor1, Speed);
    } // 给值
    else
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
        __HAL_TIM_SetCompare(&Motor_Timer1, Motor1, -Speed);
    }
}
void W2_Control(int32_t Speed)
{
    if (Speed >= 0)
    {
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SetCompare(&Motor_Timer1, Motor2, Speed);
    } // 给值
    else
    {
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
        __HAL_TIM_SetCompare(&Motor_Timer1, Motor2, -Speed);
    }
}

/***********************************************************
 *@名称 :GetSpeed
 *@描述	:获取旋转编码器的速度值
 *@参数	:_Motor结构体，该函数放在while循环获取
 *@返回值	:无
 *@作者	:JCML
 *@日期	:2023-06-18
 ***********************************************************/
void GetSpeed(_Motor *speed)
{
    speed->M1_ActualSpeed = -(float)_motor.Temp_W1;
    speed->M2_ActualSpeed = (float)_motor.Temp_W2; // 获取当前轮子的速度,此处直接将速度转为float型，可在后面加上滤波
}

void Encode_CallBack(_Motor *speed)
{
    _motor.Temp_W1 = ((short)__HAL_TIM_GET_COUNTER(&Encoder_Timer1)); // 读取M1的旋转次数,此处取的是霍尔编码器一周计数加一
    _motor.Temp_W2 = ((short)__HAL_TIM_GET_COUNTER(&Encoder_Timer2)); // 读取M2的旋转次数
    __HAL_TIM_SET_COUNTER(&Encoder_Timer1, 0);
    __HAL_TIM_SET_COUNTER(&Encoder_Timer2, 0); // 将两个计数器清零
}
///***********************************************************
//*@名称 :HAL_TIM_PeriodElapsedCallback
//*@描述	:定时器的回调函数,如果有冲突请将其放在main.c中，来调用Encode_CallBack更新计数值
//*@参数	:TIM_HandleTypeDef
//*@返回值	:无
//*@作者	:JCML
//*@日期	:2023-06-18
//***********************************************************/
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//每100ms进一次中断
//{
//  if(htim==(&Encoder_TimeCounter))
//  {
//    _motor.Temp_W1 = ((short)__HAL_TIM_GET_COUNTER(&Encoder_Timer1));//读取M1的旋转次数,此处取的是霍尔编码器一周计数加一
//    _motor.Temp_W2 = ((short)__HAL_TIM_GET_COUNTER(&Encoder_Timer2));//读取M2的旋转次数
//    __HAL_TIM_SET_COUNTER(&Encoder_Timer1, 0);
//    __HAL_TIM_SET_COUNTER(&Encoder_Timer2, 0);//将两个计数器清零
//  }
//}