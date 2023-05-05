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
//任务优先级
#define START_TASK_PRIO		5
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);//任务函数

//任务优先级
#define MQTT_TASK_PRIO		3
//任务堆栈大小	
#define MQTT_TASK_SIZE 		512  
//任务句柄
TaskHandle_t MQTTTASK_Handler;

//任务优先级
#define Lcd_TASK_PRIO		  2
//任务堆栈大小	
#define Lcd_TASK_SIZE 		512  
//任务句柄
TaskHandle_t LcdTASK_Handler;

//任务优先级
#define Actuator_TASK_PRIO		  4
//任务堆栈大小	
#define Actuator_TASK_SIZE 		512  
//任务句柄
TaskHandle_t ActuatorTASK_Handler;

//任务优先级
#define Vioce_TASK_PRIO		  1
//任务堆栈大小	
#define Vioce_TASK_SIZE 		512  
//任务句柄
TaskHandle_t VioceTASK_Handler;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 	 
	delay_init();	    				//延时函数初始化	  
	LED_Init();		  					//初始化LED 
	InitSTEP();//步进电机
  Usart1_Init(115200);              //串口1功能初始化，波特率115200
	Usart2_Init(115200);//WIFI		//WIFI模块串口	
	LCD_Init();
	POINT_COLOR=RED;//设置字体为红色
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建wifi任务
    xTaskCreate((TaskFunction_t )Mqtt_Task,     	
                (const char*    )"Mqtt_Task",   	
                (uint16_t       )MQTT_TASK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MQTT_TASK_PRIO,	
                (TaskHandle_t*  )&MQTTTASK_Handler);   
		//创建lcd触摸检测任务
    xTaskCreate((TaskFunction_t )Lcd_TASK,     
                (const char*    )"Lcd_TASK",   
                (uint16_t       )Lcd_TASK_SIZE, 
                (void*          )&User_Data,
                (UBaseType_t    )Lcd_TASK_PRIO,
                (TaskHandle_t*  )&LcdTASK_Handler);
    //创建执行器任务								
		xTaskCreate((TaskFunction_t )Actuator_TASK,     
                (const char*    )"Actuator_TASK",   
                (uint16_t       )Lcd_TASK_SIZE, 
                (void*          )&User_Data,
                (UBaseType_t    )Lcd_TASK_PRIO,
                (TaskHandle_t*  )&LcdTASK_Handler);
    //创建语音任务								
		xTaskCreate((TaskFunction_t )Vioce_Task,     
                (const char*    )"Vioce_Task",   
                (uint16_t       )Vioce_TASK_SIZE, 
                (void*          )&User_Data,
                (UBaseType_t    )Vioce_TASK_PRIO,
                (TaskHandle_t*  )&VioceTASK_Handler); 								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


