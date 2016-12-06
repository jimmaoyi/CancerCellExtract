/**
  ******************************************************************************
  * @file    bsp_nearfield.c
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2015/12/23 ������,Administrator
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
#include "stm32f10x_exti.h"
#include "bsp_nearfield.h"
#include "bsp_usart.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/  
/* Private functions ---------------------------------------------------------*/
/**
  * @fun    void bsp_NearFieldGpioInit
  * @brief  
  *         2015/12/23 ������,Administrator
  * @param  None
  *
  * @retval 
  */
void bsp_NearFieldGpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_4 | GPIO_Pin_6| GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_0 | GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //GPIO_ResetBits(GPIOA,GPIO_Pin_11);			             //RC522 RESET
  //GPIO_ResetBits(GPIOA,GPIO_Pin_1);			                //gprs RESET  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB����?����1?�� 
  //RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2����?����1?��
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //����AFIOʱ��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //��ֹJTAG����
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_10;	 //IO-->PF0?��PF1 ???��????  GPIO_Pin_8 |GPIO_Pin_9 |
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //��?������?3?
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO?��?��?��?a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //?��?Y����?��2?��y3?��??��PF0?��PF1
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15?�䨮?��?������?3? 
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);//3?��??��GPIOB  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE); //�����˿�ʱ�Ӻ�����ʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11); 
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ���2��2λ��ռ���ȼ���2Ϊ�����ȼ�

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ͨ��ʹ��
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
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line11);//���EXTI line8�жϱ�־
    
    
    USART2MemoryBuffer[1] = 6;
  }
}

/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

