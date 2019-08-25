#include "keyboard.h"

s32 value;

void KeyInit(void)
{
	GPIO_InitTypeDef gi;
	EXTI_InitTypeDef ei;
	NVIC_InitTypeDef ni;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	gi.GPIO_Pin=GPIO_Pin_9;
	gi.GPIO_Mode=GPIO_Mode_Out_PP;
	gi.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gi);
	gi.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&gi);
	
	gi.GPIO_Pin=GPIO_Pin_8;
	gi.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gi.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&gi);
	gi.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOA,&gi);
	
	gi.GPIO_Pin=GPIO_Pin_11;
	gi.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&gi);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	ei.EXTI_Line=EXTI_Line11;
	ei.EXTI_Mode=EXTI_Mode_Interrupt;
	ei.EXTI_Trigger=EXTI_Trigger_Falling;
	ei.EXTI_LineCmd=ENABLE;
	EXTI_Init(&ei);
	
	ni.NVIC_IRQChannel=EXTI15_10_IRQn;
	ni.NVIC_IRQChannelCmd=ENABLE;
	ni.NVIC_IRQChannelPreemptionPriority=2;
	ni.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&ni);
	
	GPSHIFT=0;
	value=0;
}

u8 GPGetData()
{
	u8 res,i;	
	GPCLK=0;
	res=0;	
	GPSHIFT=0;
	res=0;
	GPSHIFT=1;//锁存	
	for(i=0;i<8;i++)
	{			
		res<<=1;
		GPCLK=0;
		res|=GPQ;
		GPCLK=1;//上升沿移位	
	}
	GPSHIFT=0;
	GPCLK=0;
	//printf("key %d\r\n",res);
	return res;
}

void EXTI15_10_IRQHandler(void)
{
	if(WHEEL)
	{
		value++;
	}
	else
	{
		value--;
	}
	EXTI_ClearITPendingBit(EXTI_Line11);
}

s32 WheelGetValue(void)
{
	return value;
}
