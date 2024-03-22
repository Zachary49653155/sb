

#include "tim.h"


//定时器时间计算公式Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
//例如：1秒定时，重装载值=9999，预分频系数=7199

//void TIM3_Init(u16 arr,u16 psc){  //TIM3 初始化 arr重装载值 psc预分频系数
//    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
//    
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
//    TIM3_NVIC_Init (); //开启TIM3中断向量
//	      
//    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
//    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
//    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
//    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
//    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3初始化设置
//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能TIM3中断    
//    TIM_Cmd(TIM3,ENABLE); //使能TIM3
//}
//
//void TIM3_NVIC_Init (void){ //开启TIM3中断向量
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//设置抢占和子优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

 void TIMER_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 500-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 20000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
	TIM_ITConfig(TIM2,TIM_IT_Update ,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

//两个定时器
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        GPIOA->ODR=0<<15; //0 拉低电平
        TIM2->CNT = 0;
        TIM_Cmd(TIM2,DISABLE);	//失能
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );
    }
}



  void TIM3_IRQHandler(void)
{
    static unsigned int times = 0;
    static unsigned char dir = 1;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(dir)
            times++;
        else
            times--;
        if(times == 300)
            dir = 0;
        if(times == 0)
            dir = 1;
        TIM2->ARR = 500+times*20/3 - 1;
        GPIOA->ODR=1<<15; //PA15端口的电平状态  1拉高
        TIM_Cmd(TIM2,ENABLE);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
    }
}

