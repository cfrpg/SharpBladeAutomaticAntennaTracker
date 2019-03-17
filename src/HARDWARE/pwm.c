#include "pwm.h"
#include "parameter.h"

void PWMInit(void)
{
	GPIO_InitTypeDef gi;
	TIM_TimeBaseInitTypeDef  ti;
	TIM_OCInitTypeDef  to;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	   
	gi.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	gi.GPIO_Mode = GPIO_Mode_AF_PP;
	gi.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gi);
 
	ti.TIM_Period = 19999;
	ti.TIM_Prescaler =71;
	ti.TIM_ClockDivision = 0;
	ti.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &ti);
	
	to.TIM_OCMode = TIM_OCMode_PWM2;
 	to.TIM_OutputState = TIM_OutputState_Enable;
	to.TIM_Pulse=0;
	to.TIM_OCPolarity = TIM_OCPolarity_Low;
	to.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OC3Init(TIM4, &to);
	TIM_OC4Init(TIM4, &to);

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
	TIM_Cmd(TIM4, ENABLE);
	PWMSet(0,0);
}

void PWMSet(float yaw,float pitch)
{
	s16 t;
	if(yaw>params.yaw_range)
		yaw=params.yaw_range;
	if(yaw<-params.yaw_range)
		yaw=-params.yaw_range;
	if(pitch<0)
		pitch=0;
	if(pitch>params.pitch_range)
		pitch=params.pitch_range;
	
	if(params.pwm_yaw_rev)
		yaw=-yaw;
	
	t=params.pwm_yaw_center;
	if(yaw>0)
	{
		t=(s16)((params.pwm_yaw_max-params.pwm_yaw_center)*(yaw/params.yaw_range));
	}
	else
	{
		t=(s16)((params.pwm_yaw_center-params.pwm_yaw_min)*(yaw/params.yaw_range));
	}
	
	t+=params.pwm_yaw_center;
	
	TIM_SetCompare3(TIM4,t);
	t=(s16)((params.pwm_pitch_max-params.pwm_pitch_min)*(pitch/params.pitch_range));
	//printf("%d ",t);
	if(params.pwm_pitch_rev)
	{
		t=params.pwm_pitch_max-t;
	}
	else
	{
		t=t+params.pwm_pitch_min;
	}
		
	//printf("%d\r\n",t);
	TIM_SetCompare4(TIM4,t);
		
}