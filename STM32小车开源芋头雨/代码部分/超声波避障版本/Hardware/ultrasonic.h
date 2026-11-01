#ifndef __ultrasonic_H
#define __ultrasonic_H

#include "stm32f10x.h"

/**
 * 超声波模块引脚定义
 * 默认使用 PA15 作为 Trig，PA12 作为 Echo
 * 如与现有传感器引脚冲突，可在此处修改为实际接线的 IO 口
 */
/* 这里按照实际接线使用 PA2 / PA3
 * 假定：PA2 -> Echo（输入），PA3 -> Trig（输出）
 * 如果你实际接反了，只需把下面两个 PIN 互换即可
 *//**
 * 超声波模块1 - 原有的PA2/PA3（前方检测）
 */
#define ULTRASONIC1_TRIG_PORT  GPIOA
#define ULTRASONIC1_TRIG_PIN   GPIO_Pin_3    /* Trig -> PA3 */
#define ULTRASONIC1_ECHO_PORT  GPIOA
#define ULTRASONIC1_ECHO_PIN   GPIO_Pin_2    /* Echo -> PA2 */

/**
 * 超声波模块2 - PA9/PA10（侧方或后方检测）
 */
#define ULTRASONIC2_TRIG_PORT  GPIOA
#define ULTRASONIC2_TRIG_PIN   GPIO_Pin_9    /* Trig -> PA9 */
#define ULTRASONIC2_ECHO_PORT  GPIOA
#define ULTRASONIC2_ECHO_PIN   GPIO_Pin_10   /* Echo -> PA10 */



void Ultrasonic_Init(void);

/**
 * 阻塞式测距接口，由上层周期性调用
 * 返回值：单位 cm，若返回 < 0 表示本次测量失败
 */
float Ultrasonic1_MeasureDistance(void);
float Ultrasonic2_MeasureDistance(void);
/**
 * 提供给控制层使用的全局测距结果（cm）
 * 由 Ultrasonic_UpdateDistance 周期性刷新
 */
extern float g_Ultrasonic1Distance;
extern float g_Ultrasonic2Distance;        // 新增;

/**
 * 障碍物触发刹车的距离阈值（cm），可在运行时修改
 */
extern float g_UltrasonicThreshold;

/**
 * 在主循环中被调用，用一次阻塞测量刷新 g_UltrasonicDistance
 */
void Ultrasonic1_UpdateDistance(void);
void Ultrasonic2_UpdateDistance(void);     // 更新模块2（新增）


float LowPassFilter(float new_value, float* last_value, float alpha);
#endif


