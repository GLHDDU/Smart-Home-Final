#include "MQTT_TASK.h"
#include "stm32f10x.h"
#include "delay.h"      //包含需要的头文件
#include "usart1.h"     //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "uart4.h"
#include "timer2.h"
#include "timer3.h"     //包含需要的头文件
#include "timer4.h"     //包含需要的头文件
#include "wifi.h"	      //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "mqtt.h"       //包含需要的头文件
#include "dht11.h" 
#include <stdlib.h>     //包含需要的头文件
#include "FreeRTOS.h"
#include "task.h"
#include "time8.h"
#include "time7.h"
#include "step_motor.h"
#include "uart4.h"

User_Data_t User_Data;

int wifi_model;

u8 tempdata,humidata;	 //温度湿度

char *cmdid = NULL;  

u32 Led_Light;

char *AData; //提取数值

char temp_password[16];  // 解析临时密码数组

char temp_password_paragraph[128]="temp_password/";  // 发送临时密码数据命令--c8t6

void Mqtt_Task(void)
{
	
	printf("WIFI_task\r\n");
	
	WIFI_init();//WIFI初始化
	
 	while(DHT11_Init())	//DHT11初始化	
	{
		
	u1_printf("DHT11_INIT error \r\n"); 
		
	}	
	
	u1_printf("DHT11_INIT success \r\n"); 

	
	while(1)		
	{	
		parse_cmd();
		
		if(Connect_flag==1) //连接服务器成功  
		{   		
			Handle_TxDataBuff(); /*1、处理发送缓冲区数据*/		
			
			Handle_RxDataBuff(); /*2、处理接收缓冲区数据*/	
			
			Handle_CMDBuf(&User_Data);		 /*3、处理命令缓冲区数据*/
		}	
		else								//重启连接服务器
		{
 			
			ReconnectSever();
			
		}
	  vTaskDelay(500);
	}	
}

void WIFI_init(void)
{
	TIM7_Config(300,7200);
	NVICTIM7_Config();
	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
  MQTT_Buff_Init();               //初始化接收,发送,命令数据的 缓冲区 以及各状态参数
	AliIoT_Parameter_Init();	      //初始化连接阿里云IoT平台MQTT服务器的参数	
}


/*处理发送缓冲区数据*/
void Handle_TxDataBuff(void)
{
		if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //发送缓冲区有数据了
		{
		//3种情况可进入if
		//第1种：0x10 连接报文
		//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
		//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
			if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
				printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
				MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据（串口2发送缓冲区中的数据）
				MQTT_TxDataOutPtr += BUFF_UNIT; 				//指针下移
				
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
					MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];              //指针归位到缓冲区开头
				} 				
	      }	
}

/*处理接收缓冲区数据*/
void Handle_RxDataBuff(void)
{
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){//接收缓冲区有数据了														
			printf("接收到数据:\r\n");
			
			/*2.1如果收到的是CONNECT报文*/
			//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
			//接着我们要判断第4个字节，看看CONNECT报文是否成功
			if(MQTT_RxDataOutPtr[2]==0x20) //收到的是CONNECT报文
				{             			
					switch(MQTT_RxDataOutPtr[5]){//判断CONNECT报文是否成功					
					case 0x00 : printf("CONNECT报文成功\r\n");  
									ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
						break;                                                                                           
					case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接
								break;
					case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接
								break;
					case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接
								break;
					case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接						
								break;
					case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接						
								break; 
					default   : printf("连接已拒绝，未知状态，准备重启\r\n");
								Connect_flag = 0;                                            //Connect_flag置零，重启连接					
								break;							
				}				
			}		
			
			/*2.2如果收到的是SUBACK报文*/
			//判断，第一个字节是0x90，表示收到的是SUBACK报文
			//接着我们要判断订阅回复，看看是不是成功
			else if(MQTT_RxDataOutPtr[2]==0x90)//判断SUBACK报文是否成功		
				{
					switch(MQTT_RxDataOutPtr[6])
						{					
					case 0x00 :
					case 0x01 : printf("订阅成功\r\n");
											SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
											Ping_flag = 0;                        //Ping_flag清零
											TIM3_ENABLE_30S();                    //启动30s的PING定时器 
					            NVIC_Config();
						          TIM6_Config();                   //启动30s的上传数据的定时器  //目前是30/6=5S上报一次
					            
											TempHumi_State();                     //先发一次数据
								break;                               
					default   : printf("订阅失败，准备重启\r\n"); 
											Connect_flag = 0;                     //Connect_flag置零，重启连接
								break;						
				}					
			}
			
			/*2.3如果收到的是PINGRESP报文*/
			//判断，第一个字节是0xD0，表示收到的是PINGRESP报文
			else if(MQTT_RxDataOutPtr[2]==0xD0){ 
				printf("PING报文回复\r\n");
				if(Ping_flag==1){             //如果Ping_flag=1，表示第一次发送
					 Ping_flag = 0;    				  //要清除Ping_flag标志
				}else if(Ping_flag>1){ 				//如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
					Ping_flag = 0;     				  //要清除Ping_flag标志
				  TIM3_ENABLE_30S(); 				  //PING定时器重回30s的时间			  //PING定时器重回30s的时间
				}				
			}	
			
			/*2.4如果收到的是服务器发来的推送数据*/	
                //判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令			
			else if((MQTT_RxDataOutPtr[2]==0x30)){ //我们要提取控制命令
				printf("服务器等级0推送\r\n");
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
			}				
							
			MQTT_RxDataOutPtr += BUFF_UNIT;                   //指针下移
			if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)          //如果指针到缓冲区尾部了
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
		}	
}

/*处理命令缓冲区数据*/
void Handle_CMDBuf(User_Data_t *User_Data)
{
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)  //if成立的话，说明命令缓冲区有数据了
				  {                             			       
				   printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);                 //串口输出信息
					  
					/***************解析云端命令  串口通信c8t6执行****************/
				   //开，关门前灯
				   if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"AlarmSwitch\":1}"))
					{                 
					UART4_SendStr("BiglampON*");
					printf("BiglampSwitch_ON！         \r\n");
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"AlarmSwitch\":0}"))
					{
					UART4_SendStr("BiglampOFF*");
					printf("BiglampSwitch_OFF！         \r\n");;
                    }
					
					//开，关门舵机
				    if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"ELock\":1}"))
					{
					UART4_SendStr("MG995ON*");
					printf("ELock_ON！         \r\n");;
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"ELock\":0}"))
					{
					UART4_SendStr("MG995OFF*");
					printf("ELock_OFF！         \r\n");
                    }
					
					//临时密码
					if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"addOTP\"") != NULL)//专业数据分析，包括英文和数字
					{		 
					  USER_GetSteeringJsonValue((char *)MQTT_CMDOutPtr+2, (char *)"addOTP");
					  printf("临时密码:%s\r\n",temp_password);
			
						strcat(temp_password_paragraph, temp_password);
						strcat(temp_password_paragraph, "@*");
						
						
						UART4_SendStr((u8*)temp_password_paragraph);
						printf("New password: %s\n", temp_password_paragraph);
						
						memset((void*)temp_password_paragraph,0,sizeof(temp_password_paragraph));
						strcat(temp_password_paragraph, "temp_password/");

							
					}
					/***************解析云端命令本地执行****************/
				  if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"ALLLedOpen\":1}"))
					{
						
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
					
					}	
					
				  else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"ALLLedOpen\":0}"))
					{
						
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
					
					}
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led1\":1}"))
					{
						
					TIM_SetCompare2(TIM2, 400) ;	
						
					printf("主卧主灯打开!\r\n");
					  
					User_Data->ledconf1=!User_Data->ledconf1;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led1\":0}"))
					{
						
					TIM_SetCompare2(TIM2, 0) ;
						
					printf("主卧主灯关闭\r\n");
					  
					User_Data->ledconf1=!User_Data->ledconf1;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoom_Led\":1}"))
					{
						
					TIM_SetCompare4(TIM2, 400) ;	
						
					printf("客厅主灯打开\r\n");
					  
					User_Data->ledconf3=!User_Data->ledconf3;
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoom_Led\":0}"))
					{
						
					TIM_SetCompare4(TIM2, 0) ;
						
					printf("客厅主灯关闭\r\n");
					  
					User_Data->ledconf3=!User_Data->ledconf3;
						
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"canteen_Led\":1}"))
					{
						
					GPIO_SetBits(GPIOG,GPIO_Pin_10);
						
					printf("餐厅主灯打开\r\n");
					  
					User_Data->ledconf4=!User_Data->ledconf4;
						
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"canteen_Led\":0}"))
							
					{
						
					GPIO_ResetBits(GPIOG,GPIO_Pin_10);
						
					printf("餐厅主灯关闭\r\n");
					  
					User_Data->ledconf4=!User_Data->ledconf4;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led2\":1}"))
							
					{
						
					TIM_SetCompare3(TIM2, 400) ;
						
					printf("次卧主灯打开\r\n");
					  
					User_Data->ledconf2=!User_Data->ledconf2;   
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led2\":0}"))
							
					{
						
					TIM_SetCompare3(TIM2, 0) ;
						
				  printf("次卧主灯关闭\r\n");
					  
					User_Data->ledconf2=!User_Data->ledconf2; 
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomother_Led1\":1}"))
						
					{
						
					printf("打开主卧其他灯光");
						
					GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
          User_Data->ledconf1_2=!User_Data->ledconf1_2;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomother_Led1\":0}"))
						
					{
						
					printf("关闭主卧其他灯光");
						
					GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		
          User_Data->ledconf1_2=!User_Data->ledconf1_2;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomohter_Led2\":1}"))
						
					{
						
		      printf("打开次卧其他灯光");

	        GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
          User_Data->ledconf2_2=!User_Data->ledconf2_2;
						
                    }
					
				  else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomother_Led2\":0}"))
					{
						
		      printf("关闭次卧其他灯光");

	        GPIO_ResetBits(GPIOG,GPIO_Pin_8);
		
          User_Data->ledconf2_2=!User_Data->ledconf2_2;
						
                    }
					
			    else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoomother_Led\":1}"))
					
					{
						
		      printf("打开客厅其他灯光");

	        GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
          User_Data->ledconf3_2=!User_Data->ledconf3_2;
						
                    }
					
			    else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoomother_Led\":0}"))
				 
					{
						
		      printf("关闭客厅其他灯光");

	        GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		
          User_Data->ledconf3_2=!User_Data->ledconf3_2;
						
                    }
					
																	
										
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"curtain\":1}"))
								
					{
					 
					printf("拉开窗帘\r\n");
					  
					Stepturn(360,0);
						
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"curtain\":0}"))
							
					{						
					 
					printf("拉上窗帘\r\n");
					  
					Stepturn(360,1);
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"fan\":1}"))
					{
						
					TIM_SetCompare1(TIM2,899) ;	
						
				  printf("风扇打开\r\n");
						
					User_Data->fan=!User_Data->fan;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"fan\":0}"))
								
					{
						
					TIM_SetCompare1(TIM2,0) ;	
						
				  printf("风扇关闭\r\n");
						
					User_Data->fan=!User_Data->fan;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"gexinghua\":1}"))
								
					{
						
					TIM_SetCompare1(TIM2,0) ;	
						
				  printf("个性化打开\r\n");
						
					GPIO_SetBits(GPIOG,GPIO_Pin_9);
						
					TIM_SetCompare1(TIM2,899) ;	
						
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"gexinghua\":0}"))
								
					{
						
					TIM_SetCompare1(TIM2,0) ;	
						
				  printf("个性化关闭\r\n");
						
					GPIO_ResetBits(GPIOG,GPIO_Pin_9);
						
					TIM_SetCompare1(TIM2,0) ;	
						
						
                    }
					
						 

					
	           /*************************/
		        if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"Bethroom_Ledlevel2\"") != NULL)//专业数据分析，包括英文和数字
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"Bethroom_Ledlevel2")+19;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("Led_Light:%d\r\n",Led_Light/100);
					  TIM_SetCompare3(TIM2, Led_Light/100);
						
						printf("次卧亮度改变\r\n");
			    }
					
					  if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"Bethroom_Ledlevel\"") != NULL)//专业数据分析，包括英文和数字
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"Bethroom_Ledlevel")+18;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("Led_Light:%d\r\n",Led_Light/100);
					  TIM_SetCompare2(TIM2, Led_Light/100);	
						
						printf("主卧亮度改变\r\n");
			    }
					
						if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"LivingRoom_Ledlevel\"") != NULL)//专业数据分析，包括英文和数字
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"LivingRoom_Ledlevel")+20;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("Led_Light:%d\r\n",Led_Light/100);
					  TIM_SetCompare2(TIM2, Led_Light/100);	
						
						printf("客厅亮度改变\r\n");
			    }
					
						if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"fanpwm\"") != NULL)//专业数据分析，包括英文和数字
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"fanpwm")+7;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("fanpwm:%d\r\n",Led_Light/100);
					  TIM_SetCompare1(TIM2, Led_Light/100);	
						
						printf("风扇转速改变\r\n");
			    }
					
					
					
					
					
					
					
				  MQTT_CMDOutPtr += BUFF_UNIT;                             	 //指针下移
				  if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头				
			}//处理命令缓冲区数据的else if分支结尾	
}

/*重连服务器*/
void ReconnectSever(void)
{
		printf("需要连接服务器\r\n");                    //串口输出信息
		TIM_Cmd(TIM1,DISABLE);                          //关闭TIM1 
		TIM_Cmd(TIM4,DISABLE);                          //关闭TIM4  
		WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
//		if(wifi_model==0)
//		{
//			if(get_current_weather()==0){   			     		//如果WiFi连接云服务器函数返回0，表示正确，进入if
//					printf("获取天气数据成功\r\n");             //串口输出信息
//					WiFi_RxCounter=0;                             //WiFi接收数据量变量清零                        
//					memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //清空WiFi接收缓冲区 		
//					MQTT_Buff_ReInit(); //重新初始化发送缓冲区  
//				 wifi_model=1;		
//				}	
//	  }
		if(WiFi_Connect_IoTServer()==0)
		{   			     		                  //如果WiFi连接云服务器函数返回0，表示正确，进入if
			printf("建立TCP连接成功\r\n");             //串口输出信息
			Connect_flag = 1;                             //Connect_flag置1，表示连接成功	
			WiFi_RxCounter=0;                             //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //清空WiFi接收缓冲区 		
			MQTT_Buff_ReInit();                           //重新初始化发送缓冲区                    
		}			
}



void TempHumi_State(void)    
{  
	char temp[512];  
	//下面这三个先不用
	u1_printf("温度：%d  湿度：%d\r\n",tempdata,humidata);

	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"CuTemperature\":%2d,\"CurrentHumidity\":%2d,\"LivingRoom_Ledlevel\":%d,\"canteenlevel\":%d,\"BathRoom_Ledlevel\":%d,\"Bethroom_Ledlevel2\":%d,\"LivingRoom_Led\":%d,\"canteenRoom_Led\":%d,\"BathRoom_Led\":%d,\"BathRoom_Led2\":%d,\"VehACShigh\":%d,\"VehACSwitch\":%d,\"VehACSlow\":%d,\"Vehconf1\":%d,\"Vehconf2\":%d,\"Vehconf3\":%d,\"Vehconf4\":%d,},\"version\":\"1.0.0\"}",tempdata,humidata,50,50,50,50,User_Data.ledconf3,User_Data.ledconf4,User_Data.ledconf1,User_Data.ledconf2,tempdata,User_Data.Vehconf1_pattern,tempdata,User_Data.Vehconf1,User_Data.Vehconf2,User_Data.Vehconf3,User_Data.Vehconf4);  //构建回复湿度温度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
	
	}



/*******************************************************************
*函数：unsigned char *USER_GetSteeringJsonValue(char *cJson, char *Tag)
*功能：json为字符串序列，将json格式中的目标对象Tag对应的值字符串转换为字符串
*输入：
		char *cJson json字符串
		char *Tag 要操作的对象标签
*输出：返回数值
*特殊说明：用户可以在此基础上改造和扩展该函数，这里只是个简单的DEMO
*******************************************************************/
//此函数已改动----解析临时密码
void USER_GetSteeringJsonValue(char *cJson, char *Tag)
{ 
  char *target = NULL;
	int8_t i=0;
	memset(temp_password, 0x00, 128);
	sprintf(temp_password,"\"%s\":\"",Tag);
	target=strstr((const char *)cJson, (const char *)temp_password);
	if(target == NULL)
	{
		//printf("空字符！\r\n");
		return ;
	}
	i=strlen((const char *)temp_password);
	target=target+i;   //地址为数据地址
	memset(temp_password, 0x00, 128);
	for(i=0; i<12; i++, target++)//数值超过10个位为非法，由于2^32=4294967296
	{
		if((*target!='"'))
		{
			temp_password[i]=*target;
		}
		else
		{
			break;
		}
	}
	temp_password[i+1] = '\0';
	//printf("临时密码1111111111:%s\r\n",temp_password);
}

int Parsestring(char *AData)
{
	int longer;
	int i;  //i 用于控制循环，longer用于获取字符串的大小
	int numz = 0;
	char *flag;
//解析出字符串中的数字 -- 指针实现
	
	longer = strlen(AData);
	flag = AData;
	for(i = 0 ; i < longer ; i++)
	{
		if((*flag >= '0')&&( *flag <= '9')) 
		{
			numz = numz*10 + *flag - '0';//这里是用字符的ASCLL码值进行比较，要减去0的ASCLL码值即为实际的数字
		}
		flag++;
	}
	//printf(" num = %d \r\n",numz);
	return numz;

}

		




