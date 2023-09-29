#ifndef __HCSR04_H
#define __HCSR04_H

extern uint64_t time;

void HC_SR04_Init(void);
int16_t GetDistance(void);
float ScanDistance_mm(void);//返回单位为毫米的距离结果
float ScanDistance_cm(void);//返回单位为厘米的距离结果
float ScanDistance_m(void);//返回单位为米的距离结果

#endif
