#include "stm32f10x.h"
#include "bsp_timer.h"

/********************************Ӳ���ӿڶ���*********************************/ 
#define RS GPIO_Pin_10 //; 
#define RW GPIO_Pin_11 //; 
#define EN GPIO_Pin_12 //; 

#define D0 GPIO_Pin_2 //; 
#define D1 GPIO_Pin_3 //; 
#define D2 GPIO_Pin_4 //; 
#define D3 GPIO_Pin_5 //; 
#define D4 GPIO_Pin_8 //; 
#define D5 GPIO_Pin_9 //; 
#define D6 GPIO_Pin_7 //; 
#define D7 GPIO_Pin_6 //; 

/********************************�궨��*********************************/ 
#define LCD_RS(x) x ? GPIO_SetBits(GPIOC, RS): GPIO_ResetBits(GPIOC, RS) 
#define LCD_RW(x) x ? GPIO_SetBits(GPIOC, RW): GPIO_ResetBits(GPIOC, RW) 
#define LCD_EN(x) x ? GPIO_SetBits(GPIOC, EN): GPIO_ResetBits(GPIOC, EN) 
/*
#define LCD_D1(x) x ? GPIO_SetBits(GPIOB, D1): GPIO_ResetBits(GPIOB, D1) 
#define LCD_D2(x) x ? GPIO_SetBits(GPIOB, D2): GPIO_ResetBits(GPIOB, D2) 
#define LCD_D3(x) x ? GPIO_SetBits(GPIOB, D3): GPIO_ResetBits(GPION, D3) 
#define LCD_D4(x) x ? GPIO_SetBits(GPIOB, D4): GPIO_ResetBits(GPIOB, D4) 
#define LCD_D5(x) x ? GPIO_SetBits(GPIOB, D5): GPIO_ResetBits(GPIOB, D5) 
#define LCD_D6(x) x ? GPIO_SetBits(GPIOB, D6): GPIO_ResetBits(GPION, D6) 
#define LCD_D7(x) x ? GPIO_SetBits(GPIOB, D7): GPIO_ResetBits(GPIOB, D7) 
#define LCD_D0(x) x ? GPIO_SetBits(GPIOD, D0): GPIO_ResetBits(GPIOD, D0) 

#define DATAOUT GPIOE->ODR //PE[8..15]--(LCD)D0~D7 
*/
#define GPIO_DATA_0 GPIOD // ������0_�˿��� 
#define GPIO_DATA_0_PIN GPIO_Pin_2 // ������0_�˿ں� 
#define GPIO_DATA_1 GPIOB // ������1_�˿��� 
#define GPIO_DATA_1_PIN GPIO_Pin_3 // ������1_�˿ں� 
#define GPIO_DATA_2 GPIOB // ������2_�˿��� 
#define GPIO_DATA_2_PIN GPIO_Pin_4 // ������2_�˿ں� 
#define GPIO_DATA_3 GPIOB // ������3_�˿��� 
#define GPIO_DATA_3_PIN GPIO_Pin_5 // ������3_�˿ں� 
#define GPIO_DATA_4 GPIOB // ������4_�˿��� 
#define GPIO_DATA_4_PIN GPIO_Pin_8 // ������4_�˿ں� 
#define GPIO_DATA_5 GPIOB // ������5_�˿��� 
#define GPIO_DATA_5_PIN GPIO_Pin_9 // ������5_�˿ں� 
#define GPIO_DATA_6 GPIOB // ������6_�˿��� 
#define GPIO_DATA_6_PIN GPIO_Pin_7 // ������6_�˿ں� 
#define GPIO_DATA_7 GPIOB // ������7_�˿��� 
#define GPIO_DATA_7_PIN GPIO_Pin_6 // ������7_�˿ں� 
#define RCC_GPIO_DATA RCC_APB2Periph_GPIOB // ������ʱ���� 
/******************************** �������� ---------------------------------------------------------*/ 



typedef unsigned char BYTE; 
typedef unsigned char BOOL; 

BYTE dis1[] = {" WELCOME TO "}; 
BYTE dis2[] = {" WWW.PRECHIN.COM "}; 
BYTE dis3[] = {" OT EMOCLEW "}; 
BYTE dis4[] = {" MOC.NIHCERP.WWW "}; 

/*********************************�������� -----------------------------------------------*/ 
void NOP(void); 
static void LCD_Cfg_Data(u8 in); 


/*******************************************��ʱ����**********************************/ 
void NOP(void) 
{ vu16 i; 
for(i=0; i<100; i++); 
} 

/*******************************************��ʱ����**********************************/ 
void delay(vu32 nCount) 
{ 
for(; nCount != 0; nCount--); 
} 

void Gpio_data(unsigned char x) //�˿��������� 
{ 
GPIO_ResetBits(GPIO_DATA_0, GPIO_DATA_0_PIN); //DB0 
GPIO_ResetBits(GPIO_DATA_1, GPIO_DATA_1_PIN); //DB1 
GPIO_ResetBits(GPIO_DATA_2, GPIO_DATA_2_PIN); //DB2 
GPIO_ResetBits(GPIO_DATA_3, GPIO_DATA_3_PIN); //DB3 
GPIO_ResetBits(GPIO_DATA_4, GPIO_DATA_4_PIN); //DB4 
GPIO_ResetBits(GPIO_DATA_5, GPIO_DATA_5_PIN); //DB5 
GPIO_ResetBits(GPIO_DATA_6, GPIO_DATA_6_PIN); //DB6 
GPIO_ResetBits(GPIO_DATA_7, GPIO_DATA_7_PIN); //DB7 
if(x&0X01)GPIO_SetBits(GPIO_DATA_0, GPIO_DATA_0_PIN);//DB0 
if(x&0X02)GPIO_SetBits(GPIO_DATA_1, GPIO_DATA_1_PIN);//DB1 
if(x&0X04)GPIO_SetBits(GPIO_DATA_2, GPIO_DATA_2_PIN);//DB2 
if(x&0X08)GPIO_SetBits(GPIO_DATA_3, GPIO_DATA_3_PIN);//DB3 
if(x&0X10)GPIO_SetBits(GPIO_DATA_4, GPIO_DATA_4_PIN);//DB4 
if(x&0X20)GPIO_SetBits(GPIO_DATA_5, GPIO_DATA_5_PIN);//DB5 
if(x&0X40)GPIO_SetBits(GPIO_DATA_6, GPIO_DATA_6_PIN);//DB6 
if(x&0X80)GPIO_SetBits(GPIO_DATA_7, GPIO_DATA_7_PIN);//DB7 
} 

/******************************��æ*************************************************/ 
/********************************************************************* 
*�� �ƣ�LCD_busy() 
*�� �ܣ���LCDæµ״̬,ֱ��LCD1602��æΪֹ 
*��ڲ������� 
*���ڲ������޷����ֽ����ͣ�����0��ʾLCD�Ѿ�������ϣ����С� 
*˵ ����һ����д�������������֮ǰ��while(LCD_busy())���ȴ�LCD���� 
*********************************************************************/ 
BOOL lcd_bz(void) 
{ 
BOOL result; 
LCD_Cfg_Data(1); 
LCD_RS(0);; 
// delay_us(1); 
LCD_RW(1); 
// delay_us(1); 
LCD_EN(1); 
//delay_us(300); 
DelayMs(1);
while (GPIO_ReadInputDataBit(GPIO_DATA_7,GPIO_DATA_7_PIN)); //ѭ���ȴ�æ���˿� = 0 
LCD_EN(0); 
//return result; 
} 
/******************************************* 

************************** 
*�� �ܣ�д��LCD��ʼ��ʱ����������Լ��æ״̬ 
*********************************************************************/ 
void LCD_WriteInitcmd(uint8_t cmd) 
{ 
LCD_Cfg_Data(0); 
LCD_RS(0); 
// delay_us(1); 
LCD_RW(0); 
// delay_us(1); 
LCD_EN(0); 
// delay_us(300); 
//DATAOUT = cmd<<8; 
Gpio_data(cmd); 
LCD_EN(1); 
//delay_us(10); 
DelayMs(1);
LCD_EN(0); 
DelayMs(2); 
} 
/******************************д����*************************************************/ 
/********************************************************************* 
*�� �ƣ�LCD_Writecmd() 
*�� �ܣ�дָ�LCD1602,ָ������ռһ���ֽ� 
*��ڲ�����cmd:��д���ָ�� 
*���ڲ������� 
*********************************************************************/ 
void lcd_wcmd(uint8_t cmd) 
{ 
while(lcd_bz()); 
LCD_Cfg_Data(0); 
LCD_RS(0); //��ͬһ���Ĵ���������д�룬�м���ʱһ�� 
// delay_us(1); 
LCD_RW(0); 
// delay_us(1); 
LCD_EN(0); 
// delay_us(300);//�������� 
//DATAOUT = cmd<<8; 
Gpio_data(cmd); 
LCD_EN(1); 
//delay_us(10);//��Ҫ����ʱ 
DelayMs(1);
LCD_EN(0); //�½��أ�LCD1602��ʼ���� 
} 
/********************************************************************* 
*�� �ƣ�LCD_Writedata() 
*�� �ܣ�дһ�ֽ����ݵ�LCD1602 
*��ڲ�����dat���޷����ֽ����ͣ�0~255 ��������ASCII���ַ� 
*���ڲ������� 
*********************************************************************/ 
void lcd_wdat(uint8_t dat) 
{ 
while(lcd_bz());//�ȴ�LCD1602���� 
LCD_Cfg_Data(0); 
LCD_RS(1); 
// delay_us(1); 
LCD_RW(0); 
// delay_us(1); 
LCD_EN(0); 
// delay_us(1); 
Gpio_data(dat); 
LCD_EN(1); //������ 
//delay_us(10); //����Ҫ����ʱ�������ԣ���ʱ300us���ϲſ��� DelayMs(1);
DelayMs(1);
LCD_EN(0); //�½��أ���ʼд����Ч���� 
} 

/********************************************************************* 
*�� �ƣ�LCD_pos() 
*�� �ܣ��趨��ʾλ�� 
*��ڲ�����pos:��ʾλ��,ֵ�ķ�Χ���£� 
* 0x00----------------0x0f 0x10-------0x27 ��һ�У�һ����ʾ16���ַ��� 
* 0x40----------------0x4f 0x50-------0x67 �ڶ��� 
*���ڲ������� 
*********************************************************************/ 
void LCD_pos(uint8_t pos) 
{ 
lcd_wcmd(pos | 0x80); 
} 
/********************************************************************* 
*�� �ƣ�LCD_Setpos() 
*�� �ܣ�����ϰ���趨��ʾλ�� 
*��ڲ�����row:�У�row=1��ʾ��һ�У�row=2��ʾ�ڶ��� 
* col:�У�0~15������ָ����ʾ���У���Χ������0~40 
*���ڲ������� 
*********************************************************************/ 
void LCD_Setpos(uint8_t row,uint8_t col) 
{ 
if(row==1) lcd_wcmd(col | 0x80); 
else lcd_wcmd(col | 0xC0); 
} 
/********************************************************************* 


*�� �ܣ���ʾһ���ַ� 
*�� �ڣ�ch������ʾ���ַ� 
*********************************************************************/ 
void LCD_DispChar(char ch) 
{ 
lcd_wdat(ch); 
} 
/********************************************************************* 
*�� �ܣ���ָ��λ����ʾһ���ַ� 
*�� �ڣ�row:�� 1��2 col:�У�0~15 
* ch������ʾ���ַ� 
*********************************************************************/ 
void LCD_Setpos_DispChar(uint8_t row,uint8_t col,char ch) 
{ 
LCD_Setpos(row,col); 
lcd_wdat(ch); 
} 
/********************************************************************* 
*�� �ƣ�LCD_DispString() 
*�� �ܣ�ʹLCD1602��ʾһ���ַ�������ʾλ������ǰ�趨 
*��ڲ�����str[]:����ʾ���ַ��� 
*���ڲ������� 
*********************************************************************/ 
void LCD_DispString(char str[]) 
{ 
uint8_t i=0; 
while(str[i] != '\0') 
{ 
lcd_wdat(str[i]); 
++i; 
} 
} 
/********************************************************************* 
*�� �ƣ�LCD_Setpos_DispString() 
*�� �ܣ�ʹLCD1602��ָ��λ�ÿ�ʼ��ʾһ���ַ��� 
*��ڲ�����row:��ʾ�У�1��2 
* col:��ʾ�У�0~15 ��0~40�� 
* str[]:����ʾ���ַ��� 
*���ڲ������� 
*˵ ����ָ��λ������ʾ�ĳ�ʼλ�ã���һ���ַ���ʾ��λ�� 
*********************************************************************/ 
void LCD_Setpos_DispString(uint8_t row,uint8_t col,char str[]) 
{ 
LCD_Setpos(row,col); 
LCD_DispString(str); 
} 
/********************************************************************* 
*�� �ƣ�LCD_Dispnum() 
*�� �ܣ���ʾһ��������8λ����������ʾλ������ǰ���� 
*��ڲ�����num:����ʾ��������������8λ 
*���ڲ������� 
*********************************************************************/ 
void LCD_Dispnum(uint32_t num) 
{ 
uint8 i=0,j,k=0,wei,q; 
char str[9]; 
if(num>=10000000)wei=8; 
else if(num>=1000000)wei=7; 
else if(num>=100000)wei=6; 
else if(num>=10000)wei=5; 
else if(num>=1000) wei=4; 
else if(num>=100)wei=3; 
else if(num>=10) wei=2; 
else wei=1; 
for(i=wei;i>0;i--) 
{ q=1; 
j=1; //i=1ʱ��q=1,�õ���λ 
for(;j<i;j++)q *=10; 
str[k++]=num/q +'0'; 
num %= q; 
} 
str[k] = '\0'; //����ַ���������־ 
LCD_DispString(str);//��ʾ�ַ��� 
} 
/********************************************************************* 
*�� �ƣ�LCD_Setpos_Dispnum() 
*�� �ܣ���ָ��λ����ʾһ��������8λ������ 
*��ڲ�����num:����ʾ��������������8λ 
*���ڲ������� 
*********************************************************************/ 
void LCD_Setpos_Dispnum(uint8_t row,uint8_t col,uint32_t num) 
{ 
LCD_Setpos(row,col); 
LCD_Dispnum(num); 
} 
/********************************************************************* 

void LCD_DispDecimal(uint32_t num,uint8_t dot) 
{ 
uint8 i=0,j,k=0,wei,q; 
char str[10]; 
if(num>=10000000)wei=8; 
else if(num>=1000000)wei=7; 
else if(num>=100000)wei=6; 
else if(num>=10000)wei=5; 
else if(num>=1000) wei=4; 
else if(num>=100)wei=3; 
else if(num>=10) wei=2; 
else wei=1; 
for(i=wei;i>0;i--) 
{ q=1; 
j=1; //i=1ʱ��q=1,�õ���λ 
for(;j<i;j++)q *=10; 
str[k++]=num/q +'0'; 
num %= q; 
} 
str[k] = '\0'; //����ַ���������־ 
for(i=8;i>0;i--) 
{ if((str[i]>='0')&&(str[i]<='9')) break; 
} 
str[i+2]='\0'; //����ַ��������� 
for(j=0;j?j++,i--)//С������λ�������ƣ�׼������ 
{ 
str[i+1]=str[i]; 
} 
str[i+1]='.'; //����С���� 
LCD_DispString(str);//��ʾ����С�� 
} 
*/ 
/********************************************************************* 
*�� �ƣ�LCD_DispDateTime() 
*�� �ܣ���ʾ��������ʾ������ʱ�� 
*��ڲ�����year:��,����������ֵ 
* month:�£�1~12 
* day:�գ�1~31 
* hour:ʱ��1~24 �� 1~12 
* min:�֣�0~59 
* sec:�룬0~59 
*���ڲ������� 
*˵ ����ʹ��ǰ�����Ⱥ궨��USE_LCD_DISP_DATE���ڰ����ļ�֮ǰ������Ч 
*��ʾ��ʽ��Date:yyyy-mm-dd * 
* Time:hh:mm:ss * 
*********************************************************************/ 
void LCD_DispDateTime(uint32_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec) 
{ 
LCD_Setpos(1,0); 
LCD_DispString("Date:"); 
LCD_Dispnum((uint32_t)year); 
LCD_DispChar('-'); 
LCD_Dispnum((uint32_t)month); 
LCD_DispChar('-'); 
LCD_Dispnum((uint32_t)day); 
LCD_Setpos(1,15); 
LCD_DispChar('*'); //��һ�н�������ʾ 
LCD_Setpos(2,0); 
LCD_DispString("Time:"); 
LCD_Dispnum((uint32_t)hour); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)min); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)sec); 
LCD_Setpos(2,15); 
LCD_DispChar('*'); //�ڶ��н����� ��ʾ 
} 
/********************************************************************* 
*�� �ƣ�LCD_DispStopWatch() 
*�� �ܣ���ʾ�����ʾʱ���֣��룬10���룬��ȷ��10ms 
*��ڲ�����hour:ʱ��1~24 �� 1~12 
* min:�֣�0~59 
* sec:�룬0~59 
* tenms:10ms����ֵ����3��ʾ30ms 
*���ڲ������� 
*��ʾ��ʽ��Current Time: * 
* hh:mm:ss:tt * 
*********************************************************************/ 
void LCD_DispStopWatch(uint8_t hour,uint8_t min,uint8_t sec,uint8_t tenms) 
{ 
LCD_Setpos(1,0); 
LCD_DispString("Current Time:"); 
LCD_Setpos(1,15); 
LCD_DispChar('*'); //��һ�н�������ʾ 
LCD_Setpos(2,2); 
LCD_Dispnum((uint32_t)hour); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)min); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)sec); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)tenms); 
LCD_Setpos(2,15); 
LCD_DispChar('*'); //�ڶ��н�������ʾ 
} 
/********************************************************************* 
*�� �ƣ�LCD_DispFreq() 
*�� �ܣ���ʾƵ�ʼƣ���̬�Զ�����Ƶ����ʾ 
*��ڲ�����freq:������ʾ8λ������ʾ��Ƶ��ֵ��Ƶ�ʷ�Χ1Hz~99MHz 
*���ڲ������� 
*˵ ������ڵ�Ƶ��ֵ��λ��Hz����ʾ�����Զ�ѡ����ʵ���ʾ��ʽ 
* freq>=1000000��ʾ��6λС����MHz 
* else freq>=1000 ��ʾ��3λС����KHz 
* else freq>=0 Ĭ����ʾ��λ��Hz ���������以�⣩ 
********************************************************************
void LCD_DispFreq(uint32_t freq) 
{ 
LCD_Setpos(1,0); 
LCD_DispString("Current Freq:"); 
LCD_Setpos(2,1); //��ʾλ�� 
if(freq>=1000000) 
{ 
LCD_DispDecimal(freq,6); 
LCD_DispString("MHz"); 
} 
else if(freq>=1000) 
{ 
LCD_DispDecimal(freq,3); 
LCD_DispString("KHz"); 
} 
else { LCD_Dispnum(freq); 
LCD_DispString("Hz"); 
} 
LCD_Setpos(2,14); 
LCD_DispString("OK"); 
} 
//��Ļ��������һ�����ڹ�����ʾ 
void LCD_ShiftLeft() 
{ 
lcd_wcmd(0x18); 
} 
//��Ļ��������һ�����ڹ�����ʾ 
void LCD_ShiftRight(void) 
{ 
lcd_wcmd(0x1C); 
} 

//ʹ��껹��ԭ��λ�� 
void LCD_Return(void) 
{ 
lcd_wcmd(0x02); 
} 
//����ʾ 
void LCD_Close(void) 
{ 
lcd_wcmd(0x08); 
} 
//����ʾ 
void LCD_Open(void) 
{ 
lcd_wcmd(0x0C); 
} 
*/ 
/********************************************************************* 
*�� �ƣ�LCD_FlickerChar() 
*�� �ܣ�ʹָ��λ���ַ���˸������ʾ��� 
*��ڲ�����row:�� 1,2 
* col:�� 0~15 
*���ڲ������� 
*˵ ����д����0x0D����ʾ������˸��д����0x0F����ʾ������˸ 
* һ���趨��˸�󣬻����λ�ñ仯��˸������˸д����0x0C 
*********************************************************************/ 
void LCD_FlickerChar(uint8_t row,uint8_t col 

) 
{ 
lcd_wcmd(0x0D); 
LCD_Setpos(row,col); 
} 
//�ر��ַ���˸ 
void LCD_CloseFlicker(void) 
{ 
lcd_wcmd(0x0C); 
} 
//��Ļ����˸һ�� 
void LCD_FlickerScreen(void) 
{ 
lcd_wcmd(0x08);//����ʾ 
DelayMs(500); 
lcd_wcmd(0x0C);//����ʾ 
DelayMs(500); 
} 
/********************************************************************* 
*�� �ƣ�LCD_Init() 
*�� �ܣ���ʼ��LCD1602 
*��ڲ������� 
*���ڲ������� 
*�������0x38:16*2��ʾ��5*&������ʾ�ַ���8λ����;ָ��ִ��ʱ��40us 
* 0x0C:����ʾ���ع�� 40us 
* 0x08:����ʾ���ع�� 40us 
* 0x0D:�ַ���˸���ع�꣬�����趨λ�ã���˸����0.4ms���� 
* 0x0F:�ַ���˸������� 
* 0x06:д�������Զ����ƹ�꣬��ͨ���Σ�����������ʾ 40us 
* 0x04:д�������Զ����ƹ�꣬���Դ���������ʾ 40us 
* 0x01:�����ʾ���ݣ������� 1.64ms 
* 0x02:ʹ��껹����ʼλ�� 1.64ms 
* 0x18:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us 
* 0x1C:��Ļ�������ַ�ͬʱ����һ���ʺϹ�����ʾ 40us 
*********************************************************************/ 
void lcd_init(void) 
{ 
DelayMs(100); //��ʱ20ms 
LCD_WriteInitcmd(0x38); //16*2��ʾ��5*7����8λ���� 
DelayMs(1); 
LCD_WriteInitcmd(0x38); //16*2��ʾ��5*7����8λ���� 
DelayMs(1); 
LCD_WriteInitcmd(0x38); //16*2��ʾ��5*7����8λ���� 
DelayMs(1); 
LCD_WriteInitcmd(0x08); //�ȹ���ʾ������ʾ 
DelayMs(1); 
LCD_WriteInitcmd(0x06); //�Զ����ƹ��,0x04Ϊ���ƹ�� 
DelayMs(1); 
LCD_WriteInitcmd(0x0c); //��ʾ�����ع��;0x08Ϊ����ʾ 
DelayMs(1); 
LCD_WriteInitcmd(0x01); //���LCD����ʾ���� 
DelayMs(1); 
} 

void LCD_GPIO_CONFIG(void) 
{ 
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOC, ENABLE); 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ; // 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //��������Ƶ��Ϊ50HZ 
GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��GPIOC�˿� 


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //����GPIOΪͬһ���Ͷ˿� 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //��������Ƶ��Ϊ50HZ 
GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��GPIOE�˿� */ 

} 
static void LCD_Cfg_Data(u8 in) 
{ 
if (in) 
{ 
//PORTE8~PORTE15 ���������� 
GPIOE->CRH = 0x88888888; 
GPIOE->ODR |= 0xFF00; 
} 
else 
{ 
//PORTE8~PORTE15 ��� 
GPIOE->CRH = 0x33333333; 
GPIOE->ODR |= 0xFF00; 
} 
} 



void LCD_TEST(void) 
{ 
char str[]={"happy!"}; 
LCD_Setpos_DispString(1,3,str); 

LCD_FlickerChar( 2,3); 
LCD_FlickerScreen(); 
LCD_FlickerScreen(); 
LCD_FlickerScreen(); 
DelayMs(100); 
LCD_CloseFlicker(); 
} 