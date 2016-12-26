  
/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "bsp_timer.h"
#include "bsp_usart.h"
#include "bsp_rtc.h"

/* Private typedef -----------------------------------------------------------*/
const uint8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
const uint8 table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 

uint8 CurrentTime[8] = {0};
uint8 sec_interrupt_flag = 0;
_Calendar_obj calendar;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
//
/* Private function prototypes -----------------------------------------------*/  
/* Private functions ---------------------------------------------------------*/

/**
  * @fun    static void RTC_NVIC_Config
  * @brief  RTC时钟中断配置
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
static void RTC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
  NVIC_Init(&NVIC_InitStructure);		
}


/**
  * @fun    uint8 RTC_Init
  * @brief  初始化RTC时钟，同时检测时钟是否正常工作
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
uint8 RTC_Init(void)
{
  uint8 temp = 0;
  
  if (BKP_ReadBackupRegister(BKP_DR1) != 0x5051)/*从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎*/
  {	 			
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);/*使能PWR和BKP外设时钟*/  
    PWR_BackupAccessCmd(ENABLE);/*使能后备寄存器访问*/ 
    BKP_DeInit();/*复位备份区域*/ 	
    RCC_LSEConfig(RCC_LSE_ON);/*设置外部低速晶振(LSE),使用外设低速晶振*/
    for(temp=0;temp<50;temp++)
    {
      if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != RESET) break;
      DelayMs(10);
    }
//    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)/*检查指定的RCC标志位设置与否,等待低速晶振就绪*/
//    {
//      temp++;
//      DelayMs(10);
//    }
//    if(temp>=250)return 1;/*初始化时钟失败,晶振有问题*/	
    if(temp == 50)
    {
      RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
    }
    else RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*设置RTC时钟(RTCCLK),选择LSE作为RTC时钟*/    
    RCC_RTCCLKCmd(ENABLE);/*使能RTC时钟*/  
    RTC_WaitForLastTask();/*等待最近一次对RTC寄存器的写操作完成*/
    RTC_WaitForSynchro();/*等待RTC寄存器同步*/  
    RTC_ITConfig(RTC_IT_SEC, ENABLE);/*使能RTC秒中断*/
    RTC_WaitForLastTask();/*等待最近一次对RTC寄存器的写操作完成*/
    RTC_EnterConfigMode();/*允许配置*/	
    if(temp == 50)
    {
      RTC_SetPrescaler(62500);
    }
    else RTC_SetPrescaler(32767);/*设置RTC预分频的值*/
    RTC_WaitForLastTask();/*等待最近一次对RTC寄存器的写操作完成*/
    RTC_Set(2016,06,15,00,00,00);/*设置时间*/	
    RTC_ExitConfigMode();/*退出配置模式*/ 
    BKP_WriteBackupRegister(BKP_DR1, 0X5050);/*向指定的后备寄存器中写入用户程序数据*/
  }
  else/*系统继续计时*/
  {
    
    RTC_WaitForSynchro();/*等待最近一次对RTC寄存器的写操作完成*/
    RTC_ITConfig(RTC_IT_SEC, ENABLE);/*使能RTC秒中断*/
    RTC_WaitForLastTask();/*等待最近一次对RTC寄存器的写操作完成*/
  }
  RTC_NVIC_Config();/*RCT中断分组设置*/		    				     
  RTC_GetTime();/*更新时间*/	
  return 0; /*ok*/ 
}


/**
  * @fun    void RTC_IRQHandler
  * @brief  RTC时钟中断处理函数
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)/*秒钟中断*/
  {							
    RTC_GetTime();/*更新时间*/   
  //  SendCurrentTime(calendar);
    sec_interrupt_flag = 1;
  }
  if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)/*闹钟中断*/
  {
    RTC_ClearITPendingBit(RTC_IT_ALR);/*清闹钟中断*/	  	   
  } 				  								 
  RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);/*清闹钟中断*/
  RTC_WaitForLastTask();	
}


/**
  * @fun    uint8 Is_Leap_Year
  * @brief  判断是否是闰年
  * @author huangzibo
  * @param  uint16 year
  *
  * @retval 
  */
uint8 Is_Leap_Year(uint16 year)
{
  if(year%4==0) /*必须能被4整除*/
  { 
    if(year%100==0) 
    { 
      if(year%400==0)
      {
        return 1;/*如果以00结尾,还要能被400整除*/ 
      }
      else return 0;   
    }
    else return 1;   
  }
  else return 0;	
}


/**
  * @fun    uint8 RTC_Set
  * @brief  设置时钟
  * @author huangzibo
  * @param  uint16 year
  * @param  uint8 month
  * @param  uint8 day  
  * @param  uint8 hour
  * @param  uint8 minute
  * @param  uint8 second 
  *
  * @retval 
  */
uint8 RTC_Set(uint16 year, uint8 month, uint8 day,uint8 hour,uint8 minute,uint8 second)
{
  uint16 t;
  uint32 seccount=0;
  if(year<1970||year>2099)return 1;	   
  for(t=1970;t<year;t++)/*把所有年份的秒钟相加*/
  {
    if(Is_Leap_Year(t))
    {
      seccount+=31622400;/*闰年的秒钟数*/
    }
    else seccount+=31536000;/*平年的秒钟数*/
  }
  month = month - 1;
  for(t=0;t<month;t++)/*把前面月份的秒钟数相加*/
  {
    seccount+=(u32)mon_table[t]*86400;/*月份秒钟数相加*/
    if(Is_Leap_Year(year)&&t==1)seccount+=86400;/*闰年2月份增加一天的秒钟数*/	   
  }
  seccount+=(u32)(day-1)*86400;/*把前面日期的秒钟数相加*/ 
  seccount+=(u32)hour*3600;/*小时秒钟数*/
  seccount+=(u32)minute*60;/*分钟秒钟数*/
  seccount+=second;/*最后的秒钟加上去*/
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);/*使能PWR和BKP外设时钟*/  
  PWR_BackupAccessCmd(ENABLE);/*使能RTC和后备寄存器访问*/ 
  RTC_SetCounter(seccount);/*设置RTC计数器的值*/
  
  RTC_WaitForLastTask();/*等待最近一次对RTC寄存器的写操作完成*/ 	
  return 0;	    
}


/**
  * @fun    void RTC_GetTime
  * @brief  获取当前RTC时间
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
uint8 RTC_GetTime(void)
{
  static uint16 daycnt=0;
  uint32 timecount=0; 
  uint32 temp=0;
  uint16 temp1=0;	  
  timecount=RTC->CNTH;/*得到计数器中的值(秒钟数)*/
  timecount<<=16;
  timecount+=RTC->CNTL;			 
  
  temp=timecount/86400;/*得到天数(秒钟数对应的)*/
  if(daycnt != temp)/*超过一天了*/
  {	  
    daycnt = temp;
    temp1 = 1970;/*从1970年开始*/
    while(temp >= 365)
    {				 
      if(Is_Leap_Year(temp1))/*是闰年*/
      {
        if(temp >= 366)
        {
          temp = temp-366;/*闰年的秒钟数*/
        }
        else 
        {
          temp1++;break;
        }  
      }
      else 
      {
        temp = temp-365;	  /*平年*/ 
      }
      temp1++;  
    }   
    calendar.year = temp1;/*得到年份*/
    temp1 = 0;
    while(temp >= 28)/*超过了一个月*/
    {
      if(Is_Leap_Year(calendar.year)&&temp1==1)/*当年是不是闰年/2月份*/
      {
        if(temp >= 29)
        {
          temp = temp-29;/*闰年的秒钟数*/
        }
        else break; 
      }
      else 
      {
        if(temp >= mon_table[temp1])
        {
          temp = temp-mon_table[temp1];/*平年*/
        }
        else break;
      }
      temp1++;  
    }
    calendar.month=temp1+1;/*得到月份*/
    calendar.date=temp+1;/*得到日期*/ 
  }
  temp = timecount%86400;/*得到秒钟数*/   	   
  calendar.hour = temp/3600;/*小时*/
  calendar.minute = (temp%3600)/60;/*分钟*/	
  calendar.second = (temp%3600)%60;/*秒钟*/
  calendar.week = RTC_Get_Week(calendar.year,calendar.month,calendar.date);/*获取星期*/  
  return 0;
}


/**
  * @fun    uint8 RTC_Get_Week
  * @brief  输入公历日期得到星期
  * @author huangzibo
  * @param  uint16 year
  * @param  uint8 month
  * @param  uint8 day  
  *
  * @retval 
  */
uint8 RTC_Get_Week(uint16 year, uint8 month, uint8 day)
{
  uint16 temp2;
  uint8 yearH,yearL;
  yearH = year/100;	
  yearL = year%100;/*如果为21世纪,年份数加100*/  
  if(yearH > 19)
  {
    yearL = yearL + 100;/*所过闰年数只算1900年之后的*/ 
  }
  temp2 = yearL + yearL/4;
  temp2 = temp2%7; 
  temp2 = temp2+day+table_week[month-1];
  if (yearL%4==0&&month<3)temp2--;
  return(temp2%7);
}


