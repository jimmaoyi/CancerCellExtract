
/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_usart.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8 Count1 = 0;
uint8 Count2 = 0;
/* Private function prototypes -----------------------------------------------*/ 
uint8 USART1ReceiveBuffer[10] = {0};
uint8 USART1MemoryBuffer[10] = {0};
uint8 USART2ReceiveBuffer[3] = {0};
uint8 USART2MemoryBuffer[3] = {0};
/* Private functions ---------------------------------------------------------*/
uint16 MBcrc16(uint8 *ptr, int len);
void SendMessage(uint8 *Buffer, uint8 length);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ��1������GPIO��USART������ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  
#ifdef  BSP_USART1
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
  NVIC_Init(&NVIC_InitStructure);	
  
  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  
  USART_Cmd(USART1, ENABLE);
  /* 
  CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
  �����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
  �巢����ɱ�־��Transmission Complete flag 
  */
  USART_ClearFlag(USART1, USART_FLAG_TC);
#endif
  
#ifdef  BSP_USART2
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  /* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
  NVIC_Init(&NVIC_InitStructure);	
  
  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);
  
  
  USART_ClearFlag(USART2, USART_FLAG_TC);
  
#endif
  
}

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���2��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
  /* дһ���ֽڵ�USART2 */
  USART_SendData(USART2, (uint8) ch);
  
  /* �ȴ����ͽ��� */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  
  return ch;
}

/**
* @fun    void USART1_IRQHandler
* @brief  ����1�жϴ�����
*         2015/12/23 ������,Administrator
* @param  None
*
* @retval 
*/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    USART1ReceiveBuffer[Count1] = USART_ReceiveData(USART1);
    Count1++;
    if(Count1 == 10) 
    {
      if(USART1ReceiveBuffer[0]==0XAA&&USART1ReceiveBuffer[1]==0X55)
      {
        if((MBcrc16(USART1ReceiveBuffer,8)==(USART1ReceiveBuffer[9]<<8|USART1ReceiveBuffer[8])))
        {
          memcpy(USART1MemoryBuffer,USART1ReceiveBuffer,10);
          memset(USART1ReceiveBuffer,0,10); 
        }
      }
      Count1 = 0;
    }
  }
}


/**
* @fun    void USART2_IRQHandler
* @brief  
*         2015/12/23 ������,Administrator
* @param  None
*
* @retval 
*/
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  { 
    USART2ReceiveBuffer[Count2] = USART_ReceiveData(USART2);
    Count2++;	
    if(Count2 == 3) 
    {
      Count2 = 0;
      if((USART2ReceiveBuffer[0]==0x90)&&(USART2ReceiveBuffer[2]==0x97))
      {
        Count2 = 0;
        memcpy(USART2MemoryBuffer,USART2ReceiveBuffer,3);
        memset(USART2ReceiveBuffer,0,3); 
      }
    }

    
    
    //USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
  }
}


/**
* @fun    uint16 MBcrc16
* @brief  16λCRCУ��
*         2015/12/23 ������,Administrator
* @param  uint8 *ptr
* @param  int len
*
* @retval CRCУ��ֵ
*/
uint16 MBcrc16(uint8 *ptr, int len)
{
  uint8 i;
  uint16 crc = 0xffff;
  if(len == 0)
  {
    len = 1;
  }
  while(len--)
  {
    crc ^= *ptr;
    for(i = 0; i < 8; i++)
    {
      if(crc & 1)
      {
        crc>>=1;
        crc ^= 0xa001;		
      }
      else
      {
        crc >>=1;
      }
    }
    ptr++;
  }
  return (crc);
}

/**
* @fun    void InitativeReportMessage(StatusMsgTypeDef StatusMsg,uint8 FunctionCode)
* @brief  ������2530�ϱ���ǰ״̬��Ϣ
*         2015/12/23 ������,Administrator
* @param  
* @param  
* @param    
*
* @retval 
*/

uint8 Buffer[19]={0};
/*
void InitativeReportMessage(StatusMsgTypeDef StatusMsg,uint8 FunctionCode)
{
  Buffer[0] = 0XAA;
  Buffer[1] = 0X55;
  Buffer[2] = 0X01;
  Buffer[3] = FunctionCode;
  Buffer[4] = 0X0C;
  
  Buffer[5] = 0X00;
  Buffer[6] = StatusMsg.WorkMode;
  
  Buffer[7] = 0X00;
  Buffer[8] = StatusMsg.WindMode;
  
  Buffer[9] = 0X00;
  Buffer[10] = StatusMsg.Symbol; 
  Buffer[11] = (StatusMsg.CurrentTemperature>>8)&0XFF;
  Buffer[12] = StatusMsg.CurrentTemperature&0XFF;
  
  Buffer[13] = 0X00;
  Buffer[14] = StatusMsg.HeatingTemperature;
  
  Buffer[15] = 0X00;
  Buffer[16] = StatusMsg.CoolingTemperature;
  
  Buffer[17] = MBcrc16(Buffer, 17)&0XFF;
  Buffer[18] = (MBcrc16(Buffer, 17)>>8)&0XFF;
  
  SendMessage(Buffer,19); 
}
*/

/**
  * @fun    void SendNetworkCMD
  * @brief  �����롢����������
  *         2015/12/26 ������,Administrator
  * @param  uint8 CommandCode������0x01������0x00
  *
  * @retval 
  */
void SendNetworkCMD(uint8 CommandCode)
{
  uint8 NetBuf[8];
  NetBuf[0] = 0XAA;
  NetBuf[1] = 0X55;
  NetBuf[2] = 0X01;
  NetBuf[3] = 0X42;
  NetBuf[4] = 0X20;
  NetBuf[5] = CommandCode;
  NetBuf[6] = MBcrc16(Buffer, 6)&0XFF;
  NetBuf[7] = (MBcrc16(Buffer, 6)>>8)&0XFF;
  SendMessage(NetBuf,8);
}

/**
* @fun    void SendMessage
* @brief  �������ݰ�
*         2015/12/23 ������,Administrator
* @param  uint8 *Buffer
* @param  uint8 length
*
* @retval 
*/
void SendMessage(uint8 *Buffer1, uint8 length)
{
  uint8 i;
  for(i=0;i<length;i++)
  {
    USART_SendData(USART1,Buffer1[i]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  }
}


/**
* @fun    void SendMessageusart2
* @brief  �������ݰ�
*         2015/12/23 ������,Administrator
* @param  uint8 *Buffer
* @param  uint8 length
*
* @retval 
*/
void SendMessageusart2(uint8 *Buffer1, uint8 length)
{
  uint8 i;
  for(i=0;i<length;i++)
  {
    USART_SendData(USART2,Buffer1[i]);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  }
}

