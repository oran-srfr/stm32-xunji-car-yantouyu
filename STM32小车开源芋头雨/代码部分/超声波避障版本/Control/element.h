#ifndef _ELEMENT_H__
#define _ELEMENT_H__

void Element_Normal(void);
void Element_Process(void);
void Element_Ring(void);
void Element_Noline(void);
void Element_Stop(void);
void Element_Corner(void);
void Element_Ten(void);
void Element_MoreRing(void);
void Element_Right(void);
void Element_Turnback(void);
void Element_Busy(void);

extern float kL,kR;
extern uint8_t Element_Flag,Ring_Flag,Noline_Flag,Stop_Flag,Ten_Flag,MoreRing_Flag,Turnback_Flag ,Busy_Flag,Busy_Flag2 ;
extern int Ring_Turn,Ten_Temp,Turnback_time;
#endif
