#include "time7.h"
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�






void TIM7_Config(unsigned short int arr, unsigned short int psc)
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʹ��TIM6ʱ��


/*��������*/
TIM_TimeBaseStructure.TIM_Period = arr;//����ֵ10000   
TIM_TimeBaseStructure.TIM_Prescaler = psc;    //Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //������Ƶ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

TIM_ClearITPendingBit(TIM7,TIM_IT_Update);                    //�������жϱ�־λ
TIM_ITConfig(TIM7,TIM_IT_Update, ENABLE);     //ʹ��TIM6�ж�
TIM_Cmd(TIM7, ENABLE);     //ʹ�ܶ�ʱ��6
}
/**********************************************************
** ������:NVIC_Config
** ��������:  �ж����ȼ�����
** �������: ��
** �������: ��
***********************************************************/
void NVICTIM7_Config(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�������2 
 
NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;//TIM6�ж�
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
}

