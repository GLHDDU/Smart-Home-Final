/**********************************************************
** ������: TIM6_Config
** ��������:  ������ʱ������
** �������: ��
** �������: ��
** ˵������ʱʱ��=(Ԥ��Ƶ��+1��*������ֵ+1) /TIM6ʱ��(72MHz)����λ(s)
   �������ʱ��t=(7200*10000)/72000000s=1s
***********************************************************/
#include "time8.h"
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
void TIM6_Config(void)
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʹ��TIM6ʱ��


/*��������*/
TIM_TimeBaseStructure.TIM_Period = 50000-1;//����ֵ10000   
TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    //Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //������Ƶ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);


TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);     //ʹ��TIM6�ж�
TIM_Cmd(TIM6, ENABLE);     //ʹ�ܶ�ʱ��6
}
/**********************************************************
** ������:NVIC_Config
** ��������:  �ж����ȼ�����
** �������: ��
** �������: ��
***********************************************************/
void NVIC_Config(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�������2 
 
NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;//TIM6�ж�
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
}



