#ifndef __PID_H
#define __PID_H

extern int8_t SetDuty;
extern uint16_t SetSpeed[];
extern float Err;
extern float pwm_CCR;
extern int8_t Grade;

float PID(int16_t speed,int8_t xGrade);


#endif
