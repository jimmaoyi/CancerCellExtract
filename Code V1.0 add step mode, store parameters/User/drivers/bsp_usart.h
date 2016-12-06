/**
  ******************************************************************************
  * @file    bsp_usart.h
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2015/12/23 ÐÇÆÚÈý,Administrator
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _bsp_usart_H
#define	_bsp_usart_H
  
#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
extern uint8 Count1;
extern uint8 USART1MemoryBuffer[10];
extern uint8 USART2MemoryBuffer[3];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void bsp_InitUart(void);

extern void SendNetworkCMD(uint8 CMD);
extern void SendMessage(uint8 *Buffer1, uint8 length);
extern void SendMessageusart2(uint8 *Buffer1, uint8 length);
  
#ifdef __cplusplus
}
#endif

#endif /* _bsp_usart_H */
/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/


