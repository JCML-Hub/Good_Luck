#include "main.h"

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
/*******************************************************************************
* 函 数 名         : delay_us
* 函数功能           : 延时函数，延时us
* 输    入         : i
* 输    出         : 无
*******************************************************************************/
void Delay_us(uint32_t i)
{
  uint32_t temp;
  SysTick->LOAD=9*i;         //设置重装数值, 72MHZ时
  SysTick->CTRL=0X01;         //使能，减到零是无动作，采用外部时钟源
  SysTick->VAL=0;                //清零计数器
  do
  {
    temp=SysTick->CTRL;           //读取当前倒计数值
  }
  while((temp&0x01)&&(!(temp&(1<<16))));     //等待时间到达
  SysTick->CTRL=0;    //关闭计数器
  SysTick->VAL=0;        //清空计数器
}
//void Delay_us(uint32_t xus)
//{
//  SysTick->LOAD = 72 * xus;				//设置定时器重装值
//  SysTick->VAL = 0x00;					//清空当前计数值
//  SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
//  while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
//  SysTick->CTRL = 0x00000004;				//关闭定时器
//}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
  while(xms--)
  {
    Delay_us(1000);
  }
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
  while(xs--)
  {
    Delay_ms(1000);
  }
}
