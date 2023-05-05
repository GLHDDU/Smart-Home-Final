#include "led.h"
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x.h"  
#include "usart1.h"     
#include "usart2.h"     
#include "timer3.h"    
#include "timer4.h"    
#include "wifi.h"	  
#include "led.h"        
#include "mqtt.h"     
#include "dht11.h"
#include <stdlib.h>    
#include "MQTT_TASK.h"
#include "Vioce_TASK.h"
#include "Lcd_TASK.h"
#include "lcd.h"
#include "Actuator_TASK.h"
#include "time8.h"
#include "step_motor.h"
	extern User_Data_t User_Data;
//�������ȼ�
#define START_TASK_PRIO		5
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);//������

//�������ȼ�
#define MQTT_TASK_PRIO		3
//�����ջ��С	
#define MQTT_TASK_SIZE 		512  
//������
TaskHandle_t MQTTTASK_Handler;

//�������ȼ�
#define Lcd_TASK_PRIO		  2
//�����ջ��С	
#define Lcd_TASK_SIZE 		512  
//������
TaskHandle_t LcdTASK_Handler;

//�������ȼ�
#define Actuator_TASK_PRIO		  4
//�����ջ��С	
#define Actuator_TASK_SIZE 		512  
//������
TaskHandle_t ActuatorTASK_Handler;

//�������ȼ�
#define Vioce_TASK_PRIO		  1
//�����ջ��С	
#define Vioce_TASK_SIZE 		512  
//������
TaskHandle_t VioceTASK_Handler;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 	 
	delay_init();	    				//��ʱ������ʼ��	  
	LED_Init();		  					//��ʼ��LED 
	InitSTEP();//�������
  Usart1_Init(115200);              //����1���ܳ�ʼ����������115200
	Usart2_Init(115200);//WIFI		//WIFIģ�鴮��	
	LCD_Init();
	POINT_COLOR=RED;//��������Ϊ��ɫ
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����wifi����
    xTaskCreate((TaskFunction_t )Mqtt_Task,     	
                (const char*    )"Mqtt_Task",   	
                (uint16_t       )MQTT_TASK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MQTT_TASK_PRIO,	
                (TaskHandle_t*  )&MQTTTASK_Handler);   
		//����lcd�����������
    xTaskCreate((TaskFunction_t )Lcd_TASK,     
                (const char*    )"Lcd_TASK",   
                (uint16_t       )Lcd_TASK_SIZE, 
                (void*          )&User_Data,
                (UBaseType_t    )Lcd_TASK_PRIO,
                (TaskHandle_t*  )&LcdTASK_Handler);
    //����ִ��������								
		xTaskCreate((TaskFunction_t )Actuator_TASK,     
                (const char*    )"Actuator_TASK",   
                (uint16_t       )Lcd_TASK_SIZE, 
                (void*          )&User_Data,
                (UBaseType_t    )Lcd_TASK_PRIO,
                (TaskHandle_t*  )&LcdTASK_Handler);
    //������������								
		xTaskCreate((TaskFunction_t )Vioce_Task,     
                (const char*    )"Vioce_Task",   
                (uint16_t       )Vioce_TASK_SIZE, 
                (void*          )&User_Data,
                (UBaseType_t    )Vioce_TASK_PRIO,
                (TaskHandle_t*  )&VioceTASK_Handler); 								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


