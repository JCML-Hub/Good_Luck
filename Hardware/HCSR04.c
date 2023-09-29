/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2023,--,China, CUIT.
  *                            All Rights Reserved
  *                     By(JCML)
  * FileName   : HCSR04.c   
  * Version    : v1.0		
  * Author     : JCML			
  * Date       : 2023-09-20    
  * Description: HCSR04测距模块， 本程序中使用了TIM3， 以及Delay
******************************************************************************
 */

#include "main.h"
#include "Delay.h"
#include "tim.h"
#include "HCSR04.h"

//此处定义所用的定时器
#define Timer	htim3

uint64_t time=0;//声明变量，用来计时
uint64_t time_end=0;//声明变量，存储回波信号时间
uint32_t Distance;


/***********************************************************
*@名称 	:HC_SR04_Init
*@描述	:初始化 开启TIM中断
*@参数	:无
*@返回值:无
*@作者	:JCML
*@日期	:2023-09-20
***********************************************************/

void HC_SR04_Init(void)
{
	HAL_TIM_Base_Start_IT(&Timer);
	HAL_GPIO_WritePin(TRIG_GPIO_Port,TRIG_Pin,GPIO_PIN_RESET);//输出低电平
	Delay_us(15);											//延时15微秒
}

/***********************************************************
*@名称 	:GetDistance
*@描述	:获取距离
*@参数	:无
*@返回值:Distance初始值
*@作者	:JCML
*@日期	:2023-09-20
***********************************************************/
int16_t GetDistance(void)//测距并返回单位为毫米的距离结果
{
	HAL_GPIO_WritePin(TRIG_GPIO_Port,TRIG_Pin,GPIO_PIN_SET);//输出高电平
	Delay_us(15);										//延时15微秒
	HAL_GPIO_WritePin(TRIG_GPIO_Port,TRIG_Pin,GPIO_PIN_RESET);//输出低电平		
	while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == 0);//等待低电平结束，转为高电平开始计时
	time=0;												//计时清零
	while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == 1);		//等待高电平结束，转为低电平结束计时
	time_end=time;										//记录结束时的时间
	if(time_end/100<38)									//判断是否小于38毫秒，大于38毫秒的就是超时，直接调到下面返回0
	{
		Distance=(time_end*346)/2;						//计算距离，25°C空气中的音速为346m/s
	}
	return Distance;									//返回测距结果
}


/*均值滤波*/
float averageFilter(float in_data)
{
	static float data[10];
	float sum = 0;
	for(int i=0; i<9; i++)
	{
		data[i]=data[i+1];
		sum = sum + data[i];
	}
	data[9] = in_data;
	sum = sum + data[9];
	return(sum/10);
}
/***********************************************************
*@名称 	:ScanDistance_mm
*@描述	:转换得到的距离值
*@参数	:无
*@返回值:转换得到的距离值
*@作者	:JCML
*@日期	:2023-09-20
***********************************************************/
float ScanDistance_mm(void)//返回单位为毫米的距离结果,加上一个简单的均值滤波
{
	float temp=0;
	temp=GetDistance() / 100.0f;
	return averageFilter(temp);
}

/***********************************************************
*@名称 	:HAL_TIM_PeriodElapsedCallback
*@描述	:中断回调函数此处用于计时
*@参数	:无
*@返回值	:无
*@作者	:JCML
*@日期	:2023-02-10
***********************************************************/

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)			//更新中断函数，用来计时，每10微秒变量time加1
// {
// 	if (htim == &htim3)		//获取TIM3定时器的更新中断标志位
// 	{
// 		time++;
// 	}
// }
