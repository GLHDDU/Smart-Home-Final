/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            操作602Wifi功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "wifi.h"	    //包含需要的头文件
#include "key.h"
#include "delay.h"	    //包含需要的头文件
#include "usart1.h"	    //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "mqtt.h"       //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
extern unsigned int Usart2_RxCounter; 
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE];
#define WEATHER_PORTNUM 	"80"		//天气连接端口号:80	
#define WEATHER_SERVERIP 	"api.seniverse.com"		//天气服务器IP
char wifi_mode = 0;     //联网模式 0：SSID和密码写在程序里   1：Smartconfig方式用APP发送
u8 weather1,high_temp1,low_temp1,weather2,high_temp2,low_temp2;

/*-------------------------------------------------*/
/*函数名：初始化WiFi的复位IO                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //定义一个设置IO端口参数的结构体
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);   //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //准备设置PA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //推免输出方式
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //设置PA4
	RESET_IO(1);                                              //复位IO拉高电平
}
/*-------------------------------------------------*/
/*函数名：WiFi发送设置指令                         */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	WiFi_printf("%s\r\n",cmd);                  //发送指令
	while(timeout--){                           //等待超时时间到0
		delay_ms(100);                          //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		u1_printf("%d ",timeout);               //串口输出现在的超时时间
	}
	u1_printf("\r\n");                          //串口输出信息
	if(timeout<=0)return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}
/*-------------------------------------------------*/
/*函数名：WiFi复位                                 */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
	RESET_IO(0);                                    //复位IO拉低电平
	delay_ms(500);                                  //延时500ms
	RESET_IO(1);                                    //复位IO拉高电平	
	while(timeout--){                               //等待超时时间到0
		delay_ms(100);                              //延时100ms
		if(strstr(WiFi_RX_BUF,"ready"))             //如果接收到ready表示复位成功
			break;       						    //主动跳出while循环
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	u1_printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到ready，返回1
	else return 0;		         				    //反之，表示正确，说明收到ready，通过break主动跳出while
}
/*-------------------------------------------------*/
/*函数名：WiFi加入路由器指令                       */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS); //发送指令	
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(WiFi_RX_BUF,"\n\r\nOK")) //如果接收到WIFI GOT IP表示成功，但是正点原子输出形式是		WIFI GOT IP\n\r\nOK，老版本输出形式是WIFI GOT IP\r\n\r\nOK，为了通用，此处这样设置
			break;       						    //主动跳出while循环
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	u1_printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}
/*-------------------------------------------------*/
/*函数名：连接TCP服务器，并进入透传模式            */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{	
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区   
	WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ServerIP,ServerPort);//发送连接服务器指令
	while(timeout--){                               //等待超时与否
		delay_ms(100);                              //延时100ms	
		if(strstr(WiFi_RX_BUF ,"CONNECT"))          //如果接受到CONNECT表示连接成功
			break;                                  //跳出while循环
		if(strstr(WiFi_RX_BUF ,"CLOSED"))           //如果接受到CLOSED表示服务器未开启
			return 1;                               //服务器未开启返回1
		if(strstr(WiFi_RX_BUF ,"ALREADY CONNECTED"))//如果接受到ALREADY CONNECTED已经建立连接
			return 2;                               //已经建立连接返回2
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间  
	}
	u1_printf("\r\n");                        //串口输出信息
	if(timeout<=0)return 3;                   //超时错误，返回3
	else                                      //连接成功，准备进入透传
	{
		u1_printf("连接服务器成功，准备进入透传\r\n");  //串口显示信息
		WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区     
		WiFi_printf("AT+CIPSEND\r\n");                  //发送进入透传指令
		while(timeout--){                               //等待超时与否
			delay_ms(100);                              //延时100ms	
			if(strstr(WiFi_RX_BUF,"\r\nOK\r\n\r\n>"))   //如果成立表示进入透传成功
				break;                          //跳出while循环
			u1_printf("%d ",timeout);           //串口输出现在的超时时间  
		}
		if(timeout<=0)return 4;                 //透传超时错误，返回4	
	}
	return 0;	                                //成功返回0	
}
/*-------------------------------------------------*/
/*函数名：WiFi_Smartconfig                         */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Smartconfig(int timeout)
{
	
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区     
	while(timeout--){                           //等待超时时间到0
		delay_ms(1000);                         //延时1s
		if(strstr(WiFi_RX_BUF,"connected"))     //如果串口接受到connected表示成功
			break;                              //跳出while循环  
		u1_printf("%d ",timeout);               //串口输出现在的超时时间  
	}	
	u1_printf("\r\n");                          //串口输出信息
	if(timeout<=0)return 1;                     //超时错误，返回1
	return 0;                                   //正确返回0
}
/*-------------------------------------------------*/
/*函数名：等待加入路由器                           */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_WaitAP(int timeout)
{		
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP"))       //如果接收到WIFI GOT IP表示成功
			break;       						    //主动跳出while循环
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	u1_printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}
/*-------------------------------------------------*/
/*函数名：WiFi连接服务器                           */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{	
	u1_printf("准备复位模块\r\n");                     //串口提示数据
	if(WiFi_Reset(50)){                                //复位，100ms超时单位，总计5s超时时间
		u1_printf("复位失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
		return 1;                                      //返回1
	}else u1_printf("复位成功\r\n");                   //串口提示数据
	
	u1_printf("准备设置STA模式\r\n");                  //串口提示数据
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //设置STA模式，100ms超时单位，总计5s超时时间
		u1_printf("设置STA模式失败，准备重启\r\n");    //返回非0值，进入if，串口提示数据
		return 2;                                      //返回2
	}else u1_printf("设置STA模式成功\r\n");            //串口提示数据
	
	if(wifi_mode==0){                                      //如果联网模式=0：SSID和密码写在程序里 
		u1_printf("准备取消自动连接\r\n");                 //串口提示数据
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //取消自动连接，100ms超时单位，总计5s超时时间
			u1_printf("取消自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
			return 3;                                      //返回3
		}else u1_printf("取消自动连接成功\r\n");           //串口提示数据
				
		u1_printf("准备连接路由器\r\n");                   //串口提示数据	
		if(WiFi_JoinAP(30)){                               //连接路由器,1s超时单位，总计30s超时时间
			u1_printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
			return 4;                                      //返回4	
		}else u1_printf("连接路由器成功\r\n");             //串口提示数据			
	}else{                                                 //如果联网模式=1：Smartconfig方式,用APP发送
		if(wifi_mode==1){                                    //如果此时K2是按下的
			u1_printf("准备设置自动连接\r\n");                 //串口提示数据
			if(WiFi_SendCmd("AT+CWAUTOCONN=1",50)){            //设置自动连接，100ms超时单位，总计5s超时时间
				u1_printf("设置自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
				return 3;                                      //返回3
			}else u1_printf("设置自动连接成功\r\n");           //串口提示数据	
			
			u1_printf("准备开启Smartconfig\r\n");              //串口提示数据
			if(WiFi_SendCmd("AT+CWSTARTSMART",50)){            //开启Smartconfig，100ms超时单位，总计5s超时时间
				u1_printf("开启Smartconfig失败，准备重启\r\n");//返回非0值，进入if，串口提示数据
				return 4;                                      //返回4
			}else u1_printf("开启Smartconfig成功\r\n");        //串口提示数据

			u1_printf("请使用APP软件传输密码\r\n");            //串口提示数据
			if(WiFi_Smartconfig(60)){                          //APP软件传输密码，1s超时单位，总计60s超时时间
				u1_printf("传输密码失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
				return 5;                                      //返回5
			}else u1_printf("传输密码成功\r\n");               //串口提示数据

			u1_printf("准备关闭Smartconfig\r\n");              //串口提示数据
			if(WiFi_SendCmd("AT+CWSTOPSMART",50)){             //关闭Smartconfig，100ms超时单位，总计5s超时时间
				u1_printf("关闭Smartconfig失败，准备重启\r\n");//返回非0值，进入if，串口提示数据
				return 6;                                      //返回6
			}else u1_printf("关闭Smartconfig成功\r\n");        //串口提示数据
		}else{                                                 //反之，此时K2是没有按下
			u1_printf("等待连接路由器\r\n");                   //串口提示数据	
			if(WiFi_WaitAP(30)){                               //等待连接路由器,1s超时单位，总计30s超时时间
				u1_printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
				return 7;                                      //返回7	
			}else u1_printf("连接路由器成功\r\n");             //串口提示数据					
		}
	}
	
	u1_printf("准备设置透传\r\n");                     //串口提示数据
	if(WiFi_SendCmd("AT+CIPMODE=1",50)){               //设置透传，100ms超时单位，总计5s超时时间
		u1_printf("设置透传失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
		return 8;                                      //返回8
	}else u1_printf("设置透传成功\r\n");               //串口提示数据
	
	u1_printf("准备关闭多路连接\r\n");                 //串口提示数据
	if(WiFi_SendCmd("AT+CIPMUX=0",50)){                //关闭多路连接，100ms超时单位，总计5s超时时间
		u1_printf("关闭多路连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
		return 9;                                      //返回9
	}else u1_printf("关闭多路连接成功\r\n");           //串口提示数据
	
	u1_printf("准备连接服务器\r\n");                   //串口提示数据
	if(WiFi_Connect_Server(100)){                      //连接服务器，100ms超时单位，总计10s超时时间
		u1_printf("连接服务器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
		return 10;                                     //返回10
	}else u1_printf("连接服务器成功\r\n");             //串口提示数据	
	
	return 0;                                          //正确返回0
	
}


/*-------------------------------------------------*/
/*函数名：WiFi连接服务器                           */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char get_current_weather(void)
{	
	printf("准备复位模块\r\n");                     //串口提示数据
	if(WiFi_Reset(50)){                                //复位，100ms超时单位，总计5s超时时间
		printf("复位失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
		return 1;                                      //返回1
	}else printf("复位成功\r\n");                   //串口提示数据
	
	printf("准备设置STA模式\r\n");                  //串口提示数据
	if(WiFi_SendCmd("AT+CWMODE=1",50)){                //设置STA模式，100ms超时单位，总计5s超时时间
		printf("设置STA模式失败，准备重启\r\n");    //返回非0值，进入if，串口提示数据
		return 2;                                      //返回2
	}else printf("设置STA模式成功\r\n");            //串口提示数据
	
	if(wifi_mode==0){                                      //如果联网模式=0：SSID和密码写在程序里 
		printf("准备取消自动连接\r\n");                 //串口提示数据
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //取消自动连接，100ms超时单位，总计5s超时时间
			printf("取消自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
			return 3;                                      //返回3
		}else printf("取消自动连接成功\r\n");           //串口提示数据
				
		printf("准备连接路由器\r\n");                   //串口提示数据	
		if(WiFi_JoinAP(30)){                               //连接路由器,1s超时单位，总计30s超时时间
			printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
			return 4;                                      //返回4	
		}else printf("连接路由器成功\r\n");             //串口提示数据			
	}else{                                                 //如果联网模式=1：Smartconfig方式,用APP发送
		if(KEY0==0){                                    //如果此时K0是按下的
			printf("准备设置自动连接\r\n");                 //串口提示数据
			if(WiFi_SendCmd("AT+CWAUTOCONN=1",50)){            //设置自动连接，100ms超时单位，总计5s超时时间
				printf("设置自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
				return 3;                                      //返回3
			}else printf("设置自动连接成功\r\n");           //串口提示数据	
			
			printf("准备开启Smartconfig\r\n");              //串口提示数据
			if(WiFi_SendCmd("AT+CWSTARTSMART",50)){            //开启Smartconfig，100ms超时单位，总计5s超时时间
				printf("开启Smartconfig失败，准备重启\r\n");//返回非0值，进入if，串口提示数据
				return 4;                                      //返回4
			}else printf("开启Smartconfig成功\r\n");        //串口提示数据

			printf("请使用APP软件传输密码\r\n");            //串口提示数据
			if(WiFi_Smartconfig(60)){                          //APP软件传输密码，1s超时单位，总计60s超时时间
				printf("传输密码失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
				return 5;                                      //返回5
			}else printf("传输密码成功\r\n");               //串口提示数据

			printf("准备关闭Smartconfig\r\n");              //串口提示数据
			if(WiFi_SendCmd("AT+CWSTOPSMART",50)){             //关闭Smartconfig，100ms超时单位，总计5s超时时间
				printf("关闭Smartconfig失败，准备重启\r\n");//返回非0值，进入if，串口提示数据
				return 6;                                      //返回6
			}else printf("关闭Smartconfig成功\r\n");        //串口提示数据
		}else{                                                 //反之，此时K2是没有按下
			printf("等待连接路由器\r\n");                   //串口提示数据	
			if(WiFi_WaitAP(30)){                               //等待连接路由器,1s超时单位，总计30s超时时间
				printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
				return 7;                                      //返回7	
			}else printf("连接路由器成功\r\n");             //串口提示数据					
		}
	}
	
	printf("准备设置透传\r\n");                     //串口提示数据
	if(WiFi_SendCmd("AT+CIPMODE=1",50)){               //设置透传，100ms超时单位，总计5s超时时间
		printf("设置透传失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
		return 8;                                      //返回8
	}else printf("设置透传成功\r\n");               //串口提示数据
	
	printf("准备关闭多路连接\r\n");                 //串口提示数据
	if(WiFi_SendCmd("AT+CIPMUX=0",50)){                //关闭多路连接，100ms超时单位，总计5s超时时间
		printf("关闭多路连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
		return 9;                                      //返回9
	}else printf("关闭多路连接成功\r\n");           //串口提示数据
	//////////*******准备获取天气数据**********////////////////
	printf("准备获取天气数据\r\n");                 //串口提示数据
	if(get_weather()){                //关闭多路连接，100ms超时单位，总计5s超时时间
		printf("获取天气数据失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
		return 9;                                      //返回9
	}else printf("获取天气数据成功\r\n");           //串口提示数据
	//////////////////////////////////////////
	return 0;                                          //正确返回0
	
}

//获取一次实时天气
//返回：0---获取成功，1---获取失败
char get_weather(void)
{

	
	char temp[256];
	sprintf(temp,"AT+CIPSTART=\"TCP\",\"%s\",%s",WEATHER_SERVERIP,WEATHER_PORTNUM);    //配置目标TCP服务器
	printf("send:AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",WEATHER_SERVERIP,WEATHER_PORTNUM);
	WiFi_SendCmd(temp,50);

	delay_ms(300);
	
	printf("send:AT+CIPMODE=1\r\n");	
	WiFi_SendCmd("AT+CIPMODE=1",50);      //传输模式为：透传	

	Usart2_RxCounter=0;
	printf("send:AT+CIPSEND\r\n");	
	WiFi_SendCmd("AT+CIPSEND",50);         //开始透传
	printf("start trans...\r\n");
	//当前
	//u2_printf("GET https://api.seniverse.com/v3/weather/now.json?key=St4bOhFvShcUJhU4_&location=hengyang&language=en&unit=c\n\n");	
	//今日开始3天
	u2_printf("GET  https://api.seniverse.com/v3/weather/daily.json?key=SF186LL7teJi5Gofo&location=hengyang&language=en&unit=c&start=0&days=5\n\n");	
	//明日开始2天
	//u2_printf("GET  https://api.seniverse.com/v3/weather/daily.json?key=SF186LL7teJi5Gofo&location=hengyang&language=en&unit=c&start=1&days=5\n\n");	

	delay_ms(20);//延时20ms返回的是指令发送成功的状态
	Usart2_RxCounter=0;	//清零串口3数据
	delay_ms(1000);
	if(Usart2_RxCounter&0X8000)		//此时再次接到一次数据，为天气的数据
	{ 
		Usart2_RxBuff[Usart2_RxCounter&0X7FFF]=0;//添加结束符
	} 
	printf("USART2_RX_BUF=%s\r\n",Usart2_RxBuff);
	USER_DataAnalysisProcess((char *)Usart2_RxBuff,1);
	///////获取第二天天气//////////
	u2_printf("GET  https://api.seniverse.com/v3/weather/daily.json?key=SF186LL7teJi5Gofo&location=hengyang&language=en&unit=c&start=1&days=5\n\n");	

	delay_ms(20);//延时20ms返回的是指令发送成功的状态
	Usart2_RxCounter=0;	//清零串口3数据
	delay_ms(1000);
	if(Usart2_RxCounter&0X8000)		//此时再次接到一次数据，为天气的数据
	{ 
		Usart2_RxBuff[Usart2_RxCounter&0X7FFF]=0;//添加结束符
	} 
	printf("USART2_RX_BUF=%s\r\n",Usart2_RxBuff);
	USER_DataAnalysisProcess((char *)Usart2_RxBuff,2);
	////////////////
	
	
	atk_8266_quit_trans();//退出透传
	
	printf("send:AT+CIPCLOSE\r\n");	
	WiFi_SendCmd("AT+CIPCLOSE",50);         //关闭连接
	return 0;
}





void USER_DataAnalysisProcess(char *RxBuf,u8 day)
{ 
	if(day==1)
	{
      //提取数据并判错			
			weather1=USER_GetTempJsonValue((char *)RxBuf, (char *)"code_day");			
			high_temp1=USER_GetTempJsonValue((char *)RxBuf, (char *)"high");	
			low_temp1=USER_GetTempJsonValue((char *)RxBuf, (char *)"low");	
				
			printf("weather1: %d\r\n",weather1);
			printf("high_temp1: %d\r\n",high_temp1);
			printf("low_temp1: %d\r\n",low_temp1);
	}
	else
	{
		//提取数据并判错			
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
	for(i=0; i<10; i++, target++)//数值超过10个位为非法，由于2^32=4294967296
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



//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
uint8_t atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(500);					//等待500ms
	return WiFi_SendCmd("AT",20);//退出透传判断.
}
