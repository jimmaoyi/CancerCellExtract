/**
  ******************************************************************************
  * @file    bsp_rtc.h
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/28 ÐÇÆÚËÄ,Administrator
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
#ifndef _bsp_rtc_H
#define	_bsp_rtc_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/ 
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/

typedef struct
{
  uint8 hour;
  uint8 minute;
  uint8 second;
  uint8 week;
  uint8 date;
  uint8 month;
  uint16 year;
}_Calendar_obj;


/* Exported constants --------------------------------------------------------*/
extern const uint8 mon_table[12];
extern const uint8 table_week[12]; 
extern uint8 sec_interrupt_flag;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern _Calendar_obj calendar;

static void RTC_NVIC_Config(void);
uint8 RTC_Init(void);
uint8 Is_Leap_Year(uint16 year);
uint8 RTC_Set(uint16 year, uint8 month, uint8 day,uint8 hour,uint8 minute,uint8 second);
uint8 RTC_GetTime(void);
uint8 RTC_Get_Week(uint16 year, uint8 month, uint8 day);


#ifdef __cplusplus
}
#endif

#endif /* _bsp_rtc_H */
/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/
