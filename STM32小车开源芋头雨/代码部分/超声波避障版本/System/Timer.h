#ifndef __TIMER_H
#define __TIMER_H

void Timer_Init(void);
uint32_t Get_Tick(void);
extern volatile uint32_t system_ticks;   // Ã¿10msÔö
extern uint8_t Ultrasonic_Flag ;
#endif
