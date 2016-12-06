/**
  ******************************************************************************
  * @file    ds1302.c
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/20 ������,Administrator
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
#include "ds1302.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "intrinsics.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define NOP() __NOP

#define DS1302_CLK_H()	GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define DS1302_CLK_L()	GPIO_ResetBits(GPIOB,GPIO_Pin_0)


#define DS1302_RST_H()	GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define DS1302_RST_L()	GPIO_ResetBits(GPIOC,GPIO_Pin_5)

#define DS1302_OUT_H()	GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define DS1302_OUT_L()	GPIO_ResetBits(GPIOC,GPIO_Pin_4)
											
#define DS1302_IN_X		(GPIOC->IDR&GPIO_Pin_4)

#define Time_24_Hour	0x00	//24ʱ�ƿ���	
#define Time_Start		0x00	//��ʼ��ʱ

#define WRITE_DS1302_SECOND	    0x80	//DS1302���Ĵ���д�����
#define WRITE_DS1302_MINUTE	    0x82
#define WRITE_DS1302_HOUR		0x84
#define WRITE_DS1302_DAY		0x86
#define WRITE_DS1302_MONTH	    0x88
#define WRITE_DS1302_WEEK		0x8A
#define WRITE_DS1302_YEAR		0x8C
#define WRITE_DS1302_WRITE	    0x8E
#define WRITE_DS1302_POWER	    0x90

#define READ_DS1302_SECOND	    0x81	//DS1302���Ĵ����������
#define READ_DS1302_MINUTE	    0x83
#define READ_DS1302_HOUR		0x85
#define READ_DS1302_DAY		    0x87
#define READ_DS1302_MONTH	    0x89
#define READ_DS1302_WEEK		0x8B
#define READ_DS1302_YEAR		0x8D
#define READ_DS1302_WRITE	    0x8F
#define READ_DS1302_POWER	    0x91

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 

/* Private function prototypes -----------------------------------------------*/  
/* Private functions ---------------------------------------------------------*/
/**
  * @fun    void Init_Ds1302
  * @brief  ��ʼ��DS1302
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void Init_Ds1302(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOB , ENABLE);
  
  /* PC4,5��� */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}


/**
  * @fun    void DS1302_IN
  * @brief  DS1302������������
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void DS1302_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**
  * @fun    void DS1302_OUT
  * @brief  DS1302�����������
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void DS1302_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**
  * @fun    void DelayNOP
  * @brief  ��ʱ
  * @author huangzibo
  * @param  uint32 Count
  *
  * @retval 
  */
void DelayNOP(uint32 Count)
{
  uint8 i = 0;
  while(Count --)
  {
    for(i=0;i<72;i++)
    {
      __ASM("NOP");
    }
  }
}

/**
  * @fun    void DS1302SendByte
  * @brief  DS1302�����ֽں���
  * @author huangzibo
  * @param  uint8 byte
  *
  * @retval 
  */
void DS1302SendByte(uint8 byte)
{
  uint8	i;
  DS1302_OUT();
  DelayNOP(2);
  DS1302_CLK_L();
  DelayNOP(2);
  for(i=0;i<8;i++)
  {
    DS1302_CLK_L();
    if(byte&0x01)	
    {
      DS1302_OUT_H();
    }
    else	
    {
      DS1302_OUT_L();
    }
    DelayNOP(2);
    DS1302_CLK_H();
    byte >>= 1;
    DelayNOP(1);		//����ʱ   
  }
}


/**
  * @fun    uint8 DS1302ReceiveByte
  * @brief  DS1302�����ֽں���
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
uint8 DS1302ReceiveByte(void)
{
  uint8	i = 0;
  uint8 byte = 0; 
  for(i=0;i<8;i++)
  {
    DS1302_CLK_H();
    byte >>= 1;
    DS1302_CLK_L();
    if(DS1302_IN_X)	
    {
      byte = byte|0x80;
    }
  }
  return(byte);
}


/**
  * @fun    void WriteDS1302
  * @brief  DS1302д����
  * @author huangzibo
  * @param  uint8 addr
  * @param  uint8 data
  *
  * @retval 
  */
void WriteDS1302(uint8 addr, uint8 data)
{
  DS1302_OUT();
  DelayNOP(2);
  DS1302_RST_L();
  DS1302_CLK_L();
  DelayNOP(1);
  DS1302_RST_H();
  DelayNOP(2);
  DS1302SendByte(addr);
  DS1302SendByte(data);
  DelayNOP(1);
  DS1302_RST_L();
}


/**
  * @fun    void ReadDS1302
  * @brief  DS1302������
  * @author huangzibo
  * @param  uint8 addr
  *
  * @retval 
  */
uint8 ReadDS1302(uint8 addr)
{
  uint8 data=0; 
  DS1302_OUT();
  DelayNOP(2);
  DS1302_RST_L();
  DS1302_CLK_L();
  DelayNOP(3);
  DS1302_RST_H();
  DelayNOP(3);
  DS1302SendByte(addr);
  DS1302_IN();
  DelayNOP(20);
  data = DS1302ReceiveByte();
  DelayNOP(1);
  DS1302_RST_L();
  return data;
}


/**
  * @fun    void DS1302_GetTime
  * @brief  ��ȡʱ�亯��
  * @author huangzibo
  * @param  uint8 *time
  *
  * @retval 
  */
void DS1302_GetTime(uint8 *time)
{
  WriteDS1302(WRITE_DS1302_WRITE,0x00);
  
  time[0] = ReadDS1302(READ_DS1302_YEAR);
  time[1] = ReadDS1302(READ_DS1302_WEEK);
  time[2] = ReadDS1302(READ_DS1302_MONTH);
  time[3] = ReadDS1302(READ_DS1302_DAY);
  time[4] = ReadDS1302(READ_DS1302_HOUR);
  time[5] = ReadDS1302(READ_DS1302_MINUTE);
  time[6] = ReadDS1302(READ_DS1302_SECOND);	
}


/**
  * @fun    uint8 ReadDS1302RAM
  * @brief  ��ȡDS1302�е�RAM
  * @author huangzibo
  * @param  uint8 addr
  *
  * @retval 
  */
uint8 ReadDS1302RAM(uint8 addr)
{
  uint8 tmp,res;
  
  tmp = (addr<<1)|0xc0;
  res = ReadDS1302(tmp);
  return(res);
}


/**
  * @fun    void WriteDS1302RAM
  * @brief  дDS1302�е�RAM
  * @author huangzibo
  * @param  uint8 addr
  * @param  uint8 data
  *
  * @retval 
  */
void WriteDS1302RAM(uint8 addr,uint8 data)
{
  uint8	tmp;
  
  WriteDS1302(WRITE_DS1302_WRITE,0x00);		//�ر�д����
  tmp = (addr<<1)|0xc0;
  WriteDS1302(tmp,data);
  WriteDS1302(WRITE_DS1302_WRITE,0x80);		//��д����
}


/**
  * @fun    void ReadDS1302Clock
  * @brief  ��ʱ�亯����˳��Ϊ����������ʱ����
  * @author huangzibo
  * @param  uint8 *time
  *
  * @retval 
  */
void ReadDS1302Clock(uint8 *time)
{
  WriteDS1302(WRITE_DS1302_WRITE,0x00);		//�ر�д����

  time[0] = ReadDS1302(READ_DS1302_YEAR);		//��
  time[1] = ReadDS1302(READ_DS1302_SECOND);		//��
  time[2] = ReadDS1302(READ_DS1302_WEEK);		//��
  time[3] = ReadDS1302(READ_DS1302_MONTH);		//��
  time[4] = ReadDS1302(READ_DS1302_DAY);		//ʱ
  time[5] = ReadDS1302(READ_DS1302_HOUR);		//��
  time[6] = ReadDS1302(READ_DS1302_MINUTE);		//��

  WriteDS1302(WRITE_DS1302_WRITE,0x80);		//��д����
}

/**
  * @fun    void WriteDS1302Clock
  * @brief  дʱ�亯����˳��Ϊ��������ʱ����
  * @author huangzibo
  * @param  uint8 time
  *
  * @retval 
  */
void WriteDS1302Clock(uint8 *time)
{
  WriteDS1302(WRITE_DS1302_WRITE,0x00);	//�ر�д����
  DS1302_OUT();
  DS1302_RST_L();
  DS1302_CLK_L();
  DS1302_RST_H();
  DelayNOP(100);
  DS1302SendByte(0xbe);				//ͻ��ģʽ
  DS1302SendByte(time[5]);			//��
  DS1302SendByte(time[4]);			//��
  DS1302SendByte(time[3]);			//ʱ
  DS1302SendByte(time[2]);			//��
  DS1302SendByte(time[1]);			//��
  DS1302SendByte(0x01);				//�ܣ����ó���һ��û��ʹ��
  DS1302SendByte(time[0]);			//��
  DS1302SendByte(0x80);				//������־�ֽ�
  DelayNOP(100);
  DS1302_RST_L();
}

/**
  * @fun    void InitDS1302Clock
  * @brief  ��ʼ��ʱ��DS1302
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void InitDS1302Clock(void)
{
  uint8	tmp;
  
  Init_Ds1302();
  tmp = ReadDS1302RAM(0);
  if(tmp == 0xa5)
  {
    WriteDS1302RAM(0,0xa5);
    WriteDS1302(WRITE_DS1302_WRITE,0x00);		//�ر�д����
    WriteDS1302(0x90,0x03);				        //��ֹ������
    WriteDS1302(WRITE_DS1302_HOUR,0x00);		//���ó�24Сʱ��
    WriteDS1302(WRITE_DS1302_SECOND,0x00);	    //ʹ��ʱ������
    WriteDS1302(WRITE_DS1302_WRITE,0x80);		//��д����
  }
}


/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

