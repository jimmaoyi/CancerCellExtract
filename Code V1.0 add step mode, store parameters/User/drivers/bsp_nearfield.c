/**
  ******************************************************************************
  * @file    bsp_nearfield.c
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2015/12/23 星期三,Administrator
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
  *         2015/12/23 星期三,Administrator
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
  
  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBê±?óê1?ü 
  //RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ê±?óê1?ü
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //开启AFIO时钟
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁止JTAG功能
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_10;	 //IO-->PF0?￠PF1 ???ú????  GPIO_Pin_8 |GPIO_Pin_9 |
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //í?íìê?3?
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO?ú?ù?è?a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //?ù?Yéè?¨2?êy3?ê??ˉPF0?￠PF1
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15?′ó?í?íìê?3? 
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);//3?ê??ˉGPIOB  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE); //开启端口时钟和外设时钟
  
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
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级组2：2位抢占优先级，2为子优先级

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断通道使能
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
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line11);//清除EXTI line8中断标志
    
    
    USART2MemoryBuffer[1] = 6;
  }
}

/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

