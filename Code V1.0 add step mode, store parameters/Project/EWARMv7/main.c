/**
******************************************************************************
* @file    main.c
* @author  Wireless Protocol Stack Develop-Dept.
* @version --
* @date    2016/7/1 FRI,Administrator
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
#include "stm32f10x_flash.h"
#include "stm32f10x_conf.h"
#include "bsp_nearfield.h"
#include "string.h"
#include "main.h"
#include "bsp_rtc.h"
#include "bsp_usart.h"

#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_beep.h"

#define BANK2_WRITE_START_ADDR  ((uint32)0x0800F800)
#define BANK3_WRITE_START_ADDR  ((uint32)0x0800F400)
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
uint8 PowerFlag = 1;
uint8 MenuCount = 0;
uint8 StandbyFlag = 0;
uint8 Minute_Ini = 0;

DisplayRefreshLedStatuse_TypeDef RefreshLedStatuse; //定义枚举变量刷新LED的显示状态

uint8 statusRefreshed = 0;
uint8 mode = 0;
uint8 ModeRunFinished = 1;
uint8 ModeAction = 0;
uint16 M1T1 = 20;
uint16 M1T2 = 20;
uint16 M1T3 = 20;
uint16 M1T4 = 10;
uint16 M2T1 = 10;
uint16 M3T1 = 10;
uint16 M3T2 = 20;
uint16 M3T3 = 20;
uint16 M3T4 = 30;
uint16 M4T1 = 10;
uint16 MotoSpeed = 30;
uint16 MotoSpeed1 = 30;
uint16 MotoStop = 0;
uint16 UpLimited = 999;
uint16 DownLimited = 1;
uint8 SetInc = 1;
uint8 SetDec = 1;

//更新参数时修改下面的holder
uint16 paraHolder = 0x0235 ;
/* Private function prototypes -----------------------------------------------*/  

void TouchValueHandle(uint8 *Buffer);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
   
  SystemInit();
  bsp_InitTimer();      /* 软件定时器初始化*/  
  bsp_InitUart();		/* 初始化串口 */ 
  bsp_NearFieldGpioInit();
  Timer2_Init(19, 2399);// 计数频率 = 72000/(7199+1) = 10KHz，计数到(19+1)为2ms
  Timer3_Init(9999, 2399);// 计数频率 = 72000/(7199+1) = 10KHz，计数到(999+1)为100ms,9999为1秒
  RTC_Init();   /* 内部RTC时钟初始化 */
  BEEP_Init();  /* 蜂鸣器初始化 */
  
  GPIO_SetBits(GPIOC, GPIO_Pin_13);//??  GPIO_Pin_4  
  Lcd_Init();
  //bsp_InitLed();/* 初始化LED的GPIO */
  //bsp_InitMotor();/* 振动马达初始化 */
  //MONI_EEPROM_init();
  bsp_KeyInit();         
        
  BEEP_SetFreq(20000);
  BEEP_SetDuty(MotoStop); 
  
  if(paraHolder != (*(uint16*) (BANK3_WRITE_START_ADDR))){
    
        FLASH_Unlock();		//-第一步
        FLASH_ErasePage(BANK2_WRITE_START_ADDR);
  
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR, M1T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*1, M1T2);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*2, M1T3);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*3, M1T4);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*4, M2T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*5, M3T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*6, M3T2);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*7, M3T3);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*8, M3T4);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*9, M4T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*10, MotoSpeed);
        
        FLASH_Lock();		//-结束修改
        
        FLASH_Unlock();		//-第一步
        FLASH_ErasePage(BANK3_WRITE_START_ADDR);
        FLASH_ProgramHalfWord(BANK3_WRITE_START_ADDR, paraHolder);
        FLASH_Lock();		//-结束修改
        
        Lcd_Puts(0,0,"Program parameters");
  }
  else{
        M1T1 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*0));
        M1T2 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*1));
        M1T3 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*2));
        M1T4 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*3));
        M2T1 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*4));
        M3T1 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*5));
        M3T2 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*6));
        M3T3 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*7));
        M3T4 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*8));
        M4T1 = (*(uint16*) (BANK2_WRITE_START_ADDR+2*9));
        MotoSpeed = (*(uint16*) (BANK2_WRITE_START_ADDR+2*10));
        
        Lcd_Puts(0,0,"System start"); 
        Lcd_Puts(0,1,"Press any key"); 
  }
  
  while(1)
  {
    
    if(1 == statusRefreshed){             //显示参数
      statusRefreshed = 0;
      displayStatuse(RefreshLedStatuse);
    }
    
    TouchValueHandle(USART2MemoryBuffer);
    
    if (1 == bsp_CheckTimer(2)){   //read card success delay 5s
      
      ModeAction = 2;  
      bsp_StopTimer(2);       
     
    }
    
    if (1 == bsp_CheckTimer(1)){   //
      
        FLASH_Unlock();		//-第一步
        FLASH_ErasePage(BANK2_WRITE_START_ADDR);
  
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR, M1T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*1, M1T2);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*2, M1T3);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*3, M1T4);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*4, M2T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*5, M3T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*6, M3T2);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*7, M3T3);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*8, M3T4);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*9, M4T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*10, MotoSpeed);
        //FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*11, paraHolder);
        FLASH_Lock();		//-结束修改 
      
      ModeAction = 1;  
      bsp_StopTimer(1);       
     
    }
    
    if (1 == bsp_CheckTimer(3)){   
      
        FLASH_Unlock();		//-第一步
        FLASH_ErasePage(BANK2_WRITE_START_ADDR);
  
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR, M1T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*1, M1T2);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*2, M1T3);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*3, M1T4);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*4, M2T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*5, M3T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*6, M3T2);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*7, M3T3);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*8, M3T4);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*9, M4T1);
        FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*10, MotoSpeed);
        //FLASH_ProgramHalfWord(BANK2_WRITE_START_ADDR+2*11, paraHolder);
        FLASH_Lock();	
      
      ModeAction = 3;  
      bsp_StopTimer(3);       
     
    }
    
    if (1 == bsp_CheckTimer(4)){   //read card success delay 5s
      
      ModeAction = 4;  
      bsp_StopTimer(4);       
     
    }   
    
    
    switch(ModeAction)  //ONE KEY DECIDE MODE NUMBER
    {
      case 0x01:
        
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Running Mode1..."); 
      
      ModeRunFinished = 0;
      Minute_Ini = calendar.minute;
      //close all EM MV
      EMa((BitAction)1);
      EMb((BitAction)1);
      EMc((BitAction)1);
      EMd((BitAction)1);
      EMf((BitAction)1);
      EMe((BitAction)1);
      MV1((BitAction)1);
      MV2((BitAction)1);
      MV3((BitAction)1);
      MV4((BitAction)1);
      //PUMP START WORK
      BEEP_SetDuty(MotoSpeed);
      //WAIT FOR THE SETTED TIME OR PRSSURE OVER SETTING 
      BeepTime = 0;
      DelaySettingTime = M1T1;    //used to  delay time

      while(PowerFlag == 1)   //LINE4
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  //AND PRESSURE AJUDGMENT
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      EMb((BitAction)0);
      EMc((BitAction)0);
      EMd((BitAction)0);
      
      BeepTime = 0;
      DelaySettingTime = M1T2;    //used to  delay time

      while(PowerFlag == 1)   //LINE5
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      EMd((BitAction)1);
      EMf((BitAction)0);
      
      BeepTime = 0;
      DelaySettingTime = M1T3;    //used to  delay time

      while(PowerFlag == 1)   //LINE5
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      EMd((BitAction)0);
      
      BeepTime = 0;
      DelaySettingTime = M1T4;    //used to  delay time

      while(PowerFlag == 1)   //LINE6
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      BEEP_SetDuty(MotoStop);   //3.3V MAYBE IS PROBLEM SHOULD BE 5v
      
      EMa((BitAction)0);
      EMb((BitAction)0);
      EMc((BitAction)0);
      EMd((BitAction)0);
      EMf((BitAction)0);
      EMe((BitAction)0);
      MV1((BitAction)0);
      MV2((BitAction)0);
      MV3((BitAction)0);
      MV4((BitAction)0);
      
      ModeAction = 0; 
      statusRefreshed = 0;
      
      ModeRunFinished = 1;
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Mode1 finished.");
      
      break;
      
      case 0x02:
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Running Mode2...");   
        
      ModeRunFinished = 0;
      Minute_Ini = calendar.minute;
      // LINE 2  
      MV1((BitAction)0);
      MV2((BitAction)0);
      MV3((BitAction)0);
      MV4((BitAction)0);
      EMd((BitAction)0);
      EMf((BitAction)0);
      EMe((BitAction)0);
      
      EMa((BitAction)1);
      EMb((BitAction)1);
      EMc((BitAction)1);
      
      //LINE3
      BEEP_SetDuty(100);  //PUMP WORK 
      
      //
      BeepTime = 0;
      DelaySettingTime = M2T1;    //used to  delay time

      while(PowerFlag == 1)   //LINE4
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      BEEP_SetDuty(MotoStop);  //PUMP STOP WORK 
      
      EMa((BitAction)0);
      EMb((BitAction)0);
      EMc((BitAction)0);
      
      ModeAction = 0;
   
      ModeRunFinished = 1;
      statusRefreshed = 0;
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Mode2 finished.");
      break;
      
      case 0x03:
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Running Mode3...");   
        
      ModeRunFinished = 0;
      Minute_Ini = calendar.minute;
      // LINE1  
      MV1((BitAction)1);
      MV2((BitAction)1);
      MV3((BitAction)0);
      MV4((BitAction)1);
      EMa((BitAction)1);
      EMb((BitAction)1);
      EMc((BitAction)1);
      EMd((BitAction)1);
      EMf((BitAction)1);
      EMe((BitAction)1);
      
      BEEP_SetDuty(MotoSpeed);  //PUMP WORK 
      
      //WAIT FOR THE SETTED TIME OR PRSSURE OVER SETTING 
      BeepTime = 0;
      DelaySettingTime = M3T1;    //used to  delay time

      while(PowerFlag == 1)   //LINE4
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  //AND PRESSURE AJUDGMENT
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      EMa((BitAction)0);
      EMd((BitAction)0);
        
      BeepTime = 0;
      DelaySettingTime = M3T2;    //used to  delay time
      while(PowerFlag == 1)   //LINE5
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      EMd((BitAction)1);
      EMf((BitAction)0);
      
      BeepTime = 0;
      DelaySettingTime = M3T3;    //used to  delay time
      while(PowerFlag == 1)   //LINE5
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      EMd((BitAction)0);
      
      BeepTime = 0;
      DelaySettingTime = M3T4;    //used to  delay time
      while(PowerFlag == 1)   //LINE5
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      BEEP_SetDuty(MotoStop);   //3.3V MAYBE IS PROBLEM SHOULD BE 5v
      
      EMa((BitAction)0);
      EMb((BitAction)0);
      EMc((BitAction)0);
      EMd((BitAction)0);
      EMf((BitAction)0);
      EMe((BitAction)0);
      MV1((BitAction)0);
      MV2((BitAction)0);
      MV3((BitAction)0);
      MV4((BitAction)0);
      ModeAction = 0;
  
      ModeRunFinished = 1;
      statusRefreshed = 0;
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Mode3 finished.");
      break;
      
      case 0x04:
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Running Mode4...");   
        
      ModeRunFinished = 0;
      Minute_Ini = calendar.minute;
      //LINE1  
      EMa((BitAction)0);
      EMb((BitAction)0);
      EMc((BitAction)0);
      EMd((BitAction)0);
      EMf((BitAction)0);
      EMe((BitAction)1);
      
      MV1((BitAction)1);
      MV2((BitAction)1);
      MV3((BitAction)1);
      MV4((BitAction)1);
      
      BEEP_SetDuty(100);  //PUMP WORK 
      BeepTime = 0;
      DelaySettingTime = M4T1;    //used to  delay time

      while(PowerFlag == 1)   //LINE4
      {
        TouchValueHandle(USART2MemoryBuffer);
        if(1 == TimeOutFlag)  
        {
          TimeOutFlag = 0;
          break;
        }
      }
      
      BEEP_SetDuty(MotoStop);  //PUMP STOP WORK 
      EMe((BitAction)0);
      TouchValueHandle(USART2MemoryBuffer);
      MV1((BitAction)0);
      MV2((BitAction)0);
      MV3((BitAction)0);
      MV4((BitAction)0);
      ModeAction = 0;

      ModeRunFinished = 1;
      statusRefreshed = 0;
      Lcd_Write_Command( 0x01,1); //显示清屏 
      DelayMs(15); //延时5ms   
      Lcd_Puts(0,0,"Mode4 finished.");
      break;
      
      default:
      break;       
    }
    
  }    
}


/**
* @fun    void TouchValueHandle
* @brief  触摸按键值处理函数
* @author huangzibo
* @param  uint8 Value
*
* @retval 
*/
void TouchValueHandle(uint8 *Buffer){
  
  if (0 != Buffer[1]){
    
  switch(Buffer[1]){
 
  case 0x03:/* UP */
    
    TimeCount = 0;
    //TouchLedStatus(power,up,menu,down,wind);
    if((1 == StandbyFlag) || (0 ==PowerFlag))
    {
      ;
    }
    else if(KeyFlag)
    {
           
    }
    else if(PowerFlag)
    {
    statusRefreshed = 1;        
    switch(RefreshLedStatuse)
    {
    case MT11:
      M1T1 = M1T1 + SetInc;
      if(M1T1 > UpLimited-SetInc)
        {
          M1T1 = UpLimited;
        }
          
      break;      
    
    case speed:
      MotoSpeed = MotoSpeed + SetInc;
      if(MotoSpeed > 98)
        {
          MotoSpeed = 100;
        }
          
      break;   
    
    case Step:
      SetInc = SetInc + 1;
      if(SetInc > 100)
        {
          SetInc = 100;
        }
 
     break;   
      
    case MT12:
      M1T2 = M1T2 + SetInc;
      if(M1T2 > UpLimited-SetInc)
        {
          M1T2 = UpLimited;
        }
          
      break;   
      
    case MT13:
      M1T3 = M1T3 + SetInc;
      if(M1T3 > UpLimited-SetInc)
        {
          M1T3 = UpLimited;
        }
          
      break;     
      
    case MT14:
      M1T4 = M1T4 + SetInc;
      if(M1T4 > UpLimited-SetInc)
        {
          M1T4 = UpLimited;
        }
          
      break;   
      
   case MT21:
      M2T1 = M2T1+ SetInc;
      if(M2T1 > UpLimited-SetInc)
        {
          M2T1 = UpLimited;
        }
          
      break;   
      
    case MT31:
      M3T1 = M3T1 + SetInc;
      if(M3T1 > UpLimited-SetInc)
        {
          M3T1 = UpLimited;
        }
          
      break;      
      
    case MT32:
      M3T2 = M3T2 + SetInc;
      if(M3T2 > UpLimited-SetInc)
        {
          M3T2 = UpLimited;
        }
          
      break;   
      
    case MT33:
      M3T3 = M3T3 + SetInc; 
      if(M3T3 > UpLimited-SetInc)
        {
          M3T3 = UpLimited;
        }
          
      break;     
      
    case MT34:
      M3T4 = M3T4 + SetInc; 
      if(M3T4 > UpLimited-SetInc)
        {
          M3T4 = UpLimited;
        }
          
      break;   
      
   case MT41:
      M4T1 = M4T1 + SetInc; 
      if(M4T1 > UpLimited-SetInc)
        {
          M4T1 = UpLimited;
        }
          
      break;   
      
    default:
      break;
    }
     
    }
    break;
    
  case 0x04:/* 菜单 */                                                                                                                    
     
   
    if(0 ==PowerFlag);
    else
    {
      //TouchLedStatus(power,up,menu,down,wind);
      if(StandbyFlag)
      {
        StandbyFlag = 0;
      
 
      }
      else if(KeyFlag)
      {
        //
       
      }
      else if(PowerFlag)
      {
        MenuCount++;
        if(MenuCount == 13)MenuCount = 1;
        
        statusRefreshed = 1;
        switch(MenuCount)
        {           
        case 10:
       
        RefreshLedStatuse = MT41;       
       
        break;
        
        case 11:
           
        RefreshLedStatuse = speed;      
       
       
        break;
        
         case 12:
     
        RefreshLedStatuse = Step;      
       
      
        break;
        
        case 1:
            RefreshLedStatuse = MT11;     
         
        break;
        
        case 2:
     
        RefreshLedStatuse = MT12;;
       
        break;
        
        case 3:
        
        RefreshLedStatuse = MT13;
        break;
        
        case 4:
  
        RefreshLedStatuse = MT14;;
   
        break;
        
        case 5:
      
        RefreshLedStatuse = MT21;;
      
        break;
        
        case 6:
     
        RefreshLedStatuse = MT31;
     
   
        break;
        
        case 7:
       
        RefreshLedStatuse = MT32;;
      
        break;
        
        case 8:
 
        RefreshLedStatuse = MT33;
  
        break;
        
        case 9:
      
        RefreshLedStatuse = MT34;;        
    
        break;
        
        default:break;  
        }

      }
    }

    break;
    
  case 0x05:/* DOWN */
    
    
   
    if((1 == StandbyFlag) || (0 ==PowerFlag))
    {
      ;
    }
    else if(KeyFlag)
    {
      //RefreshLedStatuse = AddDeviceKey;
     
    }
    else if(PowerFlag)
    {
      statusRefreshed = 1;
      switch(RefreshLedStatuse)
      {
      
      case MT11:
        M1T1 = M1T1 - SetDec;
        if(M1T1 < DownLimited + SetDec)
        {
          M1T1 = DownLimited;
        }          
      break;      
     
     case Step:
      SetInc = SetInc - 1;
      if(SetInc < 2)
        {
          SetInc = 1;
        }
 
      break;       
      
     case speed:
      MotoSpeed = MotoSpeed - SetDec;
      if(MotoSpeed < 31)
        {
          MotoSpeed = 30;
        }
          
      break;      
      
    case MT12:
      M1T2 = M1T2 - SetDec;
      if(M1T2 < DownLimited + SetDec)
        {
          M1T2 =DownLimited;
        }
          
      break;   
      
    case MT13:
      M1T3 = M1T3 - SetDec;
      if(M1T3 < DownLimited + SetDec)
        {
          M1T3 = DownLimited;
        }
          
      break;     
      
    case MT14:
      M1T4 = M1T4 - SetDec;
      if(M1T4 < DownLimited + SetDec)
        {
          M1T4 = DownLimited;
        }
          
      break;   
      
   case MT21:
      M2T1 = M2T1 - SetDec;
      if(M2T1 < DownLimited + SetDec)
        {
          M2T1 = DownLimited;
        }
          
      break;   
      
    case MT31:
      M3T1 = M3T1 - SetDec;
      if(M3T1 < DownLimited + SetDec)
        {
          M3T1 = DownLimited;
        }
          
      break;      
      
    case MT32:
      M3T2 = M3T2 - SetDec;
      if(M3T2 < DownLimited + SetDec)
        {
          M3T2 = DownLimited;
        }
          
      break;   
      
    case MT33:
      M3T3 = M3T3 - SetDec;
      if(M3T3 < DownLimited + SetDec)
        {
          M3T3 = DownLimited;
        }
          
      break;     
      
    case MT34:
      M3T4 = M3T4 - SetDec;
      if(M3T4 < DownLimited + SetDec)
        {
          M3T4 = DownLimited;
        }
          
      break;   
      
   case MT41:
     
      M4T1 = M4T1 - SetDec;
      if(M4T1 < DownLimited + SetDec)
        {
          M4T1 = DownLimited;
        }
          
      break;   
      
    default:
      break;
    }
      
    }
    break;    
    
  case 0x06:/* / */
    
    RefreshLedStatuse = ModeDisplay;
    
    if((1 == StandbyFlag) || (0 ==PowerFlag))
    {
      ;
    }
    else if((1 == PowerFlag) &&  (ModeRunFinished == 1))
    {
      
      mode++;
      if(mode == 5)mode = 0;           
      statusRefreshed = 1;   
      
      switch(mode)
      {
        case 1:
        ModeAction = 0;
        bsp_StartTimer(1,5000);   //delay 4s again 
        bsp_StopTimer(2);  
        bsp_StopTimer(3);  
        bsp_StopTimer(4);   
        
        break;
        
        case 2:
        ModeAction = 0;
        bsp_StartTimer(2,5000);   //delay 4s again 
        bsp_StopTimer(1);  
        bsp_StopTimer(3);  
        bsp_StopTimer(4);  
 
        break;
        
        case 3:
        ModeAction = 0;
        bsp_StartTimer(3,5000);   //delay 4s again 
        bsp_StopTimer(1);  
        bsp_StopTimer(2);  
        bsp_StopTimer(4);  
        
        break;
        
        case 4:
        ModeAction = 0;
        bsp_StartTimer(4,5000);   //delay 4s again 
        bsp_StopTimer(1);  
        bsp_StopTimer(3);  
        bsp_StopTimer(2);          
        break;
        
        case 0:
        ModeAction = 0; 
        bsp_StopTimer(1);  
        bsp_StopTimer(3);  
        bsp_StopTimer(2);
        bsp_StopTimer(4); 
        break;
        
        default:
        ModeAction = 0; 
        break;
        }
      
      break;  
      }
    
    default:    
    break;
    }
  memset(Buffer,0,3);  
  }
}


/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 


