//
// Created by mulai on 2023/6/18.
//

#ifndef BALANCECAR_V1_0_MOTOR_H
#define BALANCECAR_V1_0_MOTOR_H
#include "main.h"
// 定义目的车轮
#define W1 1
#define W2 2

/*定义电机的PWM驱动Timer 以及旋转编码器的接口Timer以及引脚*/
#define Motor_Timer1 htim1
#define Encoder_Timer1 htim2
#define Encoder_Timer2 htim3
#define Encoder_TimeCounter htim4 // 其中Encoder_TimeCounter是用于计算速度的
#define Motor1 TIM_CHANNEL_1
#define Motor2 TIM_CHANNEL_4

typedef struct
{
    short Temp_W1; // 暂存值，用于调试滤波
    short Temp_W2;

    float M1_ActualSpeed;
    float M2_ActualSpeed; // 接收来自电机的原始速度

    float Error;
} _Motor;

extern _Motor _motor;

void Motor_Init(void);
void W1_Control(int32_t Speed);
void W2_Control(int32_t Speed);
void W3_Control(int32_t Speed);
void W4_Control(int32_t Speed);
void Servo1_Set(uint16_t Angle);
void GetSpeed(_Motor *speed);
void Encode_CallBack(_Motor *speed);
#endif // BALANCECAR_V1_0_MOTOR_H
