/**
  ******************************************************************************
  * @file    bsp_key.c
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
  * @brief  ������ʼ������
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
  
  //����һ:����GPIOCʱ�Ӻ͸�������IOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
  
  //�����������PC0Ϊ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //������������EXTI0�ⲿ�ж��ߵ��˿�PC0
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
  
  //�����ģ�����EXTI0�ⲿ�ж���
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 | GPIO_Pin_1 | GPIO_Pin_2 ;						
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
  EXTI_Init(&EXTI_InitStructure);
  
  //�����壺�����ж����ȼ��飨ͬһ�������У����ȼ��������ͬ����������Ϊ��2��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ���2��2λ��ռ���ȼ���2Ϊ�����ȼ�
  
  //�������������ж�ͨ�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //�������������ж�ͨ�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  //�������������ж�ͨ�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @fun    void EXTI0_IRQHandler
  * @brief  �����жϴ�����
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line0);//���EXTI line0�жϱ�־
    
    USART2MemoryBuffer[1] = 3;
    //ModeAction = 1;
  }
}


void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line1);//���EXTI line1�жϱ�־
    
    USART2MemoryBuffer[1] = 4;
    //ModeAction = 2;
  }
}

void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line2);//���EXTI line2�жϱ�־
    
    USART2MemoryBuffer[1] = 5;
    //ModeAction = 3;
  }
}


/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

