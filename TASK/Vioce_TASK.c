#include "Vioce_TASK.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>     //包含需要的头文件
#include "stm32f10x.h"
#include "delay.h"      //包含需要的头文件
#include "uart5.h"
#include "step_motor.h"
#include "uart4.h"
extern u8 USART5_RX_data[USART5_MAX_RECV_LEN]; 				//接收缓冲,最大USART5_MAX_RECV_LEN个字节.

extern User_Data_t User_Data;

void Voice_request(User_Data_t *User_Data)
	 
 {

	// printf("进入语音测试");
	 if(USART5_RX_data[3]==0x01)  //打开所有灯光
	{
		
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开所有灯光");
		
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
	
	  if(USART5_RX_data[3]==0x02)  //关闭所有灯光
	{
		
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭所有灯光");
		
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
	
	 if(USART5_RX_data[3]==0x03)  //调亮一点|亮一点
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("调亮一点|亮一点");
		
		TIM_SetCompare1(TIM2, 550);	
		
		TIM_SetCompare2(TIM2, 550);	
		
		TIM_SetCompare3(TIM2, 550);
		
		TIM_SetCompare4(TIM2, 550);
		
		USART5_RX_data[3]=0x00;
	}
	
	 if(USART5_RX_data[3]==0x04)  //调暗一点|暗一点
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("调暗一点|暗一点");

		TIM_SetCompare1(TIM2, 100);	
		
		TIM_SetCompare2(TIM2, 100);	
		
		TIM_SetCompare3(TIM2, 100);
		
		TIM_SetCompare4(TIM2, 100);	
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x05)  //打开客厅灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开客厅灯光");

		TIM_SetCompare4(TIM2, 400) ;	
		
		User_Data->ledconf3=!User_Data->ledconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x06)  //关闭客厅灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭客厅灯光");

		TIM_SetCompare4(TIM2, 0) ;
		
		User_Data->ledconf3=!User_Data->ledconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x07)  //打开餐厅灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开餐厅灯光");

		TIM_SetCompare4(TIM4, 400);	
		
		GPIO_SetBits(GPIOG,GPIO_Pin_10);
		
		User_Data->ledconf4=!User_Data->ledconf4;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x08)  //关闭餐厅灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭餐厅灯光");

		TIM_SetCompare4(TIM4, 0);	
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_10);
		
		User_Data->ledconf4=!User_Data->ledconf4;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x09)  //打开主卧灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开主卧灯光");

		TIM_SetCompare2(TIM2, 400) ;	
		
		User_Data->ledconf1=!User_Data->ledconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x10)  //关闭主卧灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭主卧灯光");

		TIM_SetCompare2(TIM2, 0) ;
		
		User_Data->ledconf1=!User_Data->ledconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
  if(USART5_RX_data[3]==0x11)  //打开次卧灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开次卧灯光");

		TIM_SetCompare3(TIM2, 400) ;

    User_Data->ledconf2=!User_Data->ledconf2;    		
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x12)  //关闭次卧灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭次卧灯光");

		TIM_SetCompare3(TIM2, 0) ;

    User_Data->ledconf2=!User_Data->ledconf2;    		
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x13)  //打开阳台灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开阳台灯光");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x14)  //关闭阳台灯光
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭阳台灯光");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x15)  //打开所有空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开所有空调");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
		
	}
	
	if(USART5_RX_data[3]==0x16)  //关闭所有空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭所有空调");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x17)  //热风|调至热风|调到热风
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("热风|调至热风|调到热风");
		
		User_Data->Vehconf1_pattern=1;
		
		User_Data->Vehconf2_pattern=1;
		
		User_Data->Vehconf3_pattern=1;
		
		User_Data->Vehconf4_pattern=1;

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x18)  //冷风|调至冷风|调到冷风
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("冷风|调至冷风|调到冷风");
		
		User_Data->Vehconf1_pattern=2;
		
		User_Data->Vehconf2_pattern=2;
		
		User_Data->Vehconf3_pattern=2;
		
		User_Data->Vehconf4_pattern=2;

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x19)  //通风|调至通风|调到通风
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("通风|调至通风|调到通风");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x20)  //打开客厅空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开客厅空调");

		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x21)  //关闭客厅空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭客厅空调");

		User_Data->Vehconf3=!User_Data->Vehconf3;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x22)  //打开主卧空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开主卧空调");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x23)  //关闭主卧空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭主卧空调");

		User_Data->Vehconf1=!User_Data->Vehconf1;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x24)  //打开次卧空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开次卧空调");

		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x25)  //关闭次卧空调
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("关闭次卧空调");

		User_Data->Vehconf2=!User_Data->Vehconf2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x26)  //低风|调至低风|调到低风
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("低风|调至低风|调到低风");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x27)  //中风|调至中风|调到中风
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("中风|调至中风|调到中风");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x28)  //高风|调至高风|调到高风
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("高风|调至高风|调到高风");

		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x29)  //打开风扇一档
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);
		
		printf("打开风扇一档");

		TIM_SetCompare1(TIM2,1000) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x30) 
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]); //打开风扇二档
		
		printf("打开风扇二档");

		TIM_SetCompare1(TIM2,700) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x31)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]); //打开风扇三档
		
		printf("打开风扇三档");

		TIM_SetCompare1(TIM2,899) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x32) 
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//拉开窗帘
		
		printf("拉开窗帘");

		Stepturn(360,0);
		
		User_Data->curtain=!User_Data->curtain;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x33)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//关上窗帘
		
		printf("关上窗帘");

	  Stepturn(360,1);
		
		User_Data->curtain=!User_Data->curtain;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x34)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//打开主卧其他灯光
		
		printf("打开主卧其他灯光");

	  GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
    User_Data->ledconf1_2=!User_Data->ledconf1_2;
		
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x35)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//关闭主卧其他灯光
		
		printf("关闭主卧其他灯光");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		
    User_Data->ledconf1_2=!User_Data->ledconf1_2;
		
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x36)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//打开次卧其他灯光
		
		printf("打开次卧其他灯光");

	  GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
    User_Data->ledconf2_2=!User_Data->ledconf2_2;
		
		USART5_RX_data[3]=0x00;
	}
	
		if(USART5_RX_data[3]==0x37)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//关闭次卧其他灯光
		
		printf("关闭次卧其他灯光");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_8);
		
    User_Data->ledconf2_2=!User_Data->ledconf2_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	 if(USART5_RX_data[3]==0x38)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//打开客厅其他灯光
		
		printf("打开客厅其他灯光");

	  GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
    User_Data->ledconf3_2=!User_Data->ledconf3_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x39)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//关闭客厅其他灯光
		
		printf("关闭客厅其他灯光");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		
    User_Data->ledconf3_2=!User_Data->ledconf3_2;
		
		USART5_RX_data[3]=0x00;
	}
	
  if(USART5_RX_data[3]==0x40)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//打开餐厅其他灯光
		
		printf("打开餐厅其他灯光");

	  GPIO_SetBits(GPIOG,GPIO_Pin_10);
		
    User_Data->ledconf4_2=!User_Data->ledconf4_2;
		
		USART5_RX_data[3]=0x00;
	}
	
  if(USART5_RX_data[3]==0x41)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//关闭餐厅其他灯光
		
		printf("关闭餐厅其他灯光");

	  GPIO_ResetBits(GPIOG,GPIO_Pin_10);
		
    User_Data->ledconf4_2=!User_Data->ledconf4_2;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x43)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]); //打开风扇三档
		
		printf("关闭风扇");

		TIM_SetCompare1(TIM2,0) ;	
		
		User_Data->fan=!User_Data->fan;
		
		USART5_RX_data[3]=0x00;
	}
	
	if(USART5_RX_data[3]==0x44)  
	{
		printf("USART5_RX_data=%x\n\r",USART5_RX_data[3]);//关门
		
		printf("关门");

	  UART4_SendStr("MG995OFF*");
		
		USART5_RX_data[3]=0x00;
	}
 }


void Vioce_Task(User_Data_t *User_Data)
{
	
	UART5_Configuration();//语音播报初始化
	
  while (1)
	{
		
	 Voice_request(User_Data);
	
	 vTaskDelay(500);
		
	}


}



