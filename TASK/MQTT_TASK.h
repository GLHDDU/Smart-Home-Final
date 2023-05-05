#ifndef __MQTT_TASK_H
#define __MQTT_TASK_H
#include "sys.h"
//����������
typedef struct
{	
	int usering;
  int Vehconf1;//���Կյ�
	int Vehconf2;//���Կյ�
	int Vehconf3;//�����յ�
	int Vehconf4;//�����յ�
	int Vehconf1_pattern;//���Կյ�ģʽ
	int Vehconf2_pattern;//���Կյ�ģʽ
	int Vehconf3_pattern;//�����յ�ģʽ
	int Vehconf4_pattern;//�����յ�ģʽ
  int ledconf1;//���Ե�
	int ledconf2;//���Ե�
	int ledconf3;//��t��
	int ledconf4;//������
	int ledconf1_2;//���Ե�2
	int ledconf2_2;//���Ե�2
	int ledconf3_2;//��t��2
	int ledconf4_2;//������2
	int fan;//����
	int curtain;//����
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





