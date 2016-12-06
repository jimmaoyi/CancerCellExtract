/**
  ******************************************************************************
  * @file    bsp_key.c
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
#include "bsp_key.h"
#include "stm32f10x.h"
#include "main.h"
#include "bsp_usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 

uint8 KeyFlag = 0;

/* Private function prototypes -----------------------------------------------*/  
/* Private functions ---------------------------------------------------------*/
/**
  * @fun    void bsp_KeyInit
  * @brief  按键初始化函数
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void bsp_KeyInit(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  
  //步骤一:开启GPIOC时钟和辅助功能IO时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
  
  //步骤二：配置PC0为浮空输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //步骤三：连接EXTI0外部中断线到端口PC0
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
  
  //步骤四：配置EXTI0外部中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 | GPIO_Pin_1 | GPIO_Pin_2 ;						
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
  EXTI_Init(&EXTI_InitStructure);
  
  //步骤五：配置中断优先级组（同一个工程中，优先级组必须相同，这里设置为组2）
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级组2：2位抢占优先级，2为子优先级
  
  //步骤六：配置中断通道优先级
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //步骤六：配置中断通道优先级
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //步骤六：配置中断通道优先级
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @fun    void EXTI0_IRQHandler
  * @brief  按键中断处理函数
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line0);//清除EXTI line0中断标志
    
    USART2MemoryBuffer[1] = 3;
    //ModeAction = 1;
  }
}


void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line1);//清除EXTI line1中断标志
    
    USART2MemoryBuffer[1] = 4;
    //ModeAction = 2;
  }
}

void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line2);//清除EXTI line2中断标志
    
    USART2MemoryBuffer[1] = 5;
    //ModeAction = 3;
  }
}


/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

