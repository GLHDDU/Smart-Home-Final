/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�Դ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  			
 GPIO_ResetBits(GPIOG,GPIO_Pin_7); 					
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				
 GPIO_ResetBits(GPIOG,GPIO_Pin_8); 					
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		
 GPIO_Init(GPIOG, &GPIO_InitStructure);	 
 GPIO_ResetBits(GPIOG,GPIO_Pin_9); 		
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	    	
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 
 GPIO_ResetBits(GPIOG,GPIO_Pin_10); 		
}
 

/*-------------------------------------------------*/
/*����������������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
  LED0_ON;
	LED1_ON;
}

/*-------------------------------------------------*/
/*��������Ϩ������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
  LED0_OFF;
	LED1_OFF;
}
 


