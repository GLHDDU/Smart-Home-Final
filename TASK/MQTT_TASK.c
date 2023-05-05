#include "MQTT_TASK.h"
#include "stm32f10x.h"
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "usart1.h"     //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
#include "uart4.h"
#include "timer2.h"
#include "timer3.h"     //������Ҫ��ͷ�ļ�
#include "timer4.h"     //������Ҫ��ͷ�ļ�
#include "wifi.h"	      //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "dht11.h" 
#include <stdlib.h>     //������Ҫ��ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "time8.h"
#include "time7.h"
#include "step_motor.h"
#include "uart4.h"

User_Data_t User_Data;

int wifi_model;

u8 tempdata,humidata;	 //�¶�ʪ��

char *cmdid = NULL;  

u32 Led_Light;

char *AData; //��ȡ��ֵ

char temp_password[16];  // ������ʱ��������

char temp_password_paragraph[128]="temp_password/";  // ������ʱ������������--c8t6

void Mqtt_Task(void)
{
	
	printf("WIFI_task\r\n");
	
	WIFI_init();//WIFI��ʼ��
	
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
		
	u1_printf("DHT11_INIT error \r\n"); 
		
	}	
	
	u1_printf("DHT11_INIT success \r\n"); 

	
	while(1)		
	{	
		parse_cmd();
		
		if(Connect_flag==1) //���ӷ������ɹ�  
		{   		
			Handle_TxDataBuff(); /*1�������ͻ���������*/		
			
			Handle_RxDataBuff(); /*2��������ջ���������*/	
			
			Handle_CMDBuf(&User_Data);		 /*3�����������������*/
		}	
		else								//�������ӷ�����
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
	WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
  MQTT_Buff_Init();               //��ʼ������,����,�������ݵ� ������ �Լ���״̬����
	AliIoT_Parameter_Init();	      //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
}


/*�����ͻ���������*/
void Handle_TxDataBuff(void)
{
		if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //���ͻ�������������
		{
		//3������ɽ���if
		//��1�֣�0x10 ���ӱ���
		//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
		//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
			if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
				printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //������ʾ��Ϣ
				MQTT_TxData(MQTT_TxDataOutPtr);                       //�������ݣ�����2���ͻ������е����ݣ�
				MQTT_TxDataOutPtr += BUFF_UNIT; 				//ָ������
				
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
					MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];              //ָ���λ����������ͷ
				} 				
	      }	
}

/*������ջ���������*/
void Handle_RxDataBuff(void)
{
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){//���ջ�������������														
			printf("���յ�����:\r\n");
			
			/*2.1����յ�����CONNECT����*/
			//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
			//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
			if(MQTT_RxDataOutPtr[2]==0x20) //�յ�����CONNECT����
				{             			
					switch(MQTT_RxDataOutPtr[5]){//�ж�CONNECT�����Ƿ�ɹ�					
					case 0x00 : printf("CONNECT���ĳɹ�\r\n");  
									ConnectPack_flag = 1;                                        //CONNECT���ĳɹ������ı��Ŀɷ�
						break;                                                                                           
					case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");
								Connect_flag = 0;                                            //Connect_flag���㣬��������
								break;
					case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");
								Connect_flag = 0;                                            //Connect_flag���㣬��������
								break;
					case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");
								Connect_flag = 0;                                            //Connect_flag���㣬��������
								break;
					case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");
								Connect_flag = 0;                                            //Connect_flag���㣬��������						
								break;
					case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");
								Connect_flag = 0;                                            //Connect_flag���㣬��������						
								break; 
					default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");
								Connect_flag = 0;                                            //Connect_flag���㣬��������					
								break;							
				}				
			}		
			
			/*2.2����յ�����SUBACK����*/
			//�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
			//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
			else if(MQTT_RxDataOutPtr[2]==0x90)//�ж�SUBACK�����Ƿ�ɹ�		
				{
					switch(MQTT_RxDataOutPtr[6])
						{					
					case 0x00 :
					case 0x01 : printf("���ĳɹ�\r\n");
											SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
											Ping_flag = 0;                        //Ping_flag����
											TIM3_ENABLE_30S();                    //����30s��PING��ʱ�� 
					            NVIC_Config();
						          TIM6_Config();                   //����30s���ϴ����ݵĶ�ʱ��  //Ŀǰ��30/6=5S�ϱ�һ��
					            
											TempHumi_State();                     //�ȷ�һ������
								break;                               
					default   : printf("����ʧ�ܣ�׼������\r\n"); 
											Connect_flag = 0;                     //Connect_flag���㣬��������
								break;						
				}					
			}
			
			/*2.3����յ�����PINGRESP����*/
			//�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
			else if(MQTT_RxDataOutPtr[2]==0xD0){ 
				printf("PING���Ļظ�\r\n");
				if(Ping_flag==1){             //���Ping_flag=1����ʾ��һ�η���
					 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
				}else if(Ping_flag>1){ 				//���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
					Ping_flag = 0;     				  //Ҫ���Ping_flag��־
				  TIM3_ENABLE_30S(); 				  //PING��ʱ���ػ�30s��ʱ��			  //PING��ʱ���ػ�30s��ʱ��
				}				
			}	
			
			/*2.4����յ����Ƿ�������������������*/	
                //�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������			
			else if((MQTT_RxDataOutPtr[2]==0x30)){ //����Ҫ��ȡ��������
				printf("�������ȼ�0����\r\n");
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
			}				
							
			MQTT_RxDataOutPtr += BUFF_UNIT;                   //ָ������
			if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)          //���ָ�뵽������β����
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
		}	
}

/*���������������*/
void Handle_CMDBuf(User_Data_t *User_Data)
{
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)  //if�����Ļ���˵�����������������
				  {                             			       
				   printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);                 //���������Ϣ
					  
					/***************�����ƶ�����  ����ͨ��c8t6ִ��****************/
				   //��������ǰ��
				   if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"AlarmSwitch\":1}"))
					{                 
					UART4_SendStr("BiglampON*");
					printf("BiglampSwitch_ON��         \r\n");
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"AlarmSwitch\":0}"))
					{
					UART4_SendStr("BiglampOFF*");
					printf("BiglampSwitch_OFF��         \r\n");;
                    }
					
					//�������Ŷ��
				    if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"ELock\":1}"))
					{
					UART4_SendStr("MG995ON*");
					printf("ELock_ON��         \r\n");;
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"ELock\":0}"))
					{
					UART4_SendStr("MG995OFF*");
					printf("ELock_OFF��         \r\n");
                    }
					
					//��ʱ����
					if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"addOTP\"") != NULL)//רҵ���ݷ���������Ӣ�ĺ�����
					{		 
					  USER_GetSteeringJsonValue((char *)MQTT_CMDOutPtr+2, (char *)"addOTP");
					  printf("��ʱ����:%s\r\n",temp_password);
			
						strcat(temp_password_paragraph, temp_password);
						strcat(temp_password_paragraph, "@*");
						
						
						UART4_SendStr((u8*)temp_password_paragraph);
						printf("New password: %s\n", temp_password_paragraph);
						
						memset((void*)temp_password_paragraph,0,sizeof(temp_password_paragraph));
						strcat(temp_password_paragraph, "temp_password/");

							
					}
					/***************�����ƶ������ִ��****************/
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
						
					printf("�������ƴ�!\r\n");
					  
					User_Data->ledconf1=!User_Data->ledconf1;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led1\":0}"))
					{
						
					TIM_SetCompare2(TIM2, 0) ;
						
					printf("�������ƹر�\r\n");
					  
					User_Data->ledconf1=!User_Data->ledconf1;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoom_Led\":1}"))
					{
						
					TIM_SetCompare4(TIM2, 400) ;	
						
					printf("�������ƴ�\r\n");
					  
					User_Data->ledconf3=!User_Data->ledconf3;
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoom_Led\":0}"))
					{
						
					TIM_SetCompare4(TIM2, 0) ;
						
					printf("�������ƹر�\r\n");
					  
					User_Data->ledconf3=!User_Data->ledconf3;
						
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"canteen_Led\":1}"))
					{
						
					GPIO_SetBits(GPIOG,GPIO_Pin_10);
						
					printf("�������ƴ�\r\n");
					  
					User_Data->ledconf4=!User_Data->ledconf4;
						
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"canteen_Led\":0}"))
							
					{
						
					GPIO_ResetBits(GPIOG,GPIO_Pin_10);
						
					printf("�������ƹر�\r\n");
					  
					User_Data->ledconf4=!User_Data->ledconf4;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led2\":1}"))
							
					{
						
					TIM_SetCompare3(TIM2, 400) ;
						
					printf("�������ƴ�\r\n");
					  
					User_Data->ledconf2=!User_Data->ledconf2;   
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoom_Led2\":0}"))
							
					{
						
					TIM_SetCompare3(TIM2, 0) ;
						
				  printf("�������ƹر�\r\n");
					  
					User_Data->ledconf2=!User_Data->ledconf2; 
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomother_Led1\":1}"))
						
					{
						
					printf("�����������ƹ�");
						
					GPIO_SetBits(GPIOG,GPIO_Pin_7);
		
          User_Data->ledconf1_2=!User_Data->ledconf1_2;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomother_Led1\":0}"))
						
					{
						
					printf("�ر����������ƹ�");
						
					GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		
          User_Data->ledconf1_2=!User_Data->ledconf1_2;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomohter_Led2\":1}"))
						
					{
						
		      printf("�򿪴��������ƹ�");

	        GPIO_SetBits(GPIOG,GPIO_Pin_8);
		
          User_Data->ledconf2_2=!User_Data->ledconf2_2;
						
                    }
					
				  else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"BathRoomother_Led2\":0}"))
					{
						
		      printf("�رմ��������ƹ�");

	        GPIO_ResetBits(GPIOG,GPIO_Pin_8);
		
          User_Data->ledconf2_2=!User_Data->ledconf2_2;
						
                    }
					
			    else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoomother_Led\":1}"))
					
					{
						
		      printf("�򿪿��������ƹ�");

	        GPIO_SetBits(GPIOG,GPIO_Pin_9);
		
          User_Data->ledconf3_2=!User_Data->ledconf3_2;
						
                    }
					
			    else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LivingRoomother_Led\":0}"))
				 
					{
						
		      printf("�رտ��������ƹ�");

	        GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		
          User_Data->ledconf3_2=!User_Data->ledconf3_2;
						
                    }
					
																	
										
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"curtain\":1}"))
								
					{
					 
					printf("��������\r\n");
					  
					Stepturn(360,0);
						
                    }
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"curtain\":0}"))
							
					{						
					 
					printf("���ϴ���\r\n");
					  
					Stepturn(360,1);
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"fan\":1}"))
					{
						
					TIM_SetCompare1(TIM2,899) ;	
						
				  printf("���ȴ�\r\n");
						
					User_Data->fan=!User_Data->fan;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"fan\":0}"))
								
					{
						
					TIM_SetCompare1(TIM2,0) ;	
						
				  printf("���ȹر�\r\n");
						
					User_Data->fan=!User_Data->fan;
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"gexinghua\":1}"))
								
					{
						
					TIM_SetCompare1(TIM2,0) ;	
						
				  printf("���Ի���\r\n");
						
					GPIO_SetBits(GPIOG,GPIO_Pin_9);
						
					TIM_SetCompare1(TIM2,899) ;	
						
						
                    }
					
					else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"gexinghua\":0}"))
								
					{
						
					TIM_SetCompare1(TIM2,0) ;	
						
				  printf("���Ի��ر�\r\n");
						
					GPIO_ResetBits(GPIOG,GPIO_Pin_9);
						
					TIM_SetCompare1(TIM2,0) ;	
						
						
                    }
					
						 

					
	           /*************************/
		        if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"Bethroom_Ledlevel2\"") != NULL)//רҵ���ݷ���������Ӣ�ĺ�����
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"Bethroom_Ledlevel2")+19;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("Led_Light:%d\r\n",Led_Light/100);
					  TIM_SetCompare3(TIM2, Led_Light/100);
						
						printf("�������ȸı�\r\n");
			    }
					
					  if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"Bethroom_Ledlevel\"") != NULL)//רҵ���ݷ���������Ӣ�ĺ�����
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"Bethroom_Ledlevel")+18;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("Led_Light:%d\r\n",Led_Light/100);
					  TIM_SetCompare2(TIM2, Led_Light/100);	
						
						printf("�������ȸı�\r\n");
			    }
					
						if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"LivingRoom_Ledlevel\"") != NULL)//רҵ���ݷ���������Ӣ�ĺ�����
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"LivingRoom_Ledlevel")+20;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("Led_Light:%d\r\n",Led_Light/100);
					  TIM_SetCompare2(TIM2, Led_Light/100);	
						
						printf("�������ȸı�\r\n");
			    }
					
						if(strstr((char *)MQTT_CMDOutPtr+2, (const char *)"\"params\":{\"fanpwm\"") != NULL)//רҵ���ݷ���������Ӣ�ĺ�����
		      {			 
	          AData=strstr((char *)MQTT_CMDOutPtr+2,(const char *)"fanpwm")+7;
						printf("%s",AData);
			      Led_Light=Parsestring(AData);
			      printf("fanpwm:%d\r\n",Led_Light/100);
					  TIM_SetCompare1(TIM2, Led_Light/100);	
						
						printf("����ת�ٸı�\r\n");
			    }
					
					
					
					
					
					
					
				  MQTT_CMDOutPtr += BUFF_UNIT;                             	 //ָ������
				  if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
			}//��������������ݵ�else if��֧��β	
}

/*����������*/
void ReconnectSever(void)
{
		printf("��Ҫ���ӷ�����\r\n");                    //���������Ϣ
		TIM_Cmd(TIM1,DISABLE);                          //�ر�TIM1 
		TIM_Cmd(TIM4,DISABLE);                          //�ر�TIM4  
		WiFi_RxCounter=0;                               //WiFi������������������                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
//		if(wifi_model==0)
//		{
//			if(get_current_weather()==0){   			     		//���WiFi�����Ʒ�������������0����ʾ��ȷ������if
//					printf("��ȡ�������ݳɹ�\r\n");             //���������Ϣ
//					WiFi_RxCounter=0;                             //WiFi������������������                        
//					memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //���WiFi���ջ����� 		
//					MQTT_Buff_ReInit(); //���³�ʼ�����ͻ�����  
//				 wifi_model=1;		
//				}	
//	  }
		if(WiFi_Connect_IoTServer()==0)
		{   			     		                  //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			printf("����TCP���ӳɹ�\r\n");             //���������Ϣ
			Connect_flag = 1;                             //Connect_flag��1����ʾ���ӳɹ�	
			WiFi_RxCounter=0;                             //WiFi������������������                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //���WiFi���ջ����� 		
			MQTT_Buff_ReInit();                           //���³�ʼ�����ͻ�����                    
		}			
}



void TempHumi_State(void)    
{  
	char temp[512];  
	//�����������Ȳ���
	u1_printf("�¶ȣ�%d  ʪ�ȣ�%d\r\n",tempdata,humidata);

	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"CuTemperature\":%2d,\"CurrentHumidity\":%2d,\"LivingRoom_Ledlevel\":%d,\"canteenlevel\":%d,\"BathRoom_Ledlevel\":%d,\"Bethroom_Ledlevel2\":%d,\"LivingRoom_Led\":%d,\"canteenRoom_Led\":%d,\"BathRoom_Led\":%d,\"BathRoom_Led2\":%d,\"VehACShigh\":%d,\"VehACSwitch\":%d,\"VehACSlow\":%d,\"Vehconf1\":%d,\"Vehconf2\":%d,\"Vehconf3\":%d,\"Vehconf4\":%d,},\"version\":\"1.0.0\"}",tempdata,humidata,50,50,50,50,User_Data.ledconf3,User_Data.ledconf4,User_Data.ledconf1,User_Data.ledconf2,tempdata,User_Data.Vehconf1_pattern,tempdata,User_Data.Vehconf1,User_Data.Vehconf2,User_Data.Vehconf3,User_Data.Vehconf4);  //�����ظ�ʪ���¶�����
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������	
	
	}



/*******************************************************************
*������unsigned char *USER_GetSteeringJsonValue(char *cJson, char *Tag)
*���ܣ�jsonΪ�ַ������У���json��ʽ�е�Ŀ�����Tag��Ӧ��ֵ�ַ���ת��Ϊ�ַ���
*���룺
		char *cJson json�ַ���
		char *Tag Ҫ�����Ķ����ǩ
*�����������ֵ
*����˵�����û������ڴ˻����ϸ������չ�ú���������ֻ�Ǹ��򵥵�DEMO
*******************************************************************/
//�˺����ѸĶ�----������ʱ����
void USER_GetSteeringJsonValue(char *cJson, char *Tag)
{ 
  char *target = NULL;
	int8_t i=0;
	memset(temp_password, 0x00, 128);
	sprintf(temp_password,"\"%s\":\"",Tag);
	target=strstr((const char *)cJson, (const char *)temp_password);
	if(target == NULL)
	{
		//printf("���ַ���\r\n");
		return ;
	}
	i=strlen((const char *)temp_password);
	target=target+i;   //��ַΪ���ݵ�ַ
	memset(temp_password, 0x00, 128);
	for(i=0; i<12; i++, target++)//��ֵ����10��λΪ�Ƿ�������2^32=4294967296
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
	//printf("��ʱ����1111111111:%s\r\n",temp_password);
}

int Parsestring(char *AData)
{
	int longer;
	int i;  //i ���ڿ���ѭ����longer���ڻ�ȡ�ַ����Ĵ�С
	int numz = 0;
	char *flag;
//�������ַ����е����� -- ָ��ʵ��
	
	longer = strlen(AData);
	flag = AData;
	for(i = 0 ; i < longer ; i++)
	{
		if((*flag >= '0')&&( *flag <= '9')) 
		{
			numz = numz*10 + *flag - '0';//���������ַ���ASCLL��ֵ���бȽϣ�Ҫ��ȥ0��ASCLL��ֵ��Ϊʵ�ʵ�����
		}
		flag++;
	}
	//printf(" num = %d \r\n",numz);
	return numz;

}

		




