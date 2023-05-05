#include "stm32f10x.h"
#include "step_motor.h"
#include "stm32f10x_gpio.h"

void Delay(u32 count)
{
	u32 i = 0;
	for(i = 0; i < count; i++)
		;
};

void InitSTEP()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

//function: give a pulse to have stepper 1 Sita£¬and delay sometime
//clockwise/anticlockwise: clockwise/anticlockwise
//parameter: Step
void cw_pulse(u16 Step)
{
	switch(Step)
	{
		case 0:	GPIO_SetBits(GPIOG, GPIO_Pin_2);//1     0      0      0
		GPIO_ResetBits(GPIOG, GPIO_Pin_3);
		GPIO_ResetBits(GPIOG, GPIO_Pin_4);
		GPIO_ResetBits(GPIOG, GPIO_Pin_5);
				break;
		case 1:	GPIO_SetBits(GPIOG, GPIO_Pin_2);//1     1      0      0
		GPIO_SetBits(GPIOG, GPIO_Pin_3);
		GPIO_ResetBits(GPIOG, GPIO_Pin_4);
		GPIO_ResetBits(GPIOG, GPIO_Pin_5);
				break;
		case 2:	GPIO_ResetBits(GPIOG, GPIO_Pin_2);//0     1      0      0
		GPIO_SetBits(GPIOG, GPIO_Pin_3);
		GPIO_ResetBits(GPIOG, GPIO_Pin_4);
		GPIO_ResetBits(GPIOG, GPIO_Pin_5);
				break;
		case 3:	GPIO_ResetBits(GPIOG, GPIO_Pin_2);//0     1      1      0
		GPIO_SetBits(GPIOG, GPIO_Pin_3);
		GPIO_SetBits(GPIOG, GPIO_Pin_4);
		GPIO_ResetBits(GPIOG, GPIO_Pin_5);
				break;
		case 4:	GPIO_ResetBits(GPIOG, GPIO_Pin_2);//0     0      1      0
		GPIO_ResetBits(GPIOG, GPIO_Pin_3);
		GPIO_SetBits(GPIOG, GPIO_Pin_4);
		GPIO_ResetBits(GPIOG, GPIO_Pin_5);
				break;
		case 5:	GPIO_ResetBits(GPIOG, GPIO_Pin_2);//0     0      1      1
		GPIO_ResetBits(GPIOG, GPIO_Pin_3);
		GPIO_SetBits(GPIOG, GPIO_Pin_4);
		GPIO_SetBits(GPIOG, GPIO_Pin_5);
				break;
		case 6:	GPIO_ResetBits(GPIOG, GPIO_Pin_2);//0     0      0      1
		GPIO_ResetBits(GPIOG, GPIO_Pin_3);
		GPIO_ResetBits(GPIOG, GPIO_Pin_4);
		GPIO_SetBits(GPIOG, GPIO_Pin_5);
				break;
		case 7:	GPIO_SetBits(GPIOG, GPIO_Pin_2);//1     0      0      1
		GPIO_ResetBits(GPIOG, GPIO_Pin_3);
		GPIO_ResetBits(GPIOG, GPIO_Pin_4);
		GPIO_SetBits(GPIOG, GPIO_Pin_5);
				break;
	}
	Delay(20000);
}
//function: stepper run a special pulse
//clockwise/anticlockwise: clockwise
//parameter: nPulse-- stepper run n steps
void Stepturn(u16 angle,u16 direct)
{
	u16 step=0;
	u16 i;
	u32 nPulse=angle*11.38;
	for(i=0;i<nPulse & direct==0;i++)
	{
		cw_pulse(step);
		TurnOff();
		
		step++;
		if(8==step)
		{
			step=0;
		}
	}	
	for(i=0;i<nPulse & direct==1;i++)//anticlockwise
	{
		cw_pulse(7-step);
		TurnOff();
		
		step++;
		if(8==step)
		{
			step=0;
		}
	}	
}
void TurnOff(void)
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_2);
	GPIO_ResetBits(GPIOG, GPIO_Pin_3);
	GPIO_ResetBits(GPIOG, GPIO_Pin_4);
	GPIO_ResetBits(GPIOG, GPIO_Pin_5);
	Delay(10000);
}
