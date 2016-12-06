#include "stm32f10x.h"
#include "bsp_timer.h"
#include "main.h"
/********************���˿ڶ���*********************************/ 
#define GPIO_EN GPIOC // ʹ�ܶ˿��� 
#define GPIO_EN_PIN GPIO_Pin_12 // ʹ�ܶ˿ں� 
#define RCC_GPIO_EN RCC_APB2Periph_GPIOC // ʹ��ʱ���� 

#define GPIO_RW GPIOC // ��/дѡ��˿��� 
#define GPIO_RW_PIN GPIO_Pin_11 // ��/дѡ��˿ں� 
#define RCC_GPIO_RW RCC_APB2Periph_GPIOC // ��/дʱ���� 

#define GPIO_RS GPIOC // ����/����˿��� 
#define GPIO_RS_PIN GPIO_Pin_10 // ����/����˿ں� 
#define RCC_GPIO_RS RCC_APB2Periph_GPIOC // ����/����ʱ���� 

#define GPIO_DATA_4 GPIOB // ������2_�˿��� 
#define GPIO_DATA_4_PIN GPIO_Pin_4 // ������2_�˿ں� 
#define GPIO_DATA_5 GPIOB // ������5_�˿��� 
#define GPIO_DATA_5_PIN GPIO_Pin_9 // ������5_�˿ں� 
#define GPIO_DATA_6 GPIOB // ������6_�˿��� 
#define GPIO_DATA_6_PIN GPIO_Pin_7 // ������6_�˿ں� 
#define GPIO_DATA_7 GPIOB // ������7_�˿��� 
#define GPIO_DATA_7_PIN GPIO_Pin_6 // ������7_�˿ں� 
#define RCC_GPIO_DATA RCC_APB2Periph_GPIOB // ������ʱ���� 
/******************************************************************/ 

/***********************����ָ��***********************************/ 
#define SET_EN GPIO_SetBits(GPIO_EN, GPIO_EN_PIN) //EN ʹ�� ���1 
#define CLE_EN GPIO_ResetBits(GPIO_EN, GPIO_EN_PIN) // ���0 
#define SET_RW GPIO_SetBits(GPIO_RW, GPIO_RW_PIN) //RW ��д ���1 
#define CLE_RW GPIO_ResetBits(GPIO_RW, GPIO_RW_PIN) // ���0 
#define SET_RS GPIO_SetBits(GPIO_RS, GPIO_RS_PIN) //RS ָ�� ���1 
#define CLE_RS GPIO_ResetBits(GPIO_RS, GPIO_RS_PIN) // ���0 
/******************************************************************/ 

void Delay_us(unsigned long CountLing)
{
signed char i;
while(CountLing--)
{
i = 10;
while(i--);
}
}

void Lcd_En_Toggle(void) //��ʹ�� ���� 
{ 
SET_EN; //ʹ��1 
Delay_us(2); //��ʱ160us 
CLE_EN; 
} 


void Lcd_Busy(void)//�ж�æ 
{ 

GPIO_InitTypeDef GPIO_InitStructure; 
RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE); //��DATA�˿�ʱ�� 

GPIO_InitStructure.GPIO_Pin = GPIO_DATA_7_PIN; // 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //����ģʽ = �������� 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ� 
GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure); //��æ���˿� 


//GPIO_WriteBit(GPIOB,GPIO_DATA_7_PIN,1);

CLE_RS; //RS = 0 
//Delay_us(1); //��ʱ10us 
SET_RW; //RW = 1 
//Delay_us(1); //��ʱ10us 
SET_EN; //EN = 1 
//GPIO_Write(GPIOB, 0xFF);  

DelayMs(10); //��ʱ160us 
while (GPIO_ReadInputDataBit(GPIOB,GPIO_DATA_7_PIN)); //ѭ���ȴ�æ���˿� = 0 
CLE_EN; //EN = 0 

//�ָ��˿�Ϊ���״̬ 
RCC_APB2PeriphClockCmd(RCC_GPIO_DATA, ENABLE); //��DATA�˿�ʱ�� 
GPIO_InitStructure.GPIO_Pin = GPIO_DATA_7_PIN; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //������� 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ� 
GPIO_Init(GPIO_DATA_7, &GPIO_InitStructure); 

} 

void Gpio_data_H(unsigned char x) //�˿��������� 
{ 

GPIO_ResetBits(GPIO_DATA_4, GPIO_DATA_4_PIN); //DB2 

GPIO_ResetBits(GPIO_DATA_5, GPIO_DATA_5_PIN); //DB5 
GPIO_ResetBits(GPIO_DATA_6, GPIO_DATA_6_PIN); //DB6 
GPIO_ResetBits(GPIO_DATA_7, GPIO_DATA_7_PIN); //DB7 

if(x&0X10)GPIO_SetBits(GPIO_DATA_4, GPIO_DATA_4_PIN);//DB2 

if(x&0X20)GPIO_SetBits(GPIO_DATA_5, GPIO_DATA_5_PIN);//DB5 
if(x&0X40)GPIO_SetBits(GPIO_DATA_6, GPIO_DATA_6_PIN);//DB6 
if(x&0X80)GPIO_SetBits(GPIO_DATA_7, GPIO_DATA_7_PIN);//DB7 
} 

void Gpio_data_L(unsigned char x) //�˿��������� 
{ 

GPIO_ResetBits(GPIO_DATA_4, GPIO_DATA_4_PIN); //DB2 

GPIO_ResetBits(GPIO_DATA_5, GPIO_DATA_5_PIN); //DB5 
GPIO_ResetBits(GPIO_DATA_6, GPIO_DATA_6_PIN); //DB6 
GPIO_ResetBits(GPIO_DATA_7, GPIO_DATA_7_PIN); //DB7 

if(x&0X1)GPIO_SetBits(GPIO_DATA_4, GPIO_DATA_4_PIN);//DB2 

if(x&0X2)GPIO_SetBits(GPIO_DATA_5, GPIO_DATA_5_PIN);//DB5 
if(x&0X4)GPIO_SetBits(GPIO_DATA_6, GPIO_DATA_6_PIN);//DB6 
if(x&0X8)GPIO_SetBits(GPIO_DATA_7, GPIO_DATA_7_PIN);//DB7 
} 

//��Һ������д��ָ�� ʱ��RS=L,RW=L,Data0-Data7=ָ���룬E=������ 
void Lcd_Write_Command(unsigned char x,char Busy) 
{ 
if(Busy) Lcd_Busy(); 
//Delay_us(1); //��ʱ10us 
CLE_RS; //RS = 0 
//Delay_us(1); //��ʱ10us 
CLE_RW; //RW = 0 
//Delay_us(4); //��ʱ40us 
//Gpio_data(x); //�˿��������� 
Gpio_data_H(x);
//Delay_us(4); //��ʱ40us 
Lcd_En_Toggle(); //��ʹ������ 

Gpio_data_L(x);
//Delay_us(4); //��ʱ40us 
Lcd_En_Toggle(); //��ʹ������ 
} 

void Lcd_Init( void ) //��ʼ�� 
{ 
DelayMs(100); //��ʱ15ms 

Lcd_Write_Command(0x38,0); // дָ��38H �����æ�ź� 
DelayMs(15); //��ʱ5ms 
Lcd_Write_Command(0x38,0); // дָ��38H �����æ�ź� 
DelayMs(15); //��ʱ5ms 
Lcd_Write_Command(0x38,0); // дָ��38H �����æ�ź� 
DelayMs(15); //��ʱ5ms 
Lcd_Write_Command(0x32,1); // дָ��32H
DelayMs(15);
Lcd_Write_Command( 0x28,1); //��ʾģʽ���� 
DelayMs(15);; //��ʱ5ms 
Lcd_Write_Command( 0x0C,1); //��ʾ������겻��ʾ 
DelayMs(15);; //��ʱ5ms
Lcd_Write_Command( 0x06,1); //��ʾ����ƶ����� 
DelayMs(15);; //��ʱ5ms 
Lcd_Write_Command( 0x01,1); //��ʾ���� 
DelayMs(15);; //��ʱ5ms 
} 
/******************************************************/ 

//��Һ������д������ ʱ��RS=H,RW=L,Data0-Data7=ָ���룬E=������ 

void Lcd_Write_Data( unsigned char x) //��Һ������д������ 
{ 
Lcd_Busy(); //��æ 
//Delay_us(1); //��ʱ10us 
SET_RS; //RS = 1 
//Delay_us(1); //��ʱ10us 
CLE_RW; //RW = 0 
//Gpio_data(x); //�˿��������� 
Gpio_data_H(x);
//Delay_us(4); //��ʱ40us 
Lcd_En_Toggle(); //��ʹ������ 

Gpio_data_L(x);
//Delay_us(4); //��ʱ40us 
Lcd_En_Toggle(); //��ʹ������ 

} 


void Lcd_SetXY(unsigned char x,unsigned char y) //�ַ���ʼλ���趨��x��ʾ�У�y��ʾ�� 
{ 
unsigned char addr; 
if(y==0) 
addr=0x80+x; 
else if(y==1) 
addr=0xC0+x; 
Lcd_Write_Command(addr,1) ; 
} 
/******************************************************/ 

void Lcd_Puts(unsigned char x,unsigned char y, unsigned char *string) //��1602дһ���ַ��� 
{ 
Lcd_SetXY(x,y); 
while(*string) 
{ 
Lcd_Write_Data(*string); 
string++; 
} 
} 

unsigned char strTemp2[3] = { 0 }; 
unsigned char strTemp3[4] = { 0 };  
unsigned char strTemp4[5] = { 0 }; 
unsigned char strTemp5[6] = { 0 }; 
unsigned char strTemp6[7] = { 0 }; 
unsigned char strTemp7[8] = { 0 }; 

unsigned char *tr_2(unsigned int outData_2) 
{ 
strTemp2[0]=(unsigned char)(((outData_2/10) % 10) + 0x30); 
strTemp2[1]=(unsigned char)((outData_2%10) + 0x30); 
return strTemp2; 
} 

unsigned char *tr_3(unsigned int outData3) 
{ 
strTemp3[0]=(unsigned char)(((outData3/100) % 10) + 0x30); 
strTemp3[1]=(unsigned char)(((outData3/10) % 10) + 0x30); 
strTemp3[2]=(unsigned char)((outData3%10) + 0x30); 
return strTemp3; 
} 



unsigned char *tr_4(unsigned int outData4) 
{ 
strTemp4[0]=(unsigned char)(((outData4/1000)% 10) + 0x30); 
strTemp4[1]=(unsigned char)(((outData4/100)% 10) + 0x30); 
strTemp4[2]=(unsigned char)(((outData4/10)% 10) + 0x30); 
strTemp4[3]=(unsigned char)((outData4%10) + 0x30); 
return strTemp4; 
} 


unsigned char *tr_5(unsigned int outData5) 
{ 
strTemp5[0]=(unsigned char)(((outData5/10000)% 10) + 0x30); 
strTemp5[1]=(unsigned char)(((outData5/1000)% 10) + 0x30); 
strTemp5[2]=(unsigned char)(((outData5/100)% 10) + 0x30); 
strTemp5[3]=(unsigned char)(((outData5/10)% 10) + 0x30); 
strTemp5[4]=(unsigned char)((outData5%10) + 0x30); 
return strTemp5; 
} 

unsigned char *tr_6(unsigned int outData6) 
{ 
strTemp6[0]=(unsigned char)(((outData6/100000)% 10) + 0x30); 
strTemp6[1]=(unsigned char)(((outData6/10000)% 10) + 0x30); 
strTemp6[2]=(unsigned char)(((outData6/1000)% 10) + 0x30); 
strTemp6[3]=(unsigned char)(((outData6/100)% 10) + 0x30); 
strTemp6[4]=(unsigned char)(((outData6/10)% 10) + 0x30); 
strTemp6[5]=(unsigned char)((outData6%10) + 0x30); 
return strTemp6; 
} 

unsigned char *tr_7(unsigned int outData7) 
{ 
strTemp7[0]=(unsigned char)(((outData7/1000000)% 10) + 0x30); 
strTemp7[1]=(unsigned char)(((outData7/100000)% 10) + 0x30); 
strTemp7[2]=(unsigned char)(((outData7/10000)% 10) + 0x30); 
strTemp7[3]=(unsigned char)(((outData7/1000)% 10) + 0x30); 
strTemp7[4]=(unsigned char)(((outData7/100)% 10) + 0x30); 
strTemp7[5]=(unsigned char)(((outData7/10)% 10) + 0x30); 
strTemp7[6]=(unsigned char)((outData7%10) + 0x30); 
return strTemp7; 
} 

void displayStatuse(DisplayRefreshLedStatuse_TypeDef status){

 switch(status)
    {
    case MT11:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode1 DelayTime1:");      
      tr_3(M1T1);
      Lcd_Puts(DispPos_X,1,strTemp3);
      break;      
    
    case speed:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Set Moto Speed:");      
      tr_2(MotoSpeed);
      Lcd_Puts(DispPos_X,1,strTemp2);     
      
      break;      
      
    case MT12:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode1 DelayTime2:");      
      tr_3(M1T2);
      Lcd_Puts(DispPos_X,1,strTemp3);
     
      break;  
      
    case MT13:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode1 DelayTime3:");      
      tr_3(M1T3);
      Lcd_Puts(DispPos_X,1,strTemp3);   
     
      break;      
      
    case MT14:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode1 DelayTime4:");      
      tr_3(M1T4);
      Lcd_Puts(DispPos_X,1,strTemp3);   

      break; 
      
    case MT21:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode2 DelayTime1:");      
      tr_3(M2T1);
      Lcd_Puts(DispPos_X,1,strTemp3);
 
      break;  
      
    case MT31:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode3 DelayTime1:");      
      tr_3(M3T1);
      Lcd_Puts(DispPos_X,1,strTemp3);   
    
      break;      
      
    case MT32:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode3 DelayTime2:");      
      tr_3(M3T2);
      Lcd_Puts(DispPos_X,1,strTemp3); 

      break;  
      
    case MT33:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode3 DelayTime3:");      
      tr_3(M3T3);
      Lcd_Puts(DispPos_X,1,strTemp3);    

      break;      
      
    case MT34:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode3 DelayTime4:");      
      tr_3(M3T4);
      Lcd_Puts(DispPos_X,1,strTemp3);  

      break;  
     
    case MT41:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Mode4 DelayTime1:");      
      tr_3(M4T1);
      Lcd_Puts(DispPos_X,1,strTemp3);  
      
      break;  
      
    case Step:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"DelayTime step:");      
      tr_2(SetInc);
      Lcd_Puts(DispPos_X,1,strTemp2);  
      
      break;
      
    case ModeDisplay:
      Lcd_Write_Command( 0x01,1); //��ʾ���� 
      //DelayMs(15); //��ʱ5ms 
      Lcd_Puts(0,0,"Chose mode:");   
      
      tr_2(mode);
      Lcd_Puts(DispPos_X,1,strTemp2);  
      
      break;
      
    default:
      break;
    }
}
