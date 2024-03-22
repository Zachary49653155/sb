

#include "tim.h"


//��ʱ��ʱ����㹫ʽTout = ((��װ��ֵ+1)*(Ԥ��Ƶϵ��+1))/ʱ��Ƶ��;
//���磺1�붨ʱ����װ��ֵ=9999��Ԥ��Ƶϵ��=7199

//void TIM3_Init(u16 arr,u16 psc){  //TIM3 ��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
//    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
//    
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3
//    TIM3_NVIC_Init (); //����TIM3�ж�����
//	      
//    TIM_TimeBaseInitStrue.TIM_Period=arr; //�����Զ���װ��ֵ
//    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ��
//    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //�������������
//    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
//    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3��ʼ������
//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ��TIM3�ж�    
//    TIM_Cmd(TIM3,ENABLE); //ʹ��TIM3
//}
//
//void TIM3_NVIC_Init (void){ //����TIM3�ж�����
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//������ռ�������ȼ�
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

//������ʱ��
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        GPIOA->ODR=0<<15; //0 ���͵�ƽ
        TIM2->CNT = 0;
        TIM_Cmd(TIM2,DISABLE);	//ʧ��
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
        GPIOA->ODR=1<<15; //PA15�˿ڵĵ�ƽ״̬  1����
        TIM_Cmd(TIM2,ENABLE);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
    }
}

