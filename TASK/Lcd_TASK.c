#include "lcd.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "touch.h"
#include "led.h"
#include "delay.h"
#include "Lcd_TASK.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hanzi.h"
#include "timer2.h"   
#include "uart4.h"
#include "stdio.h"
#include "string.h"
#include "MQTT_TASK.h"
#include "timer4.h"
u16 led0pwmval=0;

u16  PWMdutycycle=200;

extern u8 tempdata,humidata;

extern User_Data_t User_Data;

void Lcd_TASK(User_Data_t *User_Data)
{
  tp_dev.init();
	
	Draw_mainpage();
	
	TIM4_PWM_Init(899,0);
	
	TIM2_PWM_Init(899,0);
	
	TIM_SetCompare1(TIM2, 0) ;	
	
	TIM_SetCompare2(TIM2, 0) ;	
	
	TIM_SetCompare3(TIM2, 0) ;
	
	TIM_SetCompare4(TIM2, 0) ;	
	
	TIM_SetCompare4(TIM4, 0);	
	
  UART4_Init(115200);	
	
	MY_NVIC_Init(0,0,UART4_IRQn,NVIC_PriorityGroup_4);

  while(1)
{   
		tp_dev.scan(0);	
	
	  lcdshounum(User_Data);
	
	  if(tp_dev.x[0]>40&&tp_dev.x[0]<120&&tp_dev.y[0]>600&&tp_dev.y[0]<750)
	{
		printf("打开所有灯光");
		
		TIM_SetCompare2(TIM2, 400);	
		
		TIM_SetCompare3(TIM2, 400);
		
		TIM_SetCompare4(TIM2, 400);	
		
		TIM_SetCompare4(TIM4,400);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
		GPIO_SetBits(GPIOG,GPIO_Pin_10);
		
	
	}
	
		if(tp_dev.x[0]>120&&tp_dev.x[0]<200&&tp_dev.y[0]>600&&tp_dev.y[0]<750)
	{ 
		delay_ms(500);
		
	
		printf("主卧空调关闭\r\n");
		
		
		User_Data->Vehconf1=!User_Data->Vehconf1;
	}
	
	  if(tp_dev.x[0]>200&&tp_dev.x[0]<270&&tp_dev.y[0]>600&&tp_dev.y[0]<750)
	{
		delay_ms(500);
	
	
		printf("模式切换\r\n");
	}
	
		if(tp_dev.x[0]>270&&tp_dev.x[0]<330&&tp_dev.y[0]>600&&tp_dev.y[0]<750)
	{
		delay_ms(500);
	  
	
		printf("温度调高\r\n");
	}
	
		if(tp_dev.x[0]>330&&tp_dev.x[0]<420&&tp_dev.y[0]>600&&tp_dev.y[0]<750)
	{
		delay_ms(500);
	
	
		printf("温度调低\r\n");
	}
	
	  if(tp_dev.x[0]>420&&tp_dev.x[0]<580&&tp_dev.y[0]>600&&tp_dev.y[0]<750)
	{
	
	  printf("风速控制\r\n");
	}
					
		if(tp_dev.x[0]>40&&tp_dev.x[0]<120&&tp_dev.y[0]>740)
	{

		
		if(led0pwmval>900)led0pwmval=0;
		
		led0pwmval=led0pwmval+100;

		printf("led0pwmval=%d",led0pwmval);	
		
    TIM_SetCompare2(TIM2, led0pwmval) ;	
		
		GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
		printf("主卧灯打开\r\n");
		
		
	}
	
		if(tp_dev.x[0]>120&&tp_dev.x[0]<200&&tp_dev.y[0]>740)
	{

		
		if(led0pwmval>900)led0pwmval=0;
		
		led0pwmval=led0pwmval+100;

		printf("led0pwmval=%d",led0pwmval);	
		
		TIM_SetCompare3(TIM2, led0pwmval) ;	
		
		GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
		printf("次卧灯打开\r\n");
		
		
	}
	
		if(tp_dev.x[0]>200&&tp_dev.x[0]<270&&tp_dev.y[0]>740)
	{

		
		if(led0pwmval>900)led0pwmval=0;
		
		led0pwmval=led0pwmval+100;

		printf("led0pwmval=%d",led0pwmval);	
		
		TIM_SetCompare4(TIM2, led0pwmval) ;	
		
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
		printf("客厅灯打开\r\n");
		
		
	}
	
		if(tp_dev.x[0]>270&&tp_dev.x[0]<330&&tp_dev.y[0]>740)
	{ 

		  GPIO_SetBits(GPIOG,GPIO_Pin_10);

			printf("打开餐厅灯光");
		
	}
	
		if(tp_dev.x[0]>330&&tp_dev.x[0]<420&&tp_dev.y[0]>740)
	{

	  UART4_SendStr("MG995OFF*");
	
		printf("关门");
		
	}
	
		if(tp_dev.x[0]>420&&tp_dev.x[0]<580&&tp_dev.y[0]>740)
	{
		
		printf("灯全关\r\n");
		
		printf("关闭所有灯光");
		
		TIM_SetCompare2(TIM2, 0);	
		
		TIM_SetCompare3(TIM2, 0);
		
		TIM_SetCompare4(TIM2, 0);	
		
		TIM_SetCompare4(TIM4, 0) ;		

	  GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_8);
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		
		GPIO_ResetBits(GPIOG,GPIO_Pin_10);		
		
		
	}
	else 
	{
	delay_ms(100);
	}
		
		
    vTaskDelay(100);
	}
  
}
   

void Draw_mainpage(void)
{
//标题
	Draw_word(42,42,48,48,DARKBLUE,title[0]);
  Draw_word(42+66,42,48,48,DARKBLUE,title[1]);
  Draw_word(42+66*2,42,48,48,DARKBLUE,title[2]);
  Draw_word(42+66*3,42,48,48,DARKBLUE,title[3]);
  Draw_word(42+66*4,42,48,48,DARKBLUE,title[4]);
  Draw_word(42+66*5,42,48,48,DARKBLUE,title[5]);
	//绘制表格
	LCD_Fill(64, 100, 378, 100, BLACK); //绘制线条
	LCD_Fill(64, 100+56, 378, 100+56, BLACK); //绘制线条
	LCD_Fill(64, 100+56*2, 378, 100+56*2, BLACK); //绘制线条
	LCD_Fill(64, 100+56*3, 378, 100+56*3, BLACK); //绘制线条
	LCD_Fill(64, 100+56*4, 378, 100+56*4, BLACK); //绘制线条
	LCD_Fill(64, 100+56*5, 378, 100+56*5, BLACK); //绘制线条
	//绘制表格	
	LCD_Fill(63, 100, 63, 100+56*5, BLACK); //绘制线条
	LCD_Fill(174, 100, 174, 100+56*5, BLACK); //绘制线条
	LCD_Fill(282, 100, 282, 100+56*5, BLACK); //绘制线条
	LCD_Fill(379, 100, 379, 100+56*5, BLACK); //绘制线条
	///今日天气文字
	Draw_word(32,487-39-39,48,48,DARKBLUE,tianqi[0]);
	Draw_word(32+48,487-39-39,48,48,DARKBLUE,tianqi[2]);
	Draw_word(32+48*2,487-39-39,48,48,DARKBLUE,tianqi[3]);
	Draw_word(32+48*3,487-39-39,48,48,DARKBLUE,tianqi[4]);
	//今日湿度
	Draw_word(240+32,487-39-39,48,48,DARKBLUE,mingritianqi[0]);
	Draw_word(240+32+48,487-39-39,48,48,DARKBLUE,mingritianqi[1]);
	Draw_word(240+32+48*2,487-39-39,48,48,DARKBLUE,mingritianqi[2]);
	Draw_word(240+32+48*3,487-39-39,48,48,DARKBLUE,mingritianqi[3]);
	//'c符号
  Draw_word(140+20*3,540+16*2+40-39-39,40,39,LBBLUE,du_fuhao[0]);
	Draw_word(140+236+20*3,540+16*2+40-39-39,40,39,LBBLUE,du_fuhao[0]);
	Draw_word(140+20*3,540+16-39-39,40,39,LBBLUE,du_fuhao[0]);
	Draw_word(140+236+20*3,540+16-39-39,40,39,LBBLUE,du_fuhao[0]);
	//按键
	Draw_word(45,600,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(115,600,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(185,600,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(255,600,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(325,600,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(395,600,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(45,720,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(115,720,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(185,720,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(255,720,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(325,720,48,45,0xcd2626,kaiguan1[0]);
	Draw_word(395,720,48,45,0xcd2626,kaiguan1[0]);
	//主卧
	Draw_word(64,100+56,48,48,GRAYBLUE,zhuwo[0]);
  Draw_word(64+48,100+56,48,48,GRAYBLUE,zhuwo[1]);
	
	//次卧
	Draw_word(64,156+56,48,48,GRAYBLUE,ciwo[0]);
  Draw_word(64+48,156+56,48,48,GRAYBLUE,ciwo[1]);
	 
	 //客厅
	Draw_word(64,212+56,48,48,GRAYBLUE,keting[0]);
  Draw_word(64+48,212+56,48,48,GRAYBLUE,keting[1]);
	 
	 //厕所
	Draw_word(64,268+56,48,48,GRAYBLUE,canting[0]);
  Draw_word(64+48,268+56,48,48,GRAYBLUE,canting[1]);
	 
	 //空调
	Draw_word(186,103,48,48,GRAYBLUE,kongtiao[0]);
	Draw_word(186+48,103,48,48,GRAYBLUE,kongtiao[1]);
	 
	 //照灯
	Draw_word(283,103,48,48,GRAYBLUE,zhaodeng[0]);
	Draw_word(283+48,103,48,48,GRAYBLUE,zhaodeng[1]);
	
	//图标
	Draw_word(8,540-39-39,128,128,LIGHTBLUE,qing_logo[0]);  //刷新logo
	Draw_word(8+236,540-39-39,128,128,LIGHTBLUE,qing_logo[0]);  //刷新logo
	
	//灯全开
	Draw_word(35,650,16,16,DARKBLUE,dengquankai[0]);  
	Draw_word(35+16,650,16,16,DARKBLUE,dengquankai[1]);  
	Draw_word(35+16+16,650,16,16,DARKBLUE,dengquankai[2]);  
	Draw_word(35+16+16+16,650,16,16,DARKBLUE,dengquankai[3]);  
	
	//空调开关
	Draw_word(110,650,16,16,DARKBLUE,kongtiaodaikaiguan[0]);  
	Draw_word(110+16,650,16,16,DARKBLUE,kongtiaodaikaiguan[1]);  
	Draw_word(110+16+16,650,16,16,DARKBLUE,kongtiaodaikaiguan[2]);  
	Draw_word(110+16+16+16,650,16,16,DARKBLUE,kongtiaodaikaiguan[3]);  
	
	//模式选择
	Draw_word(185,650,16,16,DARKBLUE,moshixuanze[0]);  
	Draw_word(185+16,650,16,16,DARKBLUE,moshixuanze[1]);  
	Draw_word(185+16+16,650,16,16,DARKBLUE,moshixuanze[2]);  
	Draw_word(185+16+16+16,650,16,16,DARKBLUE,moshixuanze[3]);  
	
	//温度调高
	Draw_word(258,650,16,16,DARKBLUE,wendutiaogao[0]);  
	Draw_word(258+16,650,16,16,DARKBLUE,wendutiaogao[1]);  
	Draw_word(258+16+16,650,16,16,DARKBLUE,wendutiaogao[2]);  
	Draw_word(258+16+16+16,650,16,16,DARKBLUE,wendutiaogao[3]);  
	
	//温度调低
	Draw_word(325,650,16,16,DARKBLUE,wendutiaodi[0]);  
	Draw_word(325+16,650,16,16,DARKBLUE,wendutiaodi[1]);  
	Draw_word(325+16+16,650,16,16,DARKBLUE,wendutiaodi[2]);  
	Draw_word(325+16+16+16,650,16,16,DARKBLUE,wendutiaodi[3]); 
	
	//风速选择
	Draw_word(400,650,16,16,DARKBLUE,fengsuxuanze[0]);  
	Draw_word(400+16,650,16,16,DARKBLUE,fengsuxuanze[1]);  
	Draw_word(400+16+16,650,16,16,DARKBLUE,fengsuxuanze[2]);  
	Draw_word(400+16+16+16,650,16,16,DARKBLUE,fengsuxuanze[3]); 

	
	//主卧灯开
	Draw_word(35,770,16,16,DARKBLUE,zhuwodengkai[0]);  
	Draw_word(35+16,770,16,16,DARKBLUE,zhuwodengkai[1]);  
	Draw_word(35+16+16,770,16,16,DARKBLUE,zhuwodengkai[2]);  
	Draw_word(35+16+16+16,770,16,16,DARKBLUE,zhuwodengkai[3]); 
	
	//次卧灯开
	Draw_word(110,770,16,16,DARKBLUE,ciwodengkai[0]);  
	Draw_word(110+16,770,16,16,DARKBLUE,ciwodengkai[1]);  
	Draw_word(110+16+16,770,16,16,DARKBLUE,ciwodengkai[2]);  
	Draw_word(110+16+16+16,770,16,16,DARKBLUE,ciwodengkai[3]); 
	
	//客厅灯开
	Draw_word(185,770,16,16,DARKBLUE,ketingdengkai[0]);  
	Draw_word(185+16,770,16,16,DARKBLUE,ketingdengkai[1]);  
	Draw_word(185+16+16,770,16,16,DARKBLUE,ketingdengkai[2]);  
	Draw_word(185+16+16+16,770,16,16,DARKBLUE,ketingdengkai[3]); 
	
	//餐厅灯开
	Draw_word(258,770,16,16,DARKBLUE,cantingdengkai[0]);  
	Draw_word(258+16,770,16,16,DARKBLUE,cantingdengkai[1]);  
	Draw_word(258+16+16,770,16,16,DARKBLUE,cantingdengkai[2]);  
	Draw_word(258+16+16+16,770,16,16,DARKBLUE,cantingdengkai[3]); 
	
	//大门关闭
	Draw_word(325,770,16,16,DARKBLUE,damneguanbi[0]);  
	Draw_word(325+16,770,16,16,DARKBLUE,damneguanbi[1]);  
	Draw_word(325+16+16,770,16,16,DARKBLUE,damneguanbi[2]);  
	Draw_word(325+16+16+16,770,16,16,DARKBLUE,damneguanbi[3]); 
	
	//灯全关
	Draw_word(400,770,16,16,DARKBLUE,dengquanguan[0]);  
	Draw_word(400+16,770,16,16,DARKBLUE,dengquanguan[1]);  
	Draw_word(400+16+16,770,16,16,DARKBLUE,dengquanguan[2]);  

}


	void draw_weather(u8 tempdata,u8 humidata)
	{
		


		
			if(tempdata<=3)
			{
				
				Draw_mainpage();
				Draw_word(8,540-39-39,128,128,LIGHTBLUE,qing_logo[0]);  //刷新logo
			}
			if(tempdata==9)
			{
				
				Draw_word(8,540-39-39,128,128,LIGHTBLUE,ying_logo[0]);  //刷新logo
			}
			if((4<=tempdata)&&(tempdata<=8))
			{
				
				Draw_word(8,540-39-39,128,128,LIGHTBLUE,duoyun_logo[0]);  //刷新logo
			}
			if(10<=tempdata)
				
			{
				
				Draw_word(8,540-39-39,128,128,LIGHTBLUE,yu_logo[0]);  //刷新logo
			}
			
		

			
				if(tempdata<=3)
			{
				
							Draw_word(8+236,540-39-39,128,128,LIGHTBLUE,qing_logo[0]);  //刷新logo
			}
			if(tempdata==9)
			{
				
							Draw_word(8+236,540-39-39,128,128,LIGHTBLUE,ying_logo[0]);  //刷新logo
			}
			if((4<=tempdata)&&(tempdata<=8))
			{
				
							Draw_word(8+236,540-39-39,128,128,LIGHTBLUE,duoyun_logo[0]);  //刷新logo
			}
			if(10<=tempdata)
			{
				
							Draw_word(8+236,540-39-39,128,128,LIGHTBLUE,yu_logo[0]);  //刷新logo
			
			}
			
		
		

		
			
			
		  LCD_ShowxNum(140+20,540+16*2+40-39-39,10,24,39,0x00);  //显示 数字
			
			
		
		  LCD_ShowxNum(140+20,540+16-39-39,tempdata,24,39,0x00);  //显示 数字
			
		
			
	    LCD_ShowxNum(140+236+20,540+16*2+40-39-39,14,24,39,0x00);  //显示 数字
			
			
		  LCD_ShowxNum(140+236+20,540+16-39-39,(tempdata+2),24,39,0x00);  //显示 数字

	}

void lcdshounum(User_Data_t *User_Data)
{

	LCD_ShowxNum(140+20,540+16*2+40-39-39,10,2,16,0x00);  //显示 数字
			
			
	LCD_ShowxNum(140+20,540+16-39-39,(u32)tempdata,2,16,0x00);  //显示 数字
			
		
	LCD_ShowxNum(140+236+20,540+16*2+40-39-39,14,2,16,0x00);  //显示 数字
			
			
	LCD_ShowxNum(140+236+20,540+16-39-39,(u32)(tempdata+2),2,16,0x00);  //显示 数字
	
	
	LCD_ShowxNum(220,183,User_Data->Vehconf1,2,16,0x00);  //显示主卧空调
	
	LCD_ShowxNum(220,233,User_Data->Vehconf2,2,16,0x00);  //显示次卧空调
	
	LCD_ShowxNum(220,283,User_Data->Vehconf3,2,16,0x00);  //显示客厅空调
	
	LCD_ShowxNum(220,348,User_Data->Vehconf4,2,16,0x00);  //显示厕所空调
	
	
	
	
	LCD_ShowxNum(300,183,User_Data->ledconf1,2,16,0x00);  //显示主卧灯
	
	LCD_ShowxNum(300,233,User_Data->ledconf2,2,16,0x00);  //显示次卧灯
	
	LCD_ShowxNum(300,283,User_Data->ledconf3,2,16,0x00);  //显示客厅灯
	
	LCD_ShowxNum(300,348,User_Data->ledconf4,2,16,0x00);  //显示餐厅灯
	
	LCD_ShowxNum(340,183,User_Data->ledconf1_2,2,16,0x00);  //显示主卧其他灯
	
	LCD_ShowxNum(340,233,User_Data->ledconf2_2,2,16,0x00);  //显示次卧其他灯
	
	LCD_ShowxNum(340,283,User_Data->ledconf3_2,2,16,0x00);  //显示客厅其他灯
	
	LCD_ShowxNum(340,348,User_Data->ledconf4_2,2,16,0x00);  //显示餐厅其他灯
	
	

}


