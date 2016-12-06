#include "stm32f10x.h"
#include "bsp_timer.h"

/********************************硬件接口定义*********************************/ 
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

/********************************宏定义*********************************/ 
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
#define GPIO_DATA_0 GPIOD // 数据线0_端口组 
#define GPIO_DATA_0_PIN GPIO_Pin_2 // 数据线0_端口号 
#define GPIO_DATA_1 GPIOB // 数据线1_端口组 
#define GPIO_DATA_1_PIN GPIO_Pin_3 // 数据线1_端口号 
#define GPIO_DATA_2 GPIOB // 数据线2_端口组 
#define GPIO_DATA_2_PIN GPIO_Pin_4 // 数据线2_端口号 
#define GPIO_DATA_3 GPIOB // 数据线3_端口组 
#define GPIO_DATA_3_PIN GPIO_Pin_5 // 数据线3_端口号 
#define GPIO_DATA_4 GPIOB // 数据线4_端口组 
#define GPIO_DATA_4_PIN GPIO_Pin_8 // 数据线4_端口号 
#define GPIO_DATA_5 GPIOB // 数据线5_端口组 
#define GPIO_DATA_5_PIN GPIO_Pin_9 // 数据线5_端口号 
#define GPIO_DATA_6 GPIOB // 数据线6_端口组 
#define GPIO_DATA_6_PIN GPIO_Pin_7 // 数据线6_端口号 
#define GPIO_DATA_7 GPIOB // 数据线7_端口组 
#define GPIO_DATA_7_PIN GPIO_Pin_6 // 数据线7_端口号 
#define RCC_GPIO_DATA RCC_APB2Periph_GPIOB // 数据线时钟组 
/******************************** 变量定义 ---------------------------------------------------------*/ 



typedef unsigned char BYTE; 
typedef unsigned char BOOL; 

BYTE dis1[] = {" WELCOME TO "}; 
BYTE dis2[] = {" WWW.PRECHIN.COM "}; 
BYTE dis3[] = {" OT EMOCLEW "}; 
BYTE dis4[] = {" MOC.NIHCERP.WWW "}; 

/*********************************声明函数 -----------------------------------------------*/ 
void NOP(void); 
static void LCD_Cfg_Data(u8 in); 


/*******************************************延时函数**********************************/ 
void NOP(void) 
{ vu16 i; 
for(i=0; i<100; i++); 
} 

/*******************************************延时函数**********************************/ 
void delay(vu32 nCount) 
{ 
for(; nCount != 0; nCount--); 
} 

void Gpio_data(unsigned char x) //端口置入数据 
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

/******************************测忙*************************************************/ 
/********************************************************************* 
*名 称：LCD_busy() 
*功 能：读LCD忙碌状态,直到LCD1602不忙为止 
*入口参数：无 
*出口参数：无符号字节类型，返回0表示LCD已经操作完毕，空闲。 
*说 明：一般在写入新命令或数据之前用while(LCD_busy())语句等待LCD就绪 
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
while (GPIO_ReadInputDataBit(GPIO_DATA_7,GPIO_DATA_7_PIN)); //循环等待忙检测端口 = 0 
LCD_EN(0); 
//return result; 
} 
/******************************************* 

************************** 
*功 能：写入LCD初始化时的命令，不可以检测忙状态 
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
/******************************写命令*************************************************/ 
/********************************************************************* 
*名 称：LCD_Writecmd() 
*功 能：写指令到LCD1602,指令数据占一个字节 
*入口参数：cmd:待写入的指令 
*出口参数：无 
*********************************************************************/ 
void lcd_wcmd(uint8_t cmd) 
{ 
while(lcd_bz()); 
LCD_Cfg_Data(0); 
LCD_RS(0); //对同一个寄存器的两次写入，中间延时一会 
// delay_us(1); 
LCD_RW(0); 
// delay_us(1); 
LCD_EN(0); 
// delay_us(300);//产生脉冲 
//DATAOUT = cmd<<8; 
Gpio_data(cmd); 
LCD_EN(1); 
//delay_us(10);//必要的延时 
DelayMs(1);
LCD_EN(0); //下降沿，LCD1602开始工作 
} 
/********************************************************************* 
*名 称：LCD_Writedata() 
*功 能：写一字节数据到LCD1602 
*入口参数：dat：无符号字节类型，0~255 包括各个ASCII码字符 
*出口参数：无 
*********************************************************************/ 
void lcd_wdat(uint8_t dat) 
{ 
while(lcd_bz());//等待LCD1602空闲 
LCD_Cfg_Data(0); 
LCD_RS(1); 
// delay_us(1); 
LCD_RW(0); 
// delay_us(1); 
LCD_EN(0); 
// delay_us(1); 
Gpio_data(dat); 
LCD_EN(1); //先拉高 
//delay_us(10); //很重要的延时，经调试，延时300us以上才可以 DelayMs(1);
DelayMs(1);
LCD_EN(0); //下降沿，开始写入有效数据 
} 

/********************************************************************* 
*名 称：LCD_pos() 
*功 能：设定显示位置 
*入口参数：pos:显示位置,值的范围如下： 
* 0x00----------------0x0f 0x10-------0x27 第一行（一次显示16个字符） 
* 0x40----------------0x4f 0x50-------0x67 第二行 
*出口参数：无 
*********************************************************************/ 
void LCD_pos(uint8_t pos) 
{ 
lcd_wcmd(pos | 0x80); 
} 
/********************************************************************* 
*名 称：LCD_Setpos() 
*功 能：根据习惯设定显示位置 
*入口参数：row:行，row=1表示第一行，row=2表示第二行 
* col:列，0~15，用于指定显示的列，范围可以是0~40 
*出口参数：无 
*********************************************************************/ 
void LCD_Setpos(uint8_t row,uint8_t col) 
{ 
if(row==1) lcd_wcmd(col | 0x80); 
else lcd_wcmd(col | 0xC0); 
} 
/********************************************************************* 


*功 能：显示一个字符 
*入 口：ch：待显示的字符 
*********************************************************************/ 
void LCD_DispChar(char ch) 
{ 
lcd_wdat(ch); 
} 
/********************************************************************* 
*功 能：在指定位置显示一个字符 
*入 口：row:行 1或2 col:列，0~15 
* ch：待显示的字符 
*********************************************************************/ 
void LCD_Setpos_DispChar(uint8_t row,uint8_t col,char ch) 
{ 
LCD_Setpos(row,col); 
lcd_wdat(ch); 
} 
/********************************************************************* 
*名 称：LCD_DispString() 
*功 能：使LCD1602显示一个字符串，显示位置需提前设定 
*入口参数：str[]:待显示的字符串 
*出口参数：无 
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
*名 称：LCD_Setpos_DispString() 
*功 能：使LCD1602从指定位置开始显示一个字符串 
*入口参数：row:显示行，1或2 
* col:显示列，0~15 （0~40） 
* str[]:待显示的字符串 
*出口参数：无 
*说 明：指定位置是显示的初始位置，第一个字符显示的位置 
*********************************************************************/ 
void LCD_Setpos_DispString(uint8_t row,uint8_t col,char str[]) 
{ 
LCD_Setpos(row,col); 
LCD_DispString(str); 
} 
/********************************************************************* 
*名 称：LCD_Dispnum() 
*功 能：显示一个不超过8位的整数，显示位置需提前设置 
*入口参数：num:待显示的整数，不超过8位 
*出口参数：无 
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
j=1; //i=1时，q=1,得到个位 
for(;j<i;j++)q *=10; 
str[k++]=num/q +'0'; 
num %= q; 
} 
str[k] = '\0'; //添加字符串结束标志 
LCD_DispString(str);//显示字符串 
} 
/********************************************************************* 
*名 称：LCD_Setpos_Dispnum() 
*功 能：在指定位置显示一个不超过8位的整数 
*入口参数：num:待显示的整数，不超过8位 
*出口参数：无 
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
j=1; //i=1时，q=1,得到个位 
for(;j<i;j++)q *=10; 
str[k++]=num/q +'0'; 
num %= q; 
} 
str[k] = '\0'; //添加字符串结束标志 
for(i=8;i>0;i--) 
{ if((str[i]>='0')&&(str[i]<='9')) break; 
} 
str[i+2]='\0'; //添加字符串结束符 
for(j=0;j?j++,i--)//小数点后各位依次右移，准备插入 
{ 
str[i+1]=str[i]; 
} 
str[i+1]='.'; //插入小数点 
LCD_DispString(str);//显示浮点小数 
} 
*/ 
/********************************************************************* 
*名 称：LCD_DispDateTime() 
*功 能：显示日历，显示日期与时间 
*入口参数：year:年,可以是任意值 
* month:月，1~12 
* day:日，1~31 
* hour:时，1~24 或 1~12 
* min:分，0~59 
* sec:秒，0~59 
*出口参数：无 
*说 明：使用前必须先宏定义USE_LCD_DISP_DATE，在包含文件之前定义有效 
*显示格式：Date:yyyy-mm-dd * 
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
LCD_DispChar('*'); //第一行结束符显示 
LCD_Setpos(2,0); 
LCD_DispString("Time:"); 
LCD_Dispnum((uint32_t)hour); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)min); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)sec); 
LCD_Setpos(2,15); 
LCD_DispChar('*'); //第二行结束符 显示 
} 
/********************************************************************* 
*名 称：LCD_DispStopWatch() 
*功 能：显示秒表，显示时，分，秒，10毫秒，精确到10ms 
*入口参数：hour:时，1~24 或 1~12 
* min:分，0~59 
* sec:秒，0~59 
* tenms:10ms计数值，如3表示30ms 
*出口参数：无 
*显示格式：Current Time: * 
* hh:mm:ss:tt * 
*********************************************************************/ 
void LCD_DispStopWatch(uint8_t hour,uint8_t min,uint8_t sec,uint8_t tenms) 
{ 
LCD_Setpos(1,0); 
LCD_DispString("Current Time:"); 
LCD_Setpos(1,15); 
LCD_DispChar('*'); //第一行结束符显示 
LCD_Setpos(2,2); 
LCD_Dispnum((uint32_t)hour); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)min); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)sec); 
LCD_DispChar(':'); 
LCD_Dispnum((uint32_t)tenms); 
LCD_Setpos(2,15); 
LCD_DispChar('*'); //第二行结束符显示 
} 
/********************************************************************* 
*名 称：LCD_DispFreq() 
*功 能：显示频率计，动态自动调整频率显示 
*入口参数：freq:可以显示8位整数表示的频率值，频率范围1Hz~99MHz 
*出口参数：无 
*说 明：入口的频率值单位是Hz，显示函数自动选择合适的显示方式 
* freq>=1000000显示成6位小数的MHz 
* else freq>=1000 显示成3位小数的KHz 
* else freq>=0 默认显示单位是Hz （三个区间互斥） 
********************************************************************
void LCD_DispFreq(uint32_t freq) 
{ 
LCD_Setpos(1,0); 
LCD_DispString("Current Freq:"); 
LCD_Setpos(2,1); //显示位置 
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
//屏幕整体左移一格，用于滚动显示 
void LCD_ShiftLeft() 
{ 
lcd_wcmd(0x18); 
} 
//屏幕整体右移一格，用于滚动显示 
void LCD_ShiftRight(void) 
{ 
lcd_wcmd(0x1C); 
} 

//使光标还回原点位置 
void LCD_Return(void) 
{ 
lcd_wcmd(0x02); 
} 
//关显示 
void LCD_Close(void) 
{ 
lcd_wcmd(0x08); 
} 
//开显示 
void LCD_Open(void) 
{ 
lcd_wcmd(0x0C); 
} 
*/ 
/********************************************************************* 
*名 称：LCD_FlickerChar() 
*功 能：使指定位置字符闪烁，不显示光标 
*入口参数：row:行 1,2 
* col:列 0~15 
*出口参数：无 
*说 明：写命令0x0D不显示光标的闪烁，写命令0x0F是显示光标的闪烁 
* 一旦设定闪烁后，会根据位置变化闪烁，关闪烁写命令0x0C 
*********************************************************************/ 
void LCD_FlickerChar(uint8_t row,uint8_t col 

) 
{ 
lcd_wcmd(0x0D); 
LCD_Setpos(row,col); 
} 
//关闭字符闪烁 
void LCD_CloseFlicker(void) 
{ 
lcd_wcmd(0x0C); 
} 
//屏幕秒闪烁一次 
void LCD_FlickerScreen(void) 
{ 
lcd_wcmd(0x08);//关显示 
DelayMs(500); 
lcd_wcmd(0x0C);//开显示 
DelayMs(500); 
} 
/********************************************************************* 
*名 称：LCD_Init() 
*功 能：初始化LCD1602 
*入口参数：无 
*出口参数：无 
*常用命令：0x38:16*2显示，5*&点阵显示字符，8位数据;指令执行时间40us 
* 0x0C:开显示，关光标 40us 
* 0x08:关显示，关光标 40us 
* 0x0D:字符闪烁，关光标，接着设定位置，闪烁周期0.4ms左右 
* 0x0F:字符闪烁，开光标 
* 0x06:写完数据自动右移光标，普通情形，从左向右显示 40us 
* 0x04:写完数据自动左移光标，可以从右向左显示 40us 
* 0x01:清除显示内容，即清屏 1.64ms 
* 0x02:使光标还回起始位置 1.64ms 
* 0x18:屏幕上所有字符同时左移一格，适合滚动显示 40us 
* 0x1C:屏幕上所有字符同时右移一格，适合滚动显示 40us 
*********************************************************************/ 
void lcd_init(void) 
{ 
DelayMs(100); //延时20ms 
LCD_WriteInitcmd(0x38); //16*2显示，5*7点阵，8位数据 
DelayMs(1); 
LCD_WriteInitcmd(0x38); //16*2显示，5*7点阵，8位数据 
DelayMs(1); 
LCD_WriteInitcmd(0x38); //16*2显示，5*7点阵，8位数据 
DelayMs(1); 
LCD_WriteInitcmd(0x08); //先关显示，后开显示 
DelayMs(1); 
LCD_WriteInitcmd(0x06); //自动右移光标,0x04为左移光标 
DelayMs(1); 
LCD_WriteInitcmd(0x0c); //显示开，关光标;0x08为关显示 
DelayMs(1); 
LCD_WriteInitcmd(0x01); //清除LCD的显示内容 
DelayMs(1); 
} 

void LCD_GPIO_CONFIG(void) 
{ 
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOC, ENABLE); 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ; // 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //输出的最大频率为50HZ 
GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIOC端口 


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //所有GPIO为同一类型端口 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //输出的最大频率为50HZ 
GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化GPIOE端口 */ 

} 
static void LCD_Cfg_Data(u8 in) 
{ 
if (in) 
{ 
//PORTE8~PORTE15 带上拉输入 
GPIOE->CRH = 0x88888888; 
GPIOE->ODR |= 0xFF00; 
} 
else 
{ 
//PORTE8~PORTE15 输出 
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