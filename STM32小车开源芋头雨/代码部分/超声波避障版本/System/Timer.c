#include "myfile.h"                  // Device header

uint8_t Ultrasonic_Flag = 0;
static uint8_t Ultrasonic_Counter = 0;


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period =100-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
	
}

/************************中断***************************/
void TIM2_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)

		
		Control();      //控制函数（最主要的控制都在这里面） 
		Key_Tick();			//获取按键值，江科大的定时器非阻塞	
		{					
		Ultrasonic_Counter++;
		if (Ultrasonic_Counter >= 5)
		{
			Ultrasonic_Counter = 0;
			Ultrasonic_Flag = 1;
		}
		if (Ultrasonic_Flag)
			{
			Ultrasonic_Flag = 0;
			Ultrasonic1_UpdateDistance();
			Ultrasonic2_UpdateDistance();
			}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}




/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
