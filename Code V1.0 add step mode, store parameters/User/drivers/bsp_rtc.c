  
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
  * @brief  RTCʱ���ж�����
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
  * @brief  ��ʼ��RTCʱ�ӣ�ͬʱ���ʱ���Ƿ���������
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
uint8 RTC_Init(void)
{
  uint8 temp = 0;
  
  if (BKP_ReadBackupRegister(BKP_DR1) != 0x5051)/*��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����*/
  {	 			
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);/*ʹ��PWR��BKP����ʱ��*/  
    PWR_BackupAccessCmd(ENABLE);/*ʹ�ܺ󱸼Ĵ�������*/ 
    BKP_DeInit();/*��λ��������*/ 	
    RCC_LSEConfig(RCC_LSE_ON);/*�����ⲿ���پ���(LSE),ʹ��������پ���*/
    for(temp=0;temp<50;temp++)
    {
      if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != RESET) break;
      DelayMs(10);
    }
//    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)/*���ָ����RCC��־λ�������,�ȴ����پ������*/
//    {
//      temp++;
//      DelayMs(10);
//    }
//    if(temp>=250)return 1;/*��ʼ��ʱ��ʧ��,����������*/	
    if(temp == 50)
    {
      RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
    }
    else RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��*/    
    RCC_RTCCLKCmd(ENABLE);/*ʹ��RTCʱ��*/  
    RTC_WaitForLastTask();/*�ȴ����һ�ζ�RTC�Ĵ�����д�������*/
    RTC_WaitForSynchro();/*�ȴ�RTC�Ĵ���ͬ��*/  
    RTC_ITConfig(RTC_IT_SEC, ENABLE);/*ʹ��RTC���ж�*/
    RTC_WaitForLastTask();/*�ȴ����һ�ζ�RTC�Ĵ�����д�������*/
    RTC_EnterConfigMode();/*��������*/	
    if(temp == 50)
    {
      RTC_SetPrescaler(62500);
    }
    else RTC_SetPrescaler(32767);/*����RTCԤ��Ƶ��ֵ*/
    RTC_WaitForLastTask();/*�ȴ����һ�ζ�RTC�Ĵ�����д�������*/
    RTC_Set(2016,06,15,00,00,00);/*����ʱ��*/	
    RTC_ExitConfigMode();/*�˳�����ģʽ*/ 
    BKP_WriteBackupRegister(BKP_DR1, 0X5050);/*��ָ���ĺ󱸼Ĵ�����д���û���������*/
  }
  else/*ϵͳ������ʱ*/
  {
    
    RTC_WaitForSynchro();/*�ȴ����һ�ζ�RTC�Ĵ�����д�������*/
    RTC_ITConfig(RTC_IT_SEC, ENABLE);/*ʹ��RTC���ж�*/
    RTC_WaitForLastTask();/*�ȴ����һ�ζ�RTC�Ĵ�����д�������*/
  }
  RTC_NVIC_Config();/*RCT�жϷ�������*/		    				     
  RTC_GetTime();/*����ʱ��*/	
  return 0; /*ok*/ 
}


/**
  * @fun    void RTC_IRQHandler
  * @brief  RTCʱ���жϴ�����
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)/*�����ж�*/
  {							
    RTC_GetTime();/*����ʱ��*/   
  //  SendCurrentTime(calendar);
    sec_interrupt_flag = 1;
  }
  if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)/*�����ж�*/
  {
    RTC_ClearITPendingBit(RTC_IT_ALR);/*�������ж�*/	  	   
  } 				  								 
  RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);/*�������ж�*/
  RTC_WaitForLastTask();	
}


/**
  * @fun    uint8 Is_Leap_Year
  * @brief  �ж��Ƿ�������
  * @author huangzibo
  * @param  uint16 year
  *
  * @retval 
  */
uint8 Is_Leap_Year(uint16 year)
{
  if(year%4==0) /*�����ܱ�4����*/
  { 
    if(year%100==0) 
    { 
      if(year%400==0)
      {
        return 1;/*�����00��β,��Ҫ�ܱ�400����*/ 
      }
      else return 0;   
    }
    else return 1;   
  }
  else return 0;	
}


/**
  * @fun    uint8 RTC_Set
  * @brief  ����ʱ��
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
  for(t=1970;t<year;t++)/*��������ݵ��������*/
  {
    if(Is_Leap_Year(t))
    {
      seccount+=31622400;/*�����������*/
    }
    else seccount+=31536000;/*ƽ���������*/
  }
  month = month - 1;
  for(t=0;t<month;t++)/*��ǰ���·ݵ����������*/
  {
    seccount+=(u32)mon_table[t]*86400;/*�·����������*/
    if(Is_Leap_Year(year)&&t==1)seccount+=86400;/*����2�·�����һ���������*/	   
  }
  seccount+=(u32)(day-1)*86400;/*��ǰ�����ڵ����������*/ 
  seccount+=(u32)hour*3600;/*Сʱ������*/
  seccount+=(u32)minute*60;/*����������*/
  seccount+=second;/*�������Ӽ���ȥ*/
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);/*ʹ��PWR��BKP����ʱ��*/  
  PWR_BackupAccessCmd(ENABLE);/*ʹ��RTC�ͺ󱸼Ĵ�������*/ 
  RTC_SetCounter(seccount);/*����RTC��������ֵ*/
  
  RTC_WaitForLastTask();/*�ȴ����һ�ζ�RTC�Ĵ�����д�������*/ 	
  return 0;	    
}


/**
  * @fun    void RTC_GetTime
  * @brief  ��ȡ��ǰRTCʱ��
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
  timecount=RTC->CNTH;/*�õ��������е�ֵ(������)*/
  timecount<<=16;
  timecount+=RTC->CNTL;			 
  
  temp=timecount/86400;/*�õ�����(��������Ӧ��)*/
  if(daycnt != temp)/*����һ����*/
  {	  
    daycnt = temp;
    temp1 = 1970;/*��1970�꿪ʼ*/
    while(temp >= 365)
    {				 
      if(Is_Leap_Year(temp1))/*������*/
      {
        if(temp >= 366)
        {
          temp = temp-366;/*�����������*/
        }
        else 
        {
          temp1++;break;
        }  
      }
      else 
      {
        temp = temp-365;	  /*ƽ��*/ 
      }
      temp1++;  
    }   
    calendar.year = temp1;/*�õ����*/
    temp1 = 0;
    while(temp >= 28)/*������һ����*/
    {
      if(Is_Leap_Year(calendar.year)&&temp1==1)/*�����ǲ�������/2�·�*/
      {
        if(temp >= 29)
        {
          temp = temp-29;/*�����������*/
        }
        else break; 
      }
      else 
      {
        if(temp >= mon_table[temp1])
        {
          temp = temp-mon_table[temp1];/*ƽ��*/
        }
        else break;
      }
      temp1++;  
    }
    calendar.month=temp1+1;/*�õ��·�*/
    calendar.date=temp+1;/*�õ�����*/ 
  }
  temp = timecount%86400;/*�õ�������*/   	   
  calendar.hour = temp/3600;/*Сʱ*/
  calendar.minute = (temp%3600)/60;/*����*/	
  calendar.second = (temp%3600)%60;/*����*/
  calendar.week = RTC_Get_Week(calendar.year,calendar.month,calendar.date);/*��ȡ����*/  
  return 0;
}


/**
  * @fun    uint8 RTC_Get_Week
  * @brief  ���빫�����ڵõ�����
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
  yearL = year%100;/*���Ϊ21����,�������100*/  
  if(yearH > 19)
  {
    yearL = yearL + 100;/*����������ֻ��1900��֮���*/ 
  }
  temp2 = yearL + yearL/4;
  temp2 = temp2%7; 
  temp2 = temp2+day+table_week[month-1];
  if (yearL%4==0&&month<3)temp2--;
  return(temp2%7);
}


