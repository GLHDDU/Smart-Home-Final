/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的头文件                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#define LED0_OUT(x)   GPIO_WriteBit(GPIOE, GPIO_Pin_5,  (BitAction)x)  //设置PE5  的电平，可以点亮熄灭LED0
#define LED1_OUT(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_5,  (BitAction)x)  //设置PB5  的电平，可以点亮熄灭LED1


#define LED0_IN_STA   GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5)  //PA5  控制LED0,读取电平状态，可以判断LED0是点亮还是熄灭
#define LED1_IN_STA   GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5)  //PB1  控制LED1,读取电平状态，可以判断LED1是点亮还是熄灭


#define LED0_ON       GPIO_ResetBits(GPIOE, GPIO_Pin_5)         //共阳极，拉低PE5电平，点亮LED0
#define LED0_OFF      GPIO_SetBits(GPIOE, GPIO_Pin_5)           //共阳极，拉高PE5电平，熄灭LED0

#define LED1_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_5)         //共阳极，拉低PB5电平，点亮LED1
#define LED1_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_5)           //共阳极，拉高PB5电平，熄灭LED1

void LED_Init(void);               //初始化	
void LED_AllOn(void);              //点亮所有LED
void LED_AllOff(void);             //熄灭所有LED

#endif
