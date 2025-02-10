#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "Timer4.h"
#include "PID.h"
#include "Serial.h"
#include "Serial2.h"

uint8_t KeyNum;

int16_t Speed=0;

int16_t Temp = 0;
uint16_t Count = 0;
int16_t Duty_PID = 0;
                            
float TIM2_CCR = 0;

uint8_t RxData = 0;


int main(void)
{
	OLED_Init();
	Motor_Init();
	Encoder_Init();
	Timer4_Init();
	Serial_Init();
	Serial2_Init();
	
	OLED_ShowString(1, 1, "Grade:");
	OLED_ShowString(2, 1, "Duty:");
	OLED_ShowString(2, 10, "%");
	OLED_ShowString(3, 1, "Speed:");
	OLED_ShowString(4, 1, "Target:");
	
	while (1)
	{	
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
		}

		Grade = RxData;
		
		if (Grade > 5)
		{
			OLED_ShowSignedNum(1, 7, -(Grade-5), 1);
		}
		else
		{
			OLED_ShowSignedNum(1, 7, Grade, 1);
		}
		
		OLED_ShowSignedNum(2, 6, Duty_PID, 3);
		OLED_ShowSignedNum(3, 7, Speed, 5);
		
		if (Grade > 5)
		{
			OLED_ShowSignedNum(4, 8, -SetSpeed[Grade-5], 5);
		}
		else
		{
			OLED_ShowSignedNum(4, 8, SetSpeed[Grade], 5);
		}
	}
}

//速度和档位的对应关系：
//1：10  2：20  3：30  4：40  5：50

void TIM4_IRQHandler(void)                  //每20ms进一次中断执行一次PID调整占空比（即调整速度），每100ms读取一次速度
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)     
	{
		Temp = -Encoder_Get();
		
		Serial2_Printf("%d\n", Temp);
		
		Count++;
		
		Duty_PID = PID(Temp,Grade);	
		
		if (Duty_PID > 100)
		{
			Duty_PID = 100;
		}
		if (Duty_PID < -100)
		{
			Duty_PID = -100;
		}

		Motor_SetSpeed(Duty_PID);
		
		if (Count >= 5)
		{
			Speed = Temp;                  //这个速度实际上是 脉冲数/单位时间
			Count = 0;
		}

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
