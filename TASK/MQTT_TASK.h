#ifndef __MQTT_TASK_H
#define __MQTT_TASK_H
#include "sys.h"
//传感器数据
typedef struct
{	
	int usering;
  int Vehconf1;//主卧空调
	int Vehconf2;//次卧空调
	int Vehconf3;//客厅空调
	int Vehconf4;//餐厅空调
	int Vehconf1_pattern;//主卧空调模式
	int Vehconf2_pattern;//次卧空调模式
	int Vehconf3_pattern;//客厅空调模式
	int Vehconf4_pattern;//餐厅空调模式
  int ledconf1;//主卧灯
	int ledconf2;//次卧灯
	int ledconf3;//客t灯
	int ledconf4;//餐厅灯
	int ledconf1_2;//主卧灯2
	int ledconf2_2;//次卧灯2
	int ledconf3_2;//客t灯2
	int ledconf4_2;//餐厅灯2
	int fan;//风扇
	int curtain;//窗帘
}User_Data_t;


void TempHumi_State(void);    
void USER_GetSteeringJsonValue(char *cJson, char *Tag);
void Mqtt_Task(void);
void Handle_TxDataBuff(void);
void WIFI_init(void);
void Handle_RxDataBuff(void);
void Handle_CMDBuf(User_Data_t *User_Data);
void ReconnectSever(void);
int Parsestring(char *AData);
#endif 





