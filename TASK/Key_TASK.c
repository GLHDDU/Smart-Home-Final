#include "44key.h"
#include "Key_TASK.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
void Key_TASK(void)
{
  Keyboard_GPIO_Config();
   while(1)
  {
	 int key_value;
	 key_value=keyboard_scan();
   if(key_value==1)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==2)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==3)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==4)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==5)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==6)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==7)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==8)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==9)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==10)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==11)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 if(key_value==12)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	  if(key_value==13)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	  if(key_value==14)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	  if(key_value==15)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	  if(key_value==16)
	 {
     printf("������ֵ��%d\r\n",key_value);
	 }
	 vTaskDelay(500);
 }
	  
}



