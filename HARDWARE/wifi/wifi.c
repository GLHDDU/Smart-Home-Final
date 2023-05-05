/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ����602Wifi���ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "wifi.h"	    //������Ҫ��ͷ�ļ�
#include "key.h"
#include "delay.h"	    //������Ҫ��ͷ�ļ�
#include "usart1.h"	    //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
extern unsigned int Usart2_RxCounter; 
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE];
#define WEATHER_PORTNUM 	"80"		//�������Ӷ˿ں�:80	
#define WEATHER_SERVERIP 	"api.seniverse.com"		//����������IP
char wifi_mode = 0;     //����ģʽ 0��SSID������д�ڳ�����   1��Smartconfig��ʽ��APP����
u8 weather1,high_temp1,low_temp1,weather2,high_temp2,low_temp2;

/*-------------------------------------------------*/
/*����������ʼ��WiFi�ĸ�λIO                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //����һ������IO�˿ڲ����Ľṹ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);   //ʹ��PA�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //׼������PA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //���������ʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //����PA4
	RESET_IO(1);                                              //��λIO���ߵ�ƽ
}
/*-------------------------------------------------*/
/*��������WiFi��������ָ��                         */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //���WiFi���ջ����� 
	WiFi_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		delay_ms(100);                          //��ʱ100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						//��������whileѭ��
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������WiFi��λ                                 */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO(0);                                    //��λIO���͵�ƽ
	delay_ms(500);                                  //��ʱ500ms
	RESET_IO(1);                                    //��λIO���ߵ�ƽ	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(100);                              //��ʱ100ms
		if(strstr(WiFi_RX_BUF,"ready"))             //������յ�ready��ʾ��λ�ɹ�
			break;       						    //��������whileѭ��
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
	else return 0;		         				    //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������WiFi����·����ָ��                       */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter=0;                               //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS); //����ָ��	
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
		if(strstr(WiFi_RX_BUF,"\n\r\nOK")) //������յ�WIFI GOT IP��ʾ�ɹ�����������ԭ�������ʽ��		WIFI GOT IP\n\r\nOK���ϰ汾�����ʽ��WIFI GOT IP\r\n\r\nOK��Ϊ��ͨ�ã��˴���������
			break;       						    //��������whileѭ��
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	return 0;                                       //��ȷ������0
}
/*-------------------------------------------------*/
/*������������TCP��������������͸��ģʽ            */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{	
	WiFi_RxCounter=0;                               //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ�����   
	WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ServerIP,ServerPort);//�������ӷ�����ָ��
	while(timeout--){                               //�ȴ���ʱ���
		delay_ms(100);                              //��ʱ100ms	
		if(strstr(WiFi_RX_BUF ,"CONNECT"))          //������ܵ�CONNECT��ʾ���ӳɹ�
			break;                                  //����whileѭ��
		if(strstr(WiFi_RX_BUF ,"CLOSED"))           //������ܵ�CLOSED��ʾ������δ����
			return 1;                               //������δ��������1
		if(strstr(WiFi_RX_BUF ,"ALREADY CONNECTED"))//������ܵ�ALREADY CONNECTED�Ѿ���������
			return 2;                               //�Ѿ��������ӷ���2
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��  
	}
	u1_printf("\r\n");                        //���������Ϣ
	if(timeout<=0)return 3;                   //��ʱ���󣬷���3
	else                                      //���ӳɹ���׼������͸��
	{
		u1_printf("���ӷ������ɹ���׼������͸��\r\n");  //������ʾ��Ϣ
		WiFi_RxCounter=0;                               //WiFi������������������                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ�����     
		WiFi_printf("AT+CIPSEND\r\n");                  //���ͽ���͸��ָ��
		while(timeout--){                               //�ȴ���ʱ���
			delay_ms(100);                              //��ʱ100ms	
			if(strstr(WiFi_RX_BUF,"\r\nOK\r\n\r\n>"))   //���������ʾ����͸���ɹ�
				break;                          //����whileѭ��
			u1_printf("%d ",timeout);           //����������ڵĳ�ʱʱ��  
		}
		if(timeout<=0)return 4;                 //͸����ʱ���󣬷���4	
	}
	return 0;	                                //�ɹ�����0	
}
/*-------------------------------------------------*/
/*��������WiFi_Smartconfig                         */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Smartconfig(int timeout)
{
	
	WiFi_RxCounter=0;                           //WiFi������������������                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //���WiFi���ջ�����     
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                         //��ʱ1s
		if(strstr(WiFi_RX_BUF,"connected"))     //������ڽ��ܵ�connected��ʾ�ɹ�
			break;                              //����whileѭ��  
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��  
	}	
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //��ʱ���󣬷���1
	return 0;                                   //��ȷ����0
}
/*-------------------------------------------------*/
/*���������ȴ�����·����                           */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_WaitAP(int timeout)
{		
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP"))       //������յ�WIFI GOT IP��ʾ�ɹ�
			break;       						    //��������whileѭ��
		u1_printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                              //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	return 0;                                       //��ȷ������0
}
/*-------------------------------------------------*/
/*��������WiFi���ӷ�����                           */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{	
	u1_printf("׼����λģ��\r\n");                     //������ʾ����
	if(WiFi_Reset(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("��λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
		return 1;                                      //����1
	}else u1_printf("��λ�ɹ�\r\n");                   //������ʾ����
	
	u1_printf("׼������STAģʽ\r\n");                  //������ʾ����
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����STAģʽʧ�ܣ�׼������\r\n");    //���ط�0ֵ������if��������ʾ����
		return 2;                                      //����2
	}else u1_printf("����STAģʽ�ɹ�\r\n");            //������ʾ����
	
	if(wifi_mode==0){                                      //�������ģʽ=0��SSID������д�ڳ����� 
		u1_printf("׼��ȡ���Զ�����\r\n");                 //������ʾ����
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //ȡ���Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
			u1_printf("ȡ���Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
			return 3;                                      //����3
		}else u1_printf("ȡ���Զ����ӳɹ�\r\n");           //������ʾ����
				
		u1_printf("׼������·����\r\n");                   //������ʾ����	
		if(WiFi_JoinAP(30)){                               //����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
			u1_printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
			return 4;                                      //����4	
		}else u1_printf("����·�����ɹ�\r\n");             //������ʾ����			
	}else{                                                 //�������ģʽ=1��Smartconfig��ʽ,��APP����
		if(wifi_mode==1){                                    //�����ʱK2�ǰ��µ�
			u1_printf("׼�������Զ�����\r\n");                 //������ʾ����
			if(WiFi_SendCmd("AT+CWAUTOCONN=1",50)){            //�����Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
				u1_printf("�����Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
				return 3;                                      //����3
			}else u1_printf("�����Զ����ӳɹ�\r\n");           //������ʾ����	
			
			u1_printf("׼������Smartconfig\r\n");              //������ʾ����
			if(WiFi_SendCmd("AT+CWSTARTSMART",50)){            //����Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
				u1_printf("����Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
				return 4;                                      //����4
			}else u1_printf("����Smartconfig�ɹ�\r\n");        //������ʾ����

			u1_printf("��ʹ��APP�����������\r\n");            //������ʾ����
			if(WiFi_Smartconfig(60)){                          //APP����������룬1s��ʱ��λ���ܼ�60s��ʱʱ��
				u1_printf("��������ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
				return 5;                                      //����5
			}else u1_printf("��������ɹ�\r\n");               //������ʾ����

			u1_printf("׼���ر�Smartconfig\r\n");              //������ʾ����
			if(WiFi_SendCmd("AT+CWSTOPSMART",50)){             //�ر�Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
				u1_printf("�ر�Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
				return 6;                                      //����6
			}else u1_printf("�ر�Smartconfig�ɹ�\r\n");        //������ʾ����
		}else{                                                 //��֮����ʱK2��û�а���
			u1_printf("�ȴ�����·����\r\n");                   //������ʾ����	
			if(WiFi_WaitAP(30)){                               //�ȴ�����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
				u1_printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
				return 7;                                      //����7	
			}else u1_printf("����·�����ɹ�\r\n");             //������ʾ����					
		}
	}
	
	u1_printf("׼������͸��\r\n");                     //������ʾ����
	if(WiFi_SendCmd("AT+CIPMODE=1",50)){               //����͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
		return 8;                                      //����8
	}else u1_printf("����͸���ɹ�\r\n");               //������ʾ����
	
	u1_printf("׼���رն�·����\r\n");                 //������ʾ����
	if(WiFi_SendCmd("AT+CIPMUX=0",50)){                //�رն�·���ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("�رն�·����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
		return 9;                                      //����9
	}else u1_printf("�رն�·���ӳɹ�\r\n");           //������ʾ����
	
	u1_printf("׼�����ӷ�����\r\n");                   //������ʾ����
	if(WiFi_Connect_Server(100)){                      //���ӷ�������100ms��ʱ��λ���ܼ�10s��ʱʱ��
		u1_printf("���ӷ�����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
		return 10;                                     //����10
	}else u1_printf("���ӷ������ɹ�\r\n");             //������ʾ����	
	
	return 0;                                          //��ȷ����0
	
}


/*-------------------------------------------------*/
/*��������WiFi���ӷ�����                           */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char get_current_weather(void)
{	
	printf("׼����λģ��\r\n");                     //������ʾ����
	if(WiFi_Reset(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("��λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
		return 1;                                      //����1
	}else printf("��λ�ɹ�\r\n");                   //������ʾ����
	
	printf("׼������STAģʽ\r\n");                  //������ʾ����
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("����STAģʽʧ�ܣ�׼������\r\n");    //���ط�0ֵ������if��������ʾ����
		return 2;                                      //����2
	}else printf("����STAģʽ�ɹ�\r\n");            //������ʾ����
	
	if(wifi_mode==0){                                      //�������ģʽ=0��SSID������д�ڳ����� 
		printf("׼��ȡ���Զ�����\r\n");                 //������ʾ����
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //ȡ���Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
			printf("ȡ���Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
			return 3;                                      //����3
		}else printf("ȡ���Զ����ӳɹ�\r\n");           //������ʾ����
				
		printf("׼������·����\r\n");                   //������ʾ����	
		if(WiFi_JoinAP(30)){                               //����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
			printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
			return 4;                                      //����4	
		}else printf("����·�����ɹ�\r\n");             //������ʾ����			
	}else{                                                 //�������ģʽ=1��Smartconfig��ʽ,��APP����
		if(KEY0==0){                                    //�����ʱK0�ǰ��µ�
			printf("׼�������Զ�����\r\n");                 //������ʾ����
			if(WiFi_SendCmd("AT+CWAUTOCONN=1",50)){            //�����Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
				printf("�����Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
				return 3;                                      //����3
			}else printf("�����Զ����ӳɹ�\r\n");           //������ʾ����	
			
			printf("׼������Smartconfig\r\n");              //������ʾ����
			if(WiFi_SendCmd("AT+CWSTARTSMART",50)){            //����Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
				printf("����Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
				return 4;                                      //����4
			}else printf("����Smartconfig�ɹ�\r\n");        //������ʾ����

			printf("��ʹ��APP�����������\r\n");            //������ʾ����
			if(WiFi_Smartconfig(60)){                          //APP����������룬1s��ʱ��λ���ܼ�60s��ʱʱ��
				printf("��������ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
				return 5;                                      //����5
			}else printf("��������ɹ�\r\n");               //������ʾ����

			printf("׼���ر�Smartconfig\r\n");              //������ʾ����
			if(WiFi_SendCmd("AT+CWSTOPSMART",50)){             //�ر�Smartconfig��100ms��ʱ��λ���ܼ�5s��ʱʱ��
				printf("�ر�Smartconfigʧ�ܣ�׼������\r\n");//���ط�0ֵ������if��������ʾ����
				return 6;                                      //����6
			}else printf("�ر�Smartconfig�ɹ�\r\n");        //������ʾ����
		}else{                                                 //��֮����ʱK2��û�а���
			printf("�ȴ�����·����\r\n");                   //������ʾ����	
			if(WiFi_WaitAP(30)){                               //�ȴ�����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
				printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
				return 7;                                      //����7	
			}else printf("����·�����ɹ�\r\n");             //������ʾ����					
		}
	}
	
	printf("׼������͸��\r\n");                     //������ʾ����
	if(WiFi_SendCmd("AT+CIPMODE=1",50)){               //����͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("����͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
		return 8;                                      //����8
	}else printf("����͸���ɹ�\r\n");               //������ʾ����
	
	printf("׼���رն�·����\r\n");                 //������ʾ����
	if(WiFi_SendCmd("AT+CIPMUX=0",50)){                //�رն�·���ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("�رն�·����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
		return 9;                                      //����9
	}else printf("�رն�·���ӳɹ�\r\n");           //������ʾ����
	//////////*******׼����ȡ��������**********////////////////
	printf("׼����ȡ��������\r\n");                 //������ʾ����
	if(get_weather()){                //�رն�·���ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("��ȡ��������ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
		return 9;                                      //����9
	}else printf("��ȡ�������ݳɹ�\r\n");           //������ʾ����
	//////////////////////////////////////////
	return 0;                                          //��ȷ����0
	
}

//��ȡһ��ʵʱ����
//���أ�0---��ȡ�ɹ���1---��ȡʧ��
char get_weather(void)
{

	
	char temp[256];
	sprintf(temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",WEATHER_SERVERIP,WEATHER_PORTNUM);    //����Ŀ��TCP������
	printf("send:AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",WEATHER_SERVERIP,WEATHER_PORTNUM);
	WiFi_SendCmd(temp,50);

	delay_ms(300);
	
	printf("send:AT+CIPMODE=1\r\n");	
	WiFi_SendCmd("AT+CIPMODE=1",50);      //����ģʽΪ��͸��	

	Usart2_RxCounter=0;
	printf("send:AT+CIPSEND\r\n");	
	WiFi_SendCmd("AT+CIPSEND",50);         //��ʼ͸��
	printf("start trans...\r\n");
	//��ǰ
	//u2_printf("GET https://api.seniverse.com/v3/weather/now.json?key=St4bOhFvShcUJhU4_&location=hengyang&language=en&unit=c\n\n");	
	//���տ�ʼ3��
	u2_printf("GET  https://api.seniverse.com/v3/weather/daily.json?key=SF186LL7teJi5Gofo&location=hengyang&language=en&unit=c&start=0&days=5\n\n");	
	//���տ�ʼ2��
	//u2_printf("GET  https://api.seniverse.com/v3/weather/daily.json?key=SF186LL7teJi5Gofo&location=hengyang&language=en&unit=c&start=1&days=5\n\n");	

	delay_ms(20);//��ʱ20ms���ص���ָ��ͳɹ���״̬
	Usart2_RxCounter=0;	//���㴮��3����
	delay_ms(1000);
	if(Usart2_RxCounter&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	{ 
		Usart2_RxBuff[Usart2_RxCounter&0X7FFF]=0;//��ӽ�����
	} 
	printf("USART2_RX_BUF=%s\r\n",Usart2_RxBuff);
	USER_DataAnalysisProcess((char *)Usart2_RxBuff,1);
	///////��ȡ�ڶ�������//////////
	u2_printf("GET  https://api.seniverse.com/v3/weather/daily.json?key=SF186LL7teJi5Gofo&location=hengyang&language=en&unit=c&start=1&days=5\n\n");	

	delay_ms(20);//��ʱ20ms���ص���ָ��ͳɹ���״̬
	Usart2_RxCounter=0;	//���㴮��3����
	delay_ms(1000);
	if(Usart2_RxCounter&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	{ 
		Usart2_RxBuff[Usart2_RxCounter&0X7FFF]=0;//��ӽ�����
	} 
	printf("USART2_RX_BUF=%s\r\n",Usart2_RxBuff);
	USER_DataAnalysisProcess((char *)Usart2_RxBuff,2);
	////////////////
	
	
	atk_8266_quit_trans();//�˳�͸��
	
	printf("send:AT+CIPCLOSE\r\n");	
	WiFi_SendCmd("AT+CIPCLOSE",50);         //�ر�����
	return 0;
}





void USER_DataAnalysisProcess(char *RxBuf,u8 day)
{ 
	if(day==1)
	{
      //��ȡ���ݲ��д�			
			weather1=USER_GetTempJsonValue((char *)RxBuf, (char *)"code_day");			
			high_temp1=USER_GetTempJsonValue((char *)RxBuf, (char *)"high");	
			low_temp1=USER_GetTempJsonValue((char *)RxBuf, (char *)"low");	
				
			printf("weather1: %d\r\n",weather1);
			printf("high_temp1: %d\r\n",high_temp1);
			printf("low_temp1: %d\r\n",low_temp1);
	}
	else
	{
		//��ȡ���ݲ��д�			
			weather2=USER_GetTempJsonValue((char *)RxBuf, (char *)"code_day");			
			high_temp2=USER_GetTempJsonValue((char *)RxBuf, (char *)"high");	
			low_temp2=USER_GetTempJsonValue((char *)RxBuf, (char *)"low");	
				
			printf("weather2: %d\r\n",weather1);
			printf("high_temp2: %d\r\n",high_temp1);
			printf("low_temp2: %d\r\n",low_temp1);
	}
			if(weather1>2)  
					{
						//printf("F_Error: GetL_Limit>GetH_Limit\r\n");
						
					}	
					else
					{
						
					}
}

/**********************USER_GetTempJsonValue*****************************/
uint8_t USER_GetTempJsonValue(char *cJson, char *Tag)
{
	char *target = NULL;
	static char temp[10];
	uint8_t len=0;
	uint8_t value=0;
	uint8_t i=0;

	memset(temp, 0x00, 128);
	sprintf(temp,"\"%s\":\"",Tag);
	target=strstr((const char *)cJson, (const char *)temp);
	if(target == NULL)
	{		
		return NULL;
	}
	i=strlen((const char *)temp);
	target=target+i;
	memset(temp, 0x00, 128);
	for(i=0; i<10; i++, target++)//��ֵ����10��λΪ�Ƿ�������2^32=4294967296
	{
		if((*target<='9')&&(*target>='0'))
		{
			temp[i]=*target;
		}
		else
		{
			break;
		}
	}
	temp[i+1] = '\0';
	len=strlen((const char *)temp);
	switch(len)
	{
		case(1):value=temp[0]-'0';break;
		case(2):value=temp[1]-'0'+(temp[0]-'0')*10;break;
		case(3):value=temp[2]-'0'+(temp[0]-'0')*100+(temp[1]-'0')*10;break;
		default:break;
	}
	return value;
}



//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
uint8_t atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return WiFi_SendCmd("AT",20);//�˳�͸���ж�.
}
