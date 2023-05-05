/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的源文件                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
	
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
/*函数名：点亮所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
  LED0_ON;
	LED1_ON;
}

/*-------------------------------------------------*/
/*函数名：熄灭所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
  LED0_OFF;
	LED1_OFF;
}
 


