/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�ͷ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#define LED0_OUT(x)   GPIO_WriteBit(GPIOE, GPIO_Pin_5,  (BitAction)x)  //����PE5  �ĵ�ƽ�����Ե���Ϩ��LED0
#define LED1_OUT(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_5,  (BitAction)x)  //����PB5  �ĵ�ƽ�����Ե���Ϩ��LED1


#define LED0_IN_STA   GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5)  //PA5  ����LED0,��ȡ��ƽ״̬�������ж�LED0�ǵ�������Ϩ��
#define LED1_IN_STA   GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)  //PB1  ����LED1,��ȡ��ƽ״̬�������ж�LED1�ǵ�������Ϩ��


#define LED0_ON       GPIO_ResetBits(GPIOE, GPIO_Pin_5)         //������������PE5��ƽ������LED0
#define LED0_OFF      GPIO_SetBits(GPIOE, GPIO_Pin_5)           //������������PE5��ƽ��Ϩ��LED0

#define LED1_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_5)         //������������PB5��ƽ������LED1
#define LED1_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_5)           //������������PB5��ƽ��Ϩ��LED1

void LED_Init(void);               //��ʼ��	
void LED_AllOn(void);              //��������LED
void LED_AllOff(void);             //Ϩ������LED

#endif
