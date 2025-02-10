#include "stm32f10x.h"                  // Device header

float Err=0,LastErr=0,LastLastErr=0;
float pwm_CCR=0,Add_CCR=0;                                //pwm�µ�PWM�Ƚ�ֵ��add���µ�PWMռ�ձȸ���ֵ
float p=1.5,i=0.1,d=0.0;     //����ϵ��������ϵ����΢��ϵ��  

int8_t SetDuty=0;
                            //�趨Ŀ���ٶ�
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
		Err=SetSpeed[xGrade]-Speed;    	 //�趨�ٶ�-ʵʱ�ٶ�
	}
    Add_CCR = p*(Err-LastErr)+i*(Err);   //PWMռ�ձ����ӵĲ���=��������+���ֻ���                         
	pwm_CCR+=Add_CCR;                                                      
	 
    LastErr=Err;							 //��������ֵ���ϴ����
    return pwm_CCR;								 //����PWM�µ�ռ�ձ�ֵ
}

