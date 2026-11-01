#ifndef __PID_H
#define __PID_H



#define INTEGRAL_MAX  1000
#define Min_Max(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define Max_PWM       6800 
void Control(void);
void Different_Speed(void);

int   PID_Control(int NowPiont, int SetPoint, int *TURN_PID); 
int   PD_Control(int NowPiont, int SetPoint, int *TURN_PID); 
float Place_Control(float NowPoint, float SetPoint, float *TURN_PID); //PD¿ØÖÆ
extern int Speed_Out_L,Speed_Out_R,Place_Out,Angle_out;
extern float sensor_err;
extern float final_err;
extern int Basic_Speed;
extern uint8_t Place_Enable,PWM_Enable,Angle_Enable;
#endif
