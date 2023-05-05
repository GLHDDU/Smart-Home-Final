#include "Vioce_TASK.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>     //������Ҫ��ͷ�ļ�
#include "stm32f10x.h"
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "uart5.h"
#include "step_motor.h"
#include "uart4.h"
extern u8 USART5_RX_data[USART5_MAX_RECV_LEN]; 				//���ջ���,���USART5_MAX_RECV_LEN���ֽ�.

extern User_Data_t User_Data;

void Voice_request(User_Data_t *User_Data)
	 
 {

	// printf("������������");
	 if(USART5_RX_data[3]==0x01)  //�����еƹ�
	{
		
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�����еƹ�");
		
		TIM_SetCompare2(TIM2, 400);	
		
		TIM_SetCompare3(TIM2, 400);
		
		TIM_SetCompare4(TIM2, 400);	
		
		TIM_SetCompare4(TIM4,400);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_10);
		
		User_Data->ledconf1=!User_Data->ledconf1;
		
		User_Data->ledconf2=!User_Data->ledconf2;
		
		User_Data->ledconf3=!User_Data->ledconf3;
		
		User_Data->ledconf4=!User_Data->ledconf4;
		
		User_Data->ledconf1_2=!User_Data->ledconf1_2;
		
		User_Data->ledconf2_2=!User_Data->ledconf2_2;
		
		User_Data->ledconf3_2=!User_Data->ledconf3_2;
		
		User_Data->ledconf4_2=!User_Data->ledconf4_2;
		USART5_RX_data[3]=0x00;
	}
	
	  if(USART5_RX_data[3]==0x02)  //�ر����еƹ�
	{
		
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ر����еƹ�");
		
		TIM_SetCompare2(TIM2, 0);	
		
		TIM_SetCompare3(TIM2, 0);
		
		TIM_SetCompare4(TIM2, 0);	
		
		TIM_SetCompare4(TIM4, 0) ;		

	  GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_8);
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_10);		
		
	  User_Data->ledconf1=!User_Data->ledconf1;
		
		User_Data->ledconf2=!User_Data->ledconf2;
		
		User_Data->ledconf3=!User_Data->ledconf3;
		
		User_Data->ledconf4=!User_Data->ledconf4;
		
		User_Data->ledconf1_2=!User_Data->ledconf1_2;
		
		User_Data->ledconf2_2=!User_Data->ledconf2_2;
		
		User_Data->ledconf3_2=!User_Data->ledconf3_2;
		
		User_Data->ledconf4_2=!User_Data->ledconf4_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	 if(USART5_RX_data[3]==0x03)  //����һ��|��һ��
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("����һ��|��һ��");
		
		TIM_SetCompare1(TIM2, 550);	
		
		TIM_SetCompare2(TIM2, 550);	
		
		TIM_SetCompare3(TIM2, 550);
		
		TIM_SetCompare4(TIM2, 550);
		
		USART5_RX_data[3]=0x00;
	}
	
	 if(USART5_RX_data[3]==0x04)  //����һ��|��һ��
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("����һ��|��һ��");

		TIM_SetCompare1(TIM2, 100);	
		
		TIM_SetCompare2(TIM2, 100);	
		
		TIM_SetCompare3(TIM2, 100);
		
		TIM_SetCompare4(TIM2, 100);	
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x05)  //�򿪿����ƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�򿪿����ƹ�");

		TIM_SetCompare4(TIM2, 400) ;	
		
		User_Data->ledconf3=!User_Data->ledconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x06)  //�رտ����ƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�رտ����ƹ�");

		TIM_SetCompare4(TIM2, 0) ;
		
		User_Data->ledconf3=!User_Data->ledconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x07)  //�򿪲����ƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�򿪲����ƹ�");

		TIM_SetCompare4(TIM4, 400);	
		
		GPIO_SetBits(GPIOG,GPIO_Pin_10);
		
		User_Data->ledconf4=!User_Data->ledconf4;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x08)  //�رղ����ƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�رղ����ƹ�");

		TIM_SetCompare4(TIM4, 0);	
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_10);
		
		User_Data->ledconf4=!User_Data->ledconf4;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x09)  //�����Եƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�����Եƹ�");

		TIM_SetCompare2(TIM2, 400) ;	
		
		User_Data->ledconf1=!User_Data->ledconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x10)  //�ر����Եƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ر����Եƹ�");

		TIM_SetCompare2(TIM2, 0) ;
		
		User_Data->ledconf1=!User_Data->ledconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
  if(USART5_RX_data[3]==0x11)  //�򿪴��Եƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�򿪴��Եƹ�");

		TIM_SetCompare3(TIM2, 400) ;

    User_Data->ledconf2=!User_Data->ledconf2;    		
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x12)  //�رմ��Եƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�رմ��Եƹ�");

		TIM_SetCompare3(TIM2, 0) ;

    User_Data->ledconf2=!User_Data->ledconf2;    		
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x13)  //����̨�ƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("����̨�ƹ�");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x14)  //�ر���̨�ƹ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ر���̨�ƹ�");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x15)  //�����пյ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�����пյ�");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
		
	}
	
	if(USART5_RX_data[3]==0x16)  //�ر����пյ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ر����пյ�");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x17)  //�ȷ�|�����ȷ�|�����ȷ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ȷ�|�����ȷ�|�����ȷ�");
		
		User_Data->Vehconf1_pattern=1;
		
		User_Data->Vehconf2_pattern=1;
		
		User_Data->Vehconf3_pattern=1;
		
		User_Data->Vehconf4_pattern=1;

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x18)  //���|�������|�������
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("���|�������|�������");
		
		User_Data->Vehconf1_pattern=2;
		
		User_Data->Vehconf2_pattern=2;
		
		User_Data->Vehconf3_pattern=2;
		
		User_Data->Vehconf4_pattern=2;

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x19)  //ͨ��|����ͨ��|����ͨ��
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("ͨ��|����ͨ��|����ͨ��");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x20)  //�򿪿����յ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�򿪿����յ�");

		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x21)  //�رտ����յ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�رտ����յ�");

		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x22)  //�����Կյ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�����Կյ�");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x23)  //�ر����Կյ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ر����Կյ�");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x24)  //�򿪴��Կյ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�򿪴��Կյ�");

		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x25)  //�رմ��Կյ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�رմ��Կյ�");

		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x26)  //�ͷ�|�����ͷ�|�����ͷ�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�ͷ�|�����ͷ�|�����ͷ�");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x27)  //�з�|�����з�|�����з�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�з�|�����з�|�����з�");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x28)  //�߷�|�����߷�|�����߷�
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�߷�|�����߷�|�����߷�");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x29)  //�򿪷���һ��
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("�򿪷���һ��");

		TIM_SetCompare1(TIM2,1000) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x30) 
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]); //�򿪷��ȶ���
		
		printf("�򿪷��ȶ���");

		TIM_SetCompare1(TIM2,700) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x31)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]); //�򿪷�������
		
		printf("�򿪷�������");

		TIM_SetCompare1(TIM2,899) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x32) 
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//��������
		
		printf("��������");

		Stepturn(360,0);
		
		User_Data->curtain=!User_Data->curtain;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x33)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//���ϴ���
		
		printf("���ϴ���");

	  Stepturn(360,1);
		
		User_Data->curtain=!User_Data->curtain;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x34)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�����������ƹ�
		
		printf("�����������ƹ�");

	  GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
    User_Data->ledconf1_2=!User_Data->ledconf1_2;
		
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x35)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�ر����������ƹ�
		
		printf("�ر����������ƹ�");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		
    User_Data->ledconf1_2=!User_Data->ledconf1_2;
		
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x36)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�򿪴��������ƹ�
		
		printf("�򿪴��������ƹ�");

	  GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
    User_Data->ledconf2_2=!User_Data->ledconf2_2;
		
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x37)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�رմ��������ƹ�
		
		printf("�رմ��������ƹ�");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_8);
		
    User_Data->ledconf2_2=!User_Data->ledconf2_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	 if(USART5_RX_data[3]==0x38)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�򿪿��������ƹ�
		
		printf("�򿪿��������ƹ�");

	  GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
    User_Data->ledconf3_2=!User_Data->ledconf3_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x39)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�رտ��������ƹ�
		
		printf("�رտ��������ƹ�");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		
    User_Data->ledconf3_2=!User_Data->ledconf3_2;
		
		USART5_RX_data[3]=0x00;
	}
	
  if(USART5_RX_data[3]==0x40)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�򿪲��������ƹ�
		
		printf("�򿪲��������ƹ�");

	  GPIO_SetBits(GPIOG,GPIO_Pin_10);
		
    User_Data->ledconf4_2=!User_Data->ledconf4_2;
		
		USART5_RX_data[3]=0x00;
	}
	
  if(USART5_RX_data[3]==0x41)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//�رղ��������ƹ�
		
		printf("�رղ��������ƹ�");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_10);
		
    User_Data->ledconf4_2=!User_Data->ledconf4_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x43)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]); //�򿪷�������
		
		printf("�رշ���");

		TIM_SetCompare1(TIM2,0) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x44)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//����
		
		printf("����");

	  UART4_SendStr("MG995OFF*");
		
		USART5_RX_data[3]=0x00;
	}
 }


void Vioce_Task(User_Data_t *User_Data)
{
	
	UART5_Configuration();//����������ʼ��
	
  while (1)
	{
		
	 Voice_request(User_Data);
	
	 vTaskDelay(500);
		
	}


}



