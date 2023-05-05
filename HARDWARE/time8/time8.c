/**********************************************************
** 函数名: TIM6_Config
** 功能描述:  基本定时器配置
** 输入参数: 无
** 输出参数: 无
** 说明：定时时间=(预分频数+1）*（计数值+1) /TIM6时钟(72MHz)，单位(s)
   这里溢出时间t=(7200*10000)/72000000s=1s
***********************************************************/
#include "time8.h"
#include "stm32f10x.h"  //包含需要的头文件
void TIM6_Config(void)
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //使能TIM6时钟


/*基础设置*/
TIM_TimeBaseStructure.TIM_Period = 50000-1;//计数值10000   
TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    //预分频,此值+1为分频的除数
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //采样分频
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);


TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);     //使能TIM6中断
TIM_Cmd(TIM6, ENABLE);     //使能定时器6
}
/**********************************************************
** 函数名:NVIC_Config
** 功能描述:  中断优先级配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void NVIC_Config(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //采用组别2 
 
NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;//TIM6中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
NVIC_Init(&NVIC_InitStructure);//中断初始化
}



