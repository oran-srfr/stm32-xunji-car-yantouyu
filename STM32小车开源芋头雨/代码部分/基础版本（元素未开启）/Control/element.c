#include "myfile.h"

/************************元素***************************/

//变量定义

uint8_t Element_Flag=0;
uint8_t Ten_Flag=0;
uint8_t Ring_Flag=0;
uint8_t Noline_Flag=0;
uint8_t Stop_Flag = 0;
uint8_t MoreRing_Flag=0;
uint8_t Right_Flag=0;
uint8_t Turnback_Flag=0;
int Turnback_time=0;
uint8_t K=0;
int Speed_Choice[5]={65,0,105,80,40};    //速度决策
int Ring_Turn=0;
int Ten_Temp=0;
/************************元素控制台***************************/
//在这里启动或者关掉某个元素

void Element_Process()
{
		Element_Normal();		//正常状态下，默认打开转向环和速度环
//	    Element_Turnback();
//		Element_MoreRing();		//多段圆环
//		Element_Ring();			//圆环
//		Element_Noline();		//丢线处理
//		Element_Stop();			//停车
//		Element_Ten();			//十字识别（但未处理）
//		Element_Right();
}


void Element_Normal()
{
 if(Element_Flag==0)
 {
	 Place_Enable=1;
	 PWM_Enable=1;
	 Stop_Flag = 0;
//	 Basic_Speed=Speed_Choice[4];   //如果需要不同元素不同速度，在这里给速度赋值
 }
}


//十字识别，辅助判断圆环，我的圆环在十字之后，根据赛道情况自己调整
void Element_Ten()
{
	if ((Ten_Flag==0)&&(L2 == 1 && L1 == 1 && M == 1 && R1 == 1 && R2 == 1))
	{
		Clear_Location();
		Element_Flag=1;
		Ten_Flag=1;  	
	}	
	if(Ten_Flag==1||Ten_Flag==2)
	{		
		if(Location<100)Basic_Speed=100;	
		else if(Location>150&&Location<650){Basic_Speed=Speed_Choice[0]+5;}
		else if(Location>700&&Location<800){Basic_Speed=Speed_Choice[3];}
		else{Basic_Speed=Speed_Choice[0];}
	}		
	if(Ten_Flag==1&&Location>30)
	{
		Ten_Flag=2;	
	}

	if ((Ten_Flag==2)&&(L2 == 1 && L1 == 1 && M == 1 && R1 == 1 && R2 == 1))
	{
		Clear_Location();
		Ten_Flag=3;  
		Place_Enable=0;//关掉转向环  
		Place_Out=80; //给一个固定的差速
	}	
	if(Ten_Flag==3&&Location>10&& M == 1)
	{
	 Place_Enable=1;
	 Ten_Flag=4;			
	}	
	if(Ten_Flag==4)
	{		
		if(Location<50){Basic_Speed=Speed_Choice[0]-5;}
		else if(Location>100&&Location<150){Basic_Speed=50;}
		else if(Location>150&&Location<200){Basic_Speed=Speed_Choice[2];}
		else if(Location>200&&Location<270){Basic_Speed=Speed_Choice[2]-15;}
		else
		{Basic_Speed=Speed_Choice[0];}
	}	 
	if(Ten_Flag==4&&L2 == 1 && L1 == 1 && M == 1 && R1 == 1 && R2 == 1)
	{
		Clear_yaw();	
		Basic_Speed=40;
		Ten_Flag=5;
		Clear_Location();
		Place_Enable=0;//关掉转向环  
		Place_Out=-75; //给一个固定的差速   		
	}	
	if(Ten_Flag==5&&(Location>10&&fabs(yaw)>20))
	{
		Basic_Speed=45;
		Place_Out=0;
		Ten_Flag=6;
		Clear_Location();
	}	
	if(Ten_Flag==6&&(Location>10&&M == 1))
	{
		Place_Enable=1;
		Ten_Flag=7;
	}	
	if(Ten_Flag==7&&((L2 == 1&& R2 == 1)))//(L2 == 1&& R2 == 0)||
	{
		Ten_Flag=8;
		Clear_Location(); 
		Place_Enable=0;//关掉转向环  
		Place_Out=-60; //给一个固定的差速   
	}
	if(Ten_Flag==8&&(Location>5&&M == 1))
	{
		Place_Enable=1;//关掉转向环  	
		Ten_Flag=9;
		Basic_Speed=Speed_Choice[0]-5;
	}		
	if(Ten_Flag==9)
	{
		if(Location>120&&Location<200)
		{Basic_Speed=Speed_Choice[2]+5;}
		else{Basic_Speed=Speed_Choice[0]-5;}
	}
	
	if(Ten_Flag==9 &&(L2 == 1 && L1 == 1 && M == 1 && R1 == 1 && R2 == 1)&&Location>150)
	{
	 Clear_yaw();	
	 Clear_Location();
	 Place_Enable=0;
	 Ten_Flag=10;	
	 Place_Enable=0;//关掉转向环  
	 Place_Out=-90; //给一个固定的差速
	}	
	if(Ten_Flag==10&&Location>10&&M == 1 )
	{
	 Place_Enable=1;
	 Ten_Flag=11;
	}
	
	if(Ten_Flag==11 &&(L2 == 0&& R2 == 1)&&yaw>60)
	{
	 Clear_Location();
	 Place_Enable=0;
	 Ten_Flag=12;	
	 Place_Enable=0;//关掉转向环  
	 Place_Out=-90; //给一个固定的差速
	}	
	if(Ten_Flag==12&&Location>8&&M == 1 )
	{
	 Place_Enable=1;
	 Ten_Flag=13;
	 Basic_Speed=Speed_Choice[2]-10;
	}	
	
}	
//圆环
void Element_Ring()
{
	if(Element_Flag==0&&Ring_Flag==0&&Noline_Flag==4&&Ten_Flag==1)	//调试状态下可以将&&Noline_Flag==4&&Ten_Flag==1移出，这里我是比赛为了防止误触，做了一个元素顺序限制
	{
			//第一次检测入环
			if(R2==1&&M==1&&(L1==0||R1==0)&&L2==0)//初次检测
		{		
				Ring_Flag=1;	
				Clear_Location();			
		}
	}
		//第一次检测到后过10cm再进行检测然后入环
	if(Ring_Flag==1&&Location>=10.5)
	{		
		if((L1==0&&M==1&&R1==0)||((L1==1||R1==1)&&M==1))
		{
			Ring_Flag=2;	
			Clear_Location();	
			Element_Flag=1;	
			Place_Enable=0;//关掉转向环
			Place_Out=-15; //给一个固定的差速
		}
		else{Ring_Flag=0;}
	}
		//结束入环动作，正常巡线
	if(Ring_Flag==2&&Location>30)
	{
		Place_Enable=1;
		Ring_Flag=3;
		Basic_Speed=Speed_Choice[2];
		
	}
	//出环动作和入环同理
	if(Ring_Flag==3&&Location>380)
	{
		if(L2==1&&M==1&&R2==0)
		{
			Clear_Location();	
			Place_Enable=0;
			Place_Out=-15;	
			Ring_Flag=4;			
		}
	}
	//出环结束
	if(Ring_Flag==4&&Location>50)
	{
		Element_Flag=0;
		K=1;
		Ring_Flag=5;
		Clear_Location();
		
	}
}


//丢线   
void Element_Noline()
{
		if (Element_Flag == 0 &&Noline_Flag==0)
		{
			if (L2 == 0 && L1 == 0 && M == 0 && R1 == 0 && R2 == 0)  //丢线的原理就是所有灯熄灭
	      { 	  
					Noline_Flag = 1;
					Clear_Location();					//当初次识别丢线之后开始计算路程
	      }  								
    }
	if(Noline_Flag==1)	
	{
			if(L2==0&&L1==0&&M==0&&R1==0&&R2==0)
			{
				if(Location>19)							//当累计路程大于19，正式进入丢线，否则算作误判，不进入丢线
				{
				Element_Flag=2;
				Place_Enable=0;							
				Place_Out= -100;						//首先大幅度转向到你想要的方向
				Noline_Flag=2;	
				Clear_Location();						//清零编码器，通过距离来判断转了多少，（用陀螺仪也可以）
				}		
			}
			else
				{
					Clear_Location();	
					Noline_Flag=0;
				}	
	}
	if(Noline_Flag==2&&Location>25)		//转到想要的方向后，走直线加速冲过去
	{
		Noline_Flag=3;
		PWM_Enable=0;
		Motor_SetPWM_L(6000);						//设置固定的占空比
		Motor_SetPWM_R(6000);  	
	}	
	if(Noline_Flag==3&&Location>=50&&(L2==1||L1==1||M==1||R1==1||R2==1))//识别到线之后，恢复正常寻线
		{
			
			Noline_Flag=4;
			Element_Flag=0;
			Clear_Location();	
		}
}
//停车  
void Element_Stop()
{
		if (Ten_Flag==13&&Stop_Flag ==0) 					//我的停车是在最后一个元素圆环后，这样不会和断路误判
		{
			if (L2 == 1 && L1 == 1 && M == 1 && R1 == 1 && R2 == 1)		//仍然是全灭，这里可能和丢线重叠，需要做好标志位的限制
			{   
			    Element_Flag=3;
				Stop_Flag = 1;  
			  	Clear_Location();						
			}
		}
		if (Stop_Flag == 1&&Location>30) 
		{
			Place_Enable=0;
			Basic_Speed=Speed_Choice[1];
			
		}
		
}





void Element_MoreRing()
{
 if(Element_Flag==1&&MoreRing_Flag==0&&Ten_Flag==8)
	{ 
		if (L2 == 1&& L1 == 1&& M == 1&& R1 == 1)
		{
			MoreRing_Flag=1;
			Element_Flag=2;
			Place_Enable=0;
			Place_Out=-35;
			Clear_Location();	
		}
	}
 if(MoreRing_Flag==1&&Location>3)
	{
		Place_Enable=1;
		MoreRing_Flag=2;
	}	
 if ( MoreRing_Flag==2 && L1 == 1&& M == 1&& R1 == 1&&R2 == 1 )
		{
			MoreRing_Flag=3;
			Place_Enable=0;
			Place_Out=35;
			Clear_Location();	
		}
	
 if(MoreRing_Flag==3&&Location>3)
	{
		Place_Enable=1;
		MoreRing_Flag=4;
	}	
if ( MoreRing_Flag==4&&L2 == 1&& L1 == 1&& M == 1&& R1 == 1 )
		{
			MoreRing_Flag=5;
			Place_Enable=0;
			Place_Out=-35;
			Clear_Location();	
		}
	
 if(MoreRing_Flag==5&&Location>5)
	{
		Place_Enable=1;
		MoreRing_Flag=6;
	}	
if ( MoreRing_Flag==6&& R1 ==1&& R2 ==1 )
		{
			MoreRing_Flag=7;
			Place_Enable=0;
			Place_Out=-80;
			Clear_Location();	
		}
	
 if(MoreRing_Flag==7&&Location>5&&M == 1)
	{
		Place_Enable=1;
		MoreRing_Flag=8;
	}	
}

void Element_Turnback()  //掉头
{
	if (Turnback_Flag==0&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1)) //第一个十字
	{
		Element_Flag=1;
		Turnback_Flag=1;
		Clear_Location();			
	}
	if(Turnback_Flag==1&&Location>=50)	//经过十字防误触
	{Turnback_Flag=2;}
	
	
	
	if (Turnback_Flag==2&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1)) //第二个十字
	{
		Clear_Location();	
		Turnback_Flag=3;
	}
	if(Turnback_Flag==3&&Location>=50) //经过十字防误触
	{Turnback_Flag=4;}	
	
	
if (Turnback_Flag==4&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1)) //第三个十字
	{
		Clear_Location();	
		Turnback_Flag=5;
	}
	if(Turnback_Flag==5&&Location>=50) //经过十字防误触
	{Turnback_Flag=6;}		
	
	
	if (Turnback_Flag==6&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1)) //终点线识别
	{		
		Place_Enable=0;
		Basic_Speed=Speed_Choice[1];	//停车
		Turnback_Flag=7;
	}
	if(Turnback_Flag==7)
	{
		Turnback_time++;      //停车计时器
		Clear_Location();
	}
	if (Turnback_Flag==7&&Turnback_time>=200)  //停车2s
	{ 				
		Turnback_Flag=8;
	}
	if (Turnback_Flag==8)
	{			
		Place_Enable=0;	
		PWM_Enable=0;	
		Turnback_time=0;
		Turnback_Flag=9;		
	}
	if (Turnback_Flag==9)    //开始掉头
	{
		Place_Enable=0;	
		Motor_SetPWM_L(2000);
		Motor_SetPWM_R(-2000);	
		Turnback_time++;
	}
	if (Turnback_Flag==9&& M == 1&&Turnback_time>50)  //掉头防误触机制，重新巡线
	{
		Turnback_time=0;
		Place_Enable=1;
		PWM_Enable=1;	
		Turnback_Flag=10;
		Basic_Speed=Speed_Choice[0];  //恢复正常速度
	}	
	
	if (Turnback_Flag==10&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1))  //跷跷板前识别
	{
		Turnback_Flag=11;
		Clear_Location();			
	}
	if(Turnback_Flag==11&&Location>=150)  //跷跷板停车  自己调试location来达到平衡点
	{
		Basic_Speed=Speed_Choice[1];			
		Turnback_time++;	
	}

	if(Turnback_Flag==11&&Turnback_time>=500)   //停车5s
	{
		Turnback_Flag=12;
		Basic_Speed=Speed_Choice[0];	
	}
			
	if (Turnback_Flag==12&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1))   
	{
		Turnback_Flag=13;
		Clear_Location();			
	}
	if(Turnback_Flag==13&Location>=30)
	{Turnback_Flag=14;}	
	
	
	if (Turnback_Flag==14&&(L2 == 1&& L1 == 1&& M == 1&& R1 == 1))  //终点线停车
	{
		Turnback_Flag=15;
		Clear_Location();			
	}
	if(Turnback_Flag==15&Location>=10)  //车身压住终点线 距离自定
	{
		Turnback_Flag=16; 
		Basic_Speed=Speed_Choice[1];	
	}	
		
}









