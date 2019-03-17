#include "sys.h"
#include "mavlink.h"
#include "mavlinkCRC.h"

u8 mavlinkBuff[2][263];
u8 mavlinkCurr;
s16 mavlinkLen[2];
u8 mavlinkReady;
u8 mav_state;
u16 crc;
u16 tgtcrc;

u8 pkglen,fun;

void MavlinkInit(u32 baud)
{		
	GPIO_InitTypeDef gi;
	USART_InitTypeDef ui;
	NVIC_InitTypeDef ni;
	
	mav_state=0;
	mavlinkCurr=0;
	mavlinkLen[0]=0;
	mavlinkReady=0;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
 	USART_DeInit(USART3);	
    gi.GPIO_Pin = GPIO_Pin_10;
    gi.GPIO_Speed = GPIO_Speed_50MHz;
    gi.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gi);    
    gi.GPIO_Pin = GPIO_Pin_11;
    gi.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gi);
  
    ni.NVIC_IRQChannel = USART3_IRQn;
	ni.NVIC_IRQChannelPreemptionPriority=2 ;
	ni.NVIC_IRQChannelSubPriority = 3;
	ni.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&ni);

	ui.USART_BaudRate = baud;
	ui.USART_WordLength = USART_WordLength_8b;
	ui.USART_StopBits = USART_StopBits_1;
	ui.USART_Parity = USART_Parity_No;
	ui.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	ui.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &ui);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
    USART_Cmd(USART3, ENABLE);
}

void addCRC(u8 t)
{
	u8 ch;
	ch=(u8)(t^(u8)(crc&0x00FF));
	ch=(u8)(ch^(ch<<4));
	crc=(u16)((crc>>8)^(ch<<8)^(ch<<3)^(ch>>4));
}

void USART3_IRQHandler(void)
{	
	u8 b;
	u8 curr=mavlinkCurr^1;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		
		b=USART_ReceiveData(USART3);
		if(mav_state!=0)
		{			
			if(mav_state<7)
				addCRC(b);
			mavlinkBuff[curr][mavlinkLen[curr]]=b;
			mavlinkLen[curr]++;
		}
		switch(mav_state)
		{
			case 0://get STX
				if(b==0xFE)
				{
					mavlinkBuff[curr][0]=b;
					crc=0xFFFF;
					mavlinkLen[curr]=1;
					mav_state=1;
				}
				break;
			case 1://get Len				
				pkglen=b;				
				mav_state=2;
				break;
			case 2://SEQ
				mav_state=3;			
				break;
			case 3://SYS ID				
				mav_state=4;
				break;
			case 4://COMP ID				
				mav_state=5;
				break;
			case 5://MSG ID				
				fun=b;
				mav_state=6;
				break;
			case 6://DATA
				if(mavlinkLen[curr]==pkglen+6)
				{
					mav_state=7;
					addCRC(MavmsgCRC[fun]);					
				}
				break;
			case 7://SUM 1
				tgtcrc=b;
				mav_state=8;
				break;
			case 8://SUM 2
				tgtcrc|=((u16)b)<<8;
				if(tgtcrc==crc)
				{
					mavlinkCurr^=1;
					mavlinkReady=1;
				}
				mav_state=0;
				break;
			case 9:
				
				break;			
		}
				
		
	}
}

