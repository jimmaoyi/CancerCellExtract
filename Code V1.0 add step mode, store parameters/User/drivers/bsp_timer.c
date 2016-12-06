/**
  ******************************************************************************
  * @file    bsp_timer.c
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/14 星期四,Administrator
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

#define TMR_COUNT   9            /* 软件定时器个数，第一个保留给DelayMs()使用*/
         
/* Private variables ---------------------------------------------------------*/ 
SOFT_TMR g_Tmr[TMR_COUNT];
uint32 g_iRunTime = 0;

uint8 LineNumber = 0;   //在定时器中刷行
uint16 TimeCount = 0;   //confirm timer
uint16 BeepTime = 0;    //used to be delay time
uint16 DelaySettingTime = 0;    //used to be delay time
uint8 TimeOutFlag = 0;

/**
  * @fun    void Timer2_Init
  * @brief  
  * @author huangzibo
  * @param  uint16 arr 定时器重装值
  * @param  uint16 psc 时钟预分频数
  * @配置定时器2(TIM2)为向下计数模式
  * @PCLK = 72000KHz
  * @定时器计数频率 = PCLK / ( psc + 1 )
  * @定时器中断周期 = ( arr + 1 )*( pac + 1) / PCLK
  * @retval 
  */
void Timer2_Init(uint16 arr, uint16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          //使能外设时钟
  
  TIM_TimeBaseStructure.TIM_Period = arr;		                //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
  TIM_TimeBaseStructure.TIM_Prescaler =psc;	                    //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向下计数模式
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			    //根据指定的参数初始化TIMx的时间基数单位
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ARRPreloadConfig(TIM2,ENABLE);                            //使能ARR预装载，防止向上计数时更新事件异常延迟
  
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );	                    //允许更新中断
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//TIM2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	    //先占优先级2级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    		//从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);	                            //初始化NVIC寄存器
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  
  TIM_Cmd(TIM2, DISABLE);  
}

/**
  * @fun    void Timer3_Init
  * @brief  
  * @author huangzibo
  * @param  uint16 arr 定时器重装值
  * @param  uint16 psc 时钟预分频数
  * @配置定时器3(TIM3)为向下计数模式
  * @PCLK = 72000KHz
  * @定时器计数频率 = PCLK / ( psc + 1 )
  * @定时器中断周期 = ( arr + 1 )*( pac + 1) / PCLK
  * @retval 
  */
void Timer3_Init(uint16 arr, uint16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          //使能外设时钟
  
  TIM_TimeBaseStructure.TIM_Period = arr;		                //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
  TIM_TimeBaseStructure.TIM_Prescaler =psc;	                    //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向下计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);			    //根据指定的参数初始化TIMx的时间基数单位
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ARRPreloadConfig(TIM3,ENABLE);                            //使能ARR预装载，防止向上计数时更新事件异常延迟
  
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );	                    //允许更新中断
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	    //先占优先级1级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    		//从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);	                            //初始化NVIC寄存器
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  
  TIM_Cmd(TIM3, ENABLE);  
}


/**
  * @fun    void Timer4_Init
  * @brief  定时器3初始化
  * @author huangzibo
  * @param  uint16 arr
  * @param  uint16 psc
  * 配置TIM4_CH3 PWM输出
  * @定时器计数频率 = PCLK / ( psc + 1 )
  * @定时器中断周期 = ( arr + 1 )*( pac + 1) / PCLK
  * @retval 
  */
void TIM4_PWM_Init(uint16 arr, uint16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器4时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOB和AFIO复用功能时钟 
  
  //配置PB8为复用推挽输出,输出TIM4_CH3的PWM脉冲波形	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB8，对应TIM4_CH3
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB8，对应TIM4_CH1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
  
  //初始化TIM4
  TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
  TIM_ARRPreloadConfig(TIM4,ENABLE);//使能ARR预装载，防止向上计数时更新事件异常延迟
  
  //初始化TIM4_CH3 PWM输出
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //将配置参数初始化外设TIM4 OC3
  
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR3上的预装载寄存器
  
  TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}


/**
  * @fun    void TIM2_IRQHandler
  * @brief  定时器2中断处理函数
  * @author huangzibo
  * @param  None
  *
  * @retval
  */
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 	    //检查TIM2更新中断发生与否
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 	 		//清除TIM2更新中断标志  
  
    LineNumber++;
    TimeCount++;
      
  }
}


/**
  * @fun    void TIM3_IRQHandler
  * @brief  定时器3中断处理函数
  * @author huangzibo
  * @param  None
  *
  * @retval
  */
void TIM3_IRQHandler(void)  
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)    //检查TIM3更新中断发生与否
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIM3更新中断标志

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
* @brief  每隔1ms对所有定时器变量减1
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
      /* 如果定时器变量减到1则设置定时器到达标志 */
      if (--_tmr->count == 0)
      {
        _tmr->flag = 1;
      }
    }
  }
}


/**
* @fun    void SysTick_ISR
* @brief  SysTick中断服务程序，每隔1ms进入1次
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
  
  g_iRunTime++;	/* 全局运行时间每1ms增1 */	
  if (g_iRunTime == 0x7FFFFFFF)
  {
    g_iRunTime = 0;
  }
}

/**
* @fun    void SysTick_Handler
* @brief  滴答定时器中断处理函数
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
* @brief  初始化软件定时器
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
  SysTick_Config(SystemCoreClock / 1000);/* 配置中断周期1ms，并启动中断 */
  
}


/**
* @fun    void DelayMs
* @brief  ms级延迟，延迟精度为正负1ms
* @author huangzibo
* @param  uint32  n
*
* @retval 
*/
void DelayMs(uint32  n)
{
  /* 避免 n = 1 出现主程序死锁 */
  if (n <= 1)
  {
    n = 2;
  }
  
  __set_PRIMASK(1);  		/* 关中断 */
  g_Tmr[0].count = n;
  g_Tmr[0].flag = 0;
  __set_PRIMASK(0);  		/* 开中断 */
  
  while (1)
  {
    CPU_IDLE();	/* 此处是空操作。用户可以定义，让CPU进入IDLE状态，以降低功耗；或实现喂狗 */
    
    /* 等待延迟时间到 */
    if (g_Tmr[0].flag == 1)
    {
      break;
    }
  }
}

void delay_us(uint32  n)
{
  /* 避免 n = 1 出现主程序死锁 */
  if (n <= 1)
  {
    n = 2;
  }
  
  __set_PRIMASK(1);  		/* 关中断 */
  g_Tmr[0].count = n;
  g_Tmr[0].flag = 0;
  __set_PRIMASK(0);  		/* 开中断 */
  
  while (1)
  {
    CPU_IDLE();	/* 此处是空操作。用户可以定义，让CPU进入IDLE状态，以降低功耗；或实现喂狗 */
    
    /* 等待延迟时间到 */
    if (g_Tmr[0].flag == 1)
    {
      break;
    }
  }
}
/**
* @fun    void bsp_StartTimer
* @brief  启动一个定时器，并设置周期
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
    /* while(1); 死机 */
    return;
  }
  
  __set_PRIMASK(1);  		/* 关中断 */
  g_Tmr[_id].count = _period;
  g_Tmr[_id].flag = 0;
  __set_PRIMASK(0);  		/* 开中断 */
}


/**
* @fun    void bsp_StopTimer
* @brief  停止一个定时器
* @author huangzibo
* @param  uint8 _id
*
* @retval 
*/
void bsp_StopTimer(uint8 _id)
{
  if (_id >= TMR_COUNT)
  {
    /* while(1); 死机 */
    return;
  }
  
  __set_PRIMASK(1);  		/* 关中断 */
  g_Tmr[_id].count = 0;
  g_Tmr[_id].flag = 0;
  __set_PRIMASK(0);  		/* 开中断 */
}


/**
* @fun    uint8 bsp_CheckTimer
* @brief  检测定时器是否超时
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
* @brief  获取CPU运行时间，单位1ms
* @author huangzibo
* @param  None
*
* @retval 
*/
int32 bsp_GetRunTime(void)
{
  int runtime; 
  
  __set_PRIMASK(1);  		/* 关中断 */
  
  runtime = g_iRunTime;	/* 由于在Systick中断被改写，因此关中断进行保护 */
  
  __set_PRIMASK(0);  		/* 开中断 */
  
  return runtime;
}

/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

