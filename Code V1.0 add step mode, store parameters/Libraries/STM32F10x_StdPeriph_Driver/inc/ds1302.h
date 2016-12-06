/**
  ******************************************************************************
  * @file    ds1302.h
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/20 ÐÇÆÚÈý,Administrator
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
#ifndef _ds1302_H
#define	_ds1302_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

void InitDS1302Clock(void);
void WriteDS1302Clock(uint8 *time);
void ReadDS1302Clock(uint8 *time);
void WriteDS1302RAM(uint8 addr,uint8 data);
uint8 ReadDS1302RAM(uint8 addr);
uint8 ReadDS1302(uint8 addr);
void WriteDS1302(uint8 addr, uint8 data);
uint8 DS1302ReceiveByte(void);
void DS1302SendByte(uint8 byte);
void DS1302_GetTime(uint8 *time);


#endif /* _ds1302_H */
/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/
