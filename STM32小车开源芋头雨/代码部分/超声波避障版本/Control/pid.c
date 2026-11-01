#include "myfile.h"

uint8_t Place_Enable,PWM_Enable=1;
uint8_t Angle_Enable=0;
int Speed_Out_L,Speed_Out_R,Place_Out,Angle_out=0;
float sensor_err=0;
float final_err=0;

int Basic_Speed=30;  //速度调节  

int Left_Speed,Right_Speed=0;
int Speed_PID[3] = {150,0,80}; 
float Place_PD[4] = {5.0,0.0,4.3,0.0};
float Angle_PD[4] = {4,0,2,0};
void Control()
{	
			
			
			update_yaw();
	
			sensor_err = Error_Calcaulate(); 
	
			final_err = get_fused_error(sensor_err,gyro_z);			
	
			if(Place_Enable)
			{
				Place_Out=(int)Place_Control(final_err,0,Place_PD);
			}
					
			if(Angle_Enable)
			{
				Angle_out=(int)Place_Control(yaw,0,Angle_PD);
				Angle_out=Min_Max(Angle_out ,-50, 50);
			}
			
			Element_Process();

			Encoder_Read();
			
			Different_Speed();		

			Speed_Out_L=PID_Control(Speed_L,Left_Speed,Speed_PID); 
			Speed_Out_R=PID_Control(Speed_R,Right_Speed,Speed_PID);
			
			

			Speed_Out_L=Min_Max( Speed_Out_L ,-Max_PWM, Max_PWM );
			Speed_Out_R=Min_Max( Speed_Out_R ,-Max_PWM, Max_PWM );
			
			if(PWM_Enable)
			{
				Motor_SetPWM_L(Speed_Out_L);
				Motor_SetPWM_R(Speed_Out_R);
//				Motor_SetPWM_L(7200);
//				Motor_SetPWM_R(7200);			
			}
			 
}

float Place_Control(float NowPoint, float SetPoint, float *TURN_PID) 
{
	static float LastError = 0; 
	float KP,KPP,KD,KDD; 
	float NowError, Out; 
	NowError = SetPoint - NowPoint; 
	KP = *TURN_PID; 
	KPP= *(TURN_PID+1);
	KD = *(TURN_PID+2); 
	KDD= *(TURN_PID+3);
	Out = KP * NowError+(KPP*(NowError*fabs(NowError)))+ KD *(NowError-LastError)+KDD*(gyro_z); 
	if(fabs(Out)<=5)Out=0;
	LastError = NowError; 
	return Out; 
}


int PID_Control(int NowPoint, int SetPoint, int *TURN_PID) 
{
	static int Integral,LastError = 0;      
	int KP,KI,KD,Out,NowError; 
	KP = *TURN_PID; 
	KI = *(TURN_PID+1); 
	KD = *(TURN_PID+2); 
	
	NowError = SetPoint - NowPoint;
    
	Integral += NowError;
	Integral = Min_Max(Integral, -INTEGRAL_MAX, INTEGRAL_MAX); 
	
	Out = KP * NowError + KI * Integral+KD *(NowError-LastError);
	LastError=NowError;
	
	return Out;
}

void Different_Speed() 
{ 
 float k;  
 float Turn_factor=0.5;
		
 if(Place_Out >= 0) 
	 {
		k = Place_Out * 0.01+Angle_out* 0.01; 
		 Left_Speed = Basic_Speed * (1 - k); 
		 Right_Speed = Basic_Speed * (1 + k*Turn_factor); 
	 } 
	 else 
	{ 
		k = -(Place_Out * 0.01+Angle_out* 0.01); 
		Left_Speed = Basic_Speed * (1 + k*Turn_factor); 
		Right_Speed = Basic_Speed * (1 - k); 
  } 
}
