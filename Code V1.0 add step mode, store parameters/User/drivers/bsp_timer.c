/**
  ******************************************************************************
  * @file    bsp_timer.c
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/14 ������,Administrator
  * @brief  This file
  *
  * @verbatim  
  *     
  ******************************************************************************
  * @attention
  *     
  *     
  *
  * <h2><center>&copy; COPYRIGHT 2015 WuLianGroup</center></h2>
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "bsp_timer.h"
#include "main.h"
#include <stdio.h>

#include "bsp_usart.h"
#include "bsp_nearfield.h"

#define TMR_COUNT   9            /* �����ʱ����������һ��������DelayMs()ʹ��*/
         
/* Private variables ---------------------------------------------------------*/ 
SOFT_TMR g_Tmr[TMR_COUNT];
uint32 g_iRunTime = 0;

uint8 LineNumber = 0;   //�ڶ�ʱ����ˢ��
uint16 TimeCount = 0;   //confirm timer
uint16 BeepTime = 0;    //used to be delay time
uint16 DelaySettingTime = 0;    //used to be delay time
uint8 TimeOutFlag = 0;

/**
  * @fun    void Timer2_Init
  * @brief  
  * @author huangzibo
  * @param  uint16 arr ��ʱ����װֵ
  * @param  uint16 psc ʱ��Ԥ��Ƶ��
  * @���ö�ʱ��2(TIM2)Ϊ���¼���ģʽ
  * @PCLK = 72000KHz
  * @��ʱ������Ƶ�� = PCLK / ( psc + 1 )
  * @��ʱ���ж����� = ( arr + 1 )*( pac + 1) / PCLK
  * @retval 
  */
void Timer2_Init(uint16 arr, uint16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          //ʹ������ʱ��
  
  TIM_TimeBaseStructure.TIM_Period = arr;		                //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
  TIM_TimeBaseStructure.TIM_Prescaler =psc;	                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���¼���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			    //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ARRPreloadConfig(TIM2,ENABLE);                            //ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�
  
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );	                    //��������ж�
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//TIM2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	    //��ռ���ȼ�2��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    		//�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);	                            //��ʼ��NVIC�Ĵ���
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  
  TIM_Cmd(TIM2, DISABLE);  
}

/**
  * @fun    void Timer3_Init
  * @brief  
  * @author huangzibo
  * @param  uint16 arr ��ʱ����װֵ
  * @param  uint16 psc ʱ��Ԥ��Ƶ��
  * @���ö�ʱ��3(TIM3)Ϊ���¼���ģʽ
  * @PCLK = 72000KHz
  * @��ʱ������Ƶ�� = PCLK / ( psc + 1 )
  * @��ʱ���ж����� = ( arr + 1 )*( pac + 1) / PCLK
  * @retval 
  */
void Timer3_Init(uint16 arr, uint16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          //ʹ������ʱ��
  
  TIM_TimeBaseStructure.TIM_Period = arr;		                //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
  TIM_TimeBaseStructure.TIM_Prescaler =psc;	                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���¼���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);			    //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ARRPreloadConfig(TIM3,ENABLE);                            //ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�
  
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );	                    //��������ж�
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//TIM3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	    //��ռ���ȼ�1��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    		//�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);	                            //��ʼ��NVIC�Ĵ���
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  
  TIM_Cmd(TIM3, ENABLE);  
}


/**
  * @fun    void Timer4_Init
  * @brief  ��ʱ��3��ʼ��
  * @author huangzibo
  * @param  uint16 arr
  * @param  uint16 psc
  * ����TIM4_CH3 PWM���
  * @��ʱ������Ƶ�� = PCLK / ( psc + 1 )
  * @��ʱ���ж����� = ( arr + 1 )*( pac + 1) / PCLK
  * @retval 
  */
void TIM4_PWM_Init(uint16 arr, uint16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOB��AFIO���ù���ʱ�� 
  
  //����PB8Ϊ�����������,���TIM4_CH3��PWM���岨��	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB8����ӦTIM4_CH3
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB8����ӦTIM4_CH1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
  
  //��ʼ��TIM4
  TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�
  
  //��ʼ��TIM4_CH3 PWM���
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //�����ò�����ʼ������TIM4 OC3
  
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
  
  TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}


/**
  * @fun    void TIM2_IRQHandler
  * @brief  ��ʱ��2�жϴ�����
  * @author huangzibo
  * @param  None
  *
  * @retval
  */
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	    //���TIM2�����жϷ������
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 	 		//���TIM2�����жϱ�־  
  
    LineNumber++;
    TimeCount++;
      
  }
}


/**
  * @fun    void TIM3_IRQHandler
  * @brief  ��ʱ��3�жϴ�����
  * @author huangzibo
  * @param  None
  *
  * @retval
  */
void TIM3_IRQHandler(void)  
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)    //���TIM3�����жϷ������
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIM3�����жϱ�־

    BeepTime++;      

    if(BeepTime == DelaySettingTime)
    {      
      TimeOutFlag = 1;
    }
    if(BeepTime > 50000)
    {
      BeepTime--;
    }  
  }
}

/**
* @fun    void bsp_SoftTimerDec
* @brief  ÿ��1ms�����ж�ʱ��������1
* @author huangzibo
* @param  SOFT_TMR *_tmr
*
* @retval 
*/
void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
  if (_tmr->flag == 0)
  {
    if (_tmr->count > 0)
    {
      /* �����ʱ����������1�����ö�ʱ�������־ */
      if (--_tmr->count == 0)
      {
        _tmr->flag = 1;
      }
    }
  }
}


/**
* @fun    void SysTick_ISR
* @brief  SysTick�жϷ������ÿ��1ms����1��
* @author huangzibo
* @param  None
*
* @retval 
*/
void SysTick_ISR(void)
{
  uint8 i;
  
  for (i = 0; i < TMR_COUNT; i++)
  {
    bsp_SoftTimerDec(&g_Tmr[i]);
  }
  
  g_iRunTime++;	/* ȫ������ʱ��ÿ1ms��1 */	
  if (g_iRunTime == 0x7FFFFFFF)
  {
    g_iRunTime = 0;
  }
}

/**
* @fun    void SysTick_Handler
* @brief  �δ�ʱ���жϴ�����
* @author huangzibo
* @param  None
*
* @retval 
*/
void SysTick_Handler(void)
{
  SysTick_ISR();
}

/**
* @fun    void bsp_InitTimer
* @brief  ��ʼ�������ʱ��
* @author huangzibo
* @param  None
*
* @retval 
*/
void bsp_InitTimer(void)
{
  uint8 i;
  for(i = 0;i<TMR_COUNT;i++)
  {
    g_Tmr[i].count = 0;
    g_Tmr[i].flag = 0;
  }
  SysTick_Config(SystemCoreClock / 1000);/* �����ж�����1ms���������ж� */
  
}


/**
* @fun    void DelayMs
* @brief  ms���ӳ٣��ӳپ���Ϊ����1ms
* @author huangzibo
* @param  uint32  n
*
* @retval 
*/
void DelayMs(uint32  n)
{
  /* ���� n = 1 �������������� */
  if (n <= 1)
  {
    n = 2;
  }
  
  __set_PRIMASK(1);  		/* ���ж� */
  g_Tmr[0].count = n;
  g_Tmr[0].flag = 0;
  __set_PRIMASK(0);  		/* ���ж� */
  
  while (1)
  {
    CPU_IDLE();	/* �˴��ǿղ������û����Զ��壬��CPU����IDLE״̬���Խ��͹��ģ���ʵ��ι�� */
    
    /* �ȴ��ӳ�ʱ�䵽 */
    if (g_Tmr[0].flag == 1)
    {
      break;
    }
  }
}

void delay_us(uint32  n)
{
  /* ���� n = 1 �������������� */
  if (n <= 1)
  {
    n = 2;
  }
  
  __set_PRIMASK(1);  		/* ���ж� */
  g_Tmr[0].count = n;
  g_Tmr[0].flag = 0;
  __set_PRIMASK(0);  		/* ���ж� */
  
  while (1)
  {
    CPU_IDLE();	/* �˴��ǿղ������û����Զ��壬��CPU����IDLE״̬���Խ��͹��ģ���ʵ��ι�� */
    
    /* �ȴ��ӳ�ʱ�䵽 */
    if (g_Tmr[0].flag == 1)
    {
      break;
    }
  }
}
/**
* @fun    void bsp_StartTimer
* @brief  ����һ����ʱ��������������
* @author huangzibo
* @param  uint8 _id
* @param  uint32 _period
*
* @retval 
*/
void bsp_StartTimer(uint8 _id, uint32 _period)
{
  if (_id >= TMR_COUNT)
  {
    /* while(1); ���� */
    return;
  }
  
  __set_PRIMASK(1);  		/* ���ж� */
  g_Tmr[_id].count = _period;
  g_Tmr[_id].flag = 0;
  __set_PRIMASK(0);  		/* ���ж� */
}


/**
* @fun    void bsp_StopTimer
* @brief  ֹͣһ����ʱ��
* @author huangzibo
* @param  uint8 _id
*
* @retval 
*/
void bsp_StopTimer(uint8 _id)
{
  if (_id >= TMR_COUNT)
  {
    /* while(1); ���� */
    return;
  }
  
  __set_PRIMASK(1);  		/* ���ж� */
  g_Tmr[_id].count = 0;
  g_Tmr[_id].flag = 0;
  __set_PRIMASK(0);  		/* ���ж� */
}


/**
* @fun    uint8 bsp_CheckTimer
* @brief  ��ⶨʱ���Ƿ�ʱ
* @author huangzibo
* @param  uint8 _id
*
* @retval 
*/
uint8 bsp_CheckTimer(uint8 _id)
{
  if (_id >= TMR_COUNT)
  {
    return 0;
  }
  
  if (g_Tmr[_id].flag == 1)
  {
    g_Tmr[_id].flag = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @fun    int32  bsp_GetRunTime
* @brief  ��ȡCPU����ʱ�䣬��λ1ms
* @author huangzibo
* @param  None
*
* @retval 
*/
int32 bsp_GetRunTime(void)
{
  int runtime; 
  
  __set_PRIMASK(1);  		/* ���ж� */
  
  runtime = g_iRunTime;	/* ������Systick�жϱ���д����˹��жϽ��б��� */
  
  __set_PRIMASK(0);  		/* ���ж� */
  
  return runtime;
}

/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

