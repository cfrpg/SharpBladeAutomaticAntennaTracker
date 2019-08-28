#include "gps.h"

u8 gpsBuff[2][256];
u8 gpsBuffLen[2];
u8 gpsCurrBuff;
u8 gpsRecState;
u8 gpsReady;

void GPSInit(u16 baud)
{
	GPIO_InitTypeDef gi;
	USART_InitTypeDef ui;	
	DMA_InitTypeDef di;
	NVIC_InitTypeDef ni;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);

	
	gi.GPIO_Pin = GPIO_Pin_12;
	gi.GPIO_Mode = GPIO_Mode_AF_PP;
	gi.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC,&gi);
	gi.GPIO_Pin=GPIO_Pin_2;
	gi.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&gi);

	ui.USART_BaudRate = baud;
	ui.USART_WordLength = USART_WordLength_8b;
	ui.USART_StopBits = USART_StopBits_1;
	ui.USART_Parity = USART_Parity_No;
	ui.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	ui.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &ui);
	
	USART_Cmd(UART5, ENABLE);
	
	//ni.NVIC_IRQChannel=UART5_IRQn;
	ni.NVIC_IRQChannelPreemptionPriority=2;
	ni.NVIC_IRQChannelSubPriority=1;
	ni.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&ni);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	gpsCurrBuff=0;
	gpsRecState=0;
	gpsBuffLen[0]=0;
	gpsBuffLen[1]=0;
	gpsReady=0;
}

void UART5_IRQHandler(void)
{
	u8 res;
	u8 curr;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		printf("rx\r\n");
		res =USART_ReceiveData(UART5);
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);		
		curr=gpsCurrBuff^1;
		switch(gpsRecState)
		{
			case 0:
				if(res=='$')
				{
					gpsRecState=1;
					gpsBuffLen[curr]=0;
				}
			break;
			case 1:
				gpsBuff[curr][gpsBuffLen[curr]++]=res;
				if(res=='\r')
					gpsRecState++;
			break;
			case 2:
				gpsBuff[curr][gpsBuffLen[curr]++]=res;
				if(res=='\n')
				{
					gpsBuff[curr][gpsBuffLen[curr]++]=0;
					gpsCurrBuff=curr;
					gpsReady=1;
					gpsRecState=0;
				}
			break;
		}
    } 
} 