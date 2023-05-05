#ifndef		_STEP_MOTOR_H
#define		_STEP_MOTOR_H

#define		STEP1	0
#define		STEP2	1
#define		STEP3	2
#define		STEP4	3

void InitSTEP(void);
void cw_pulse(u16 Step);//clockwisely/anticlockwise 1 pulse

void TurnOff(void);
void Delay(u32 count);
void Stepturn(u16 angle,u16 direct);

#endif
