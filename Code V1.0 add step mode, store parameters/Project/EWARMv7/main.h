  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _main_H
#define	_main_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
  
#define DispPos_X     6  
  
typedef enum
{
  PowerKey = (uint8)0x20,/* 电源键 */
  SetHeatTemKey = (uint8)0x30,/* 设定温度键 */  
  SetCoolTemKey = (uint8)0x40,/* 设定温度键 */
  //StandbyHaltFlag = (uint8)0x07,  
  MenuKey = (uint8)0x00,  
  ModeDisplay = (uint8)0x10,/* 创建网络状态 */
  MT12 = (uint8)0x02,/* 允许添加新设备状态 */
  MT13 = (uint8)0x03,/* 确认状态 */    
  MT11 = (uint8)0x01,/* 允许添加新设备状态 */
  MT14 = (uint8)0x04,/* 确认状态 */   
  MT21 = (uint8)0x05,/* 确认状态 */   
  MT32 = (uint8)0x07,/* 允许添加新设备状态 */
  MT33 = (uint8)0x08,/* 确认状态 */    
  MT31 = (uint8)0x06,/* 允许添加新设备状态 */
  MT34 = (uint8)0x09,/* 确认状态 */  
  MT41 = (uint8)0x0A,
  speed13 = (uint8)0x0B,
  speed24 = (uint8)0x0C,
  Step = (uint8)0x0D,
}DisplayRefreshLedStatuse_TypeDef;

extern uint8 StandbyFlag;
extern uint8 PowerFlag;
extern uint8 Minute_Ini;
extern uint8 Motowork_flg;
extern uint8 pressureOK;   
extern uint8 statusRefreshed;

extern DisplayRefreshLedStatuse_TypeDef RefreshLedStatuse;

#define PROGRAMME_DAY_dot_NUM 6

typedef struct{ uint16 MIN;    // moni EXINT0 hold triggle level,not reset, so can't come into short again
                //-BYTE MIN;    // as above
                uint8 MIN_VOULE;    // as above
                uint8 MAX_VOULE;    // as above
              }PROGRAMME_TIME_CH;

typedef struct{ uint8  SET_status;   //-值为0表示定义已经存储,值为0x55表示值需要写入EEP
                PROGRAMME_TIME_CH day_dot[PROGRAMME_DAY_dot_NUM];
              }PROGRAMME_DAY_CH;

extern void MONI_EEPROM_init(void);
extern uint8 STM32_FLASH_write_word(uint32_t Address, uint32_t Data);

extern void Lcd_Init(void);
extern void Lcd_Puts(unsigned char x,unsigned char y, unsigned char *string);
extern void Lcd_Write_Command(unsigned char x,char Busy) ;
extern void displayStatuse(DisplayRefreshLedStatuse_TypeDef status) ;

extern unsigned char *tr_2(unsigned int outData_2) ; 
extern unsigned char *tr_3(unsigned int outData_3) ;
extern unsigned char *tr_4(unsigned int outData_4) ;
extern unsigned char *tr_5(unsigned int outData_5) ;
extern unsigned char *tr_6(unsigned int outData_6) ;
extern unsigned char *tr_7(unsigned int outData_7) ;
extern unsigned char strTemp2[3] ;  
extern unsigned char strTemp3[4] ;  
extern unsigned char strTemp4[5] ; 
extern unsigned char strTemp5[6] ; 
extern unsigned char strTemp6[7] ; 
extern unsigned char strTemp7[8] ; 

extern uint8 mode ;
extern uint8 SetInc;
extern uint16 MotoSpeed13;
extern uint16 MotoSpeed24;
extern uint16 M1T1;
extern uint16 M1T2;
extern uint16 M1T3;
extern uint16 M1T4;
extern uint16 M2T1;
extern uint16 M3T1;
extern uint16 M3T2;
extern uint16 M3T3;
extern uint16 M3T4;
extern uint16 M4T1;

#ifdef __cplusplus
}
#endif

#endif /* _main_H */

