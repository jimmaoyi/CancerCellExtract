/**
  ******************************************************************************
  * @file    bsp_key.h
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/14 ÐÇÆÚËÄ,Administrator
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
#ifndef _bsp_key_H
#define	_bsp_key_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
  
extern uint8 KeyFlag;
  
/* Exported functions ------------------------------------------------------- */
void bsp_KeyInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _bsp_key_H */
/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/
