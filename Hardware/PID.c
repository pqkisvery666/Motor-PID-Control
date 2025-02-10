#include "stm32f10x.h"                  // Device header

float Err=0,LastErr=0,LastLastErr=0;
float pwm_CCR=0,Add_CCR=0;                                //pwm新的PWM比较值，add，新的PWM占空比更改值
float p=1.5,i=0.1,d=0.0;     //比例系数、积分系数、微分系数  

int8_t SetDuty=0;
                            //设定目标速度
int8_t Grade = 0;

uint16_t SetSpeed[6]={0,30,60,90,120,150};

float PID(int16_t Speed,int8_t xGrade)
{
	if (xGrade > 5)
	{
		Err=-SetSpeed[xGrade-5]-Speed;
	}	
	else
	{
		Err=SetSpeed[xGrade]-Speed;    	 //设定速度-实时速度
	}
    Add_CCR = p*(Err-LastErr)+i*(Err);   //PWM占空比增加的部分=比例环节+积分环节                         
	pwm_CCR+=Add_CCR;                                                      
	 
    LastErr=Err;							 //把最新误差赋值给上次误差
    return pwm_CCR;								 //返回PWM新的占空比值
}

