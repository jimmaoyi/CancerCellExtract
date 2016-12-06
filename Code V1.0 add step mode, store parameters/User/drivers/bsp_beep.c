/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/2/16 星期二,Administrator
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
#include "bsp_beep.h"
#include "bsp_timer.h"
/* Private typedef -----------------------------------------------------------*/

//#define Beep(BitVal)  		    GPIO_WriteBit(GPIOB,GPIO_Pin_6,BitVal)



/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
//定时器时钟源Fck_cnt = 72000000/(1+psc)=1000000Hz
//定时器频率(freq) = 72000000/((1+psc)*(arr+1)) = 1000000/2000=500Hz
//占空比(duty) = compare/(arr+1) = 1000/2000 = 50%
uint16_t beep_arr = 1999;//蜂鸣器pwm的重载值
uint16_t beep_psc = 71;//蜂鸣器pwm的预分频值，
uint16_t beep_compare = 1000;//蜂鸣器pwm的比较值（决定占空比）

/* Private function prototypes -----------------------------------------------*/  
/* Private functions ---------------------------------------------------------*/
/**
  * @fun    void bsp_InitBeep
  * @brief  初始化蜂鸣器
  * @author huangzibo
  * @param  None
  *
  * @retval 
  */
void bsp_InitMotor(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //开启端口时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
}



/**
  * @brief 初始化蜂鸣器
  * @param none
  * @retval none
  * @note
  *    将TIM4_CH3作为蜂鸣器PWM控制
  *    频率范围是: 1MHz~15Hz，初始化后频率是500Hz
	*		 初始化后，占空比是: 0%，因此蜂鸣器不发声
  */
void BEEP_Init(void)
{
	TIM4_PWM_Init(beep_arr,beep_psc);
	BEEP_Off();
}

/**
  * @brief 关闭蜂鸣器
  * @param none
  * @retval none
  * @note
  *    将PWM占空比设置为0%
  */
void BEEP_Off(void)
{
  //BEEP_SetDuty(0);
}

/**
  * @brief 打开蜂鸣器
  * @param none
  * @retval none
  * @note
  *   将PWM频率设置为500Hz
	*		将占空比设置为50%
  */
void BEEP_On(void)
{
  TIM_Cmd(TIM3, ENABLE);
  BeepTime = 0;
  //BEEP_SetFreq(5000);
  BEEP_SetDuty(30);
}

/**
  * @brief 设置蜂鸣器PWM频率
  * @param freq : 设置的频率（单位Hz）范围(15 ~ 65535)
  * @retval none
  * @note
  *    定时器时钟源频率 Fck_cnt = PCLK/(psc+1) = 1MHz
  *    频率 Freq = PCLK/((arr+1)*(psc+1))
  */
void BEEP_SetFreq(uint16_t freq)
{
	if(freq<15)return;//能设置的最低频率必须大于15
	
	beep_arr = 24000000/((1+beep_psc)*freq)-1;//根据频率计算重载值ARR
	beep_compare = (beep_arr+1)/2;	//将占空比设置为50%

	TIM4->ARR = beep_arr;//设置重载值	
	TIM4->CCR3 = beep_compare;//设置比较值
//    TIM4->CCR1 = beep_compare;//设置比较值
}

/**
  * @brief 获取当前蜂鸣器PWM频率
  * @param none
  * @retval 当前PWM的频率
  * @note
  *    定时器计数频率 = PCLK / ( psc + 1 )
  *    定时器中断周期 = ( arr + 1 )*( psc + 1) / PCLK
  */
uint16_t BEEP_GetFreq(void)
{
	uint16_t freq;
	freq = 24000000/((beep_arr+1)*(beep_psc+1));
	return freq;
}

/**
  * @brief 设置蜂鸣器PWM占空比
  * @param duty : 占空比，如50%，则参数为50
  * @retval none
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
void BEEP_SetDuty(uint8_t duty)
{
	beep_compare = duty*(beep_arr+1)/100;//计算比较值
	TIM4->CCR3 = beep_compare;//更新比较值
}

/**
  * @brief 获取当前蜂鸣器PWM占空比
  * @param none
  * @retval 当前PWM的占空比
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
uint8_t BEEP_GetDuty(void)
{
	uint16_t duty;
	duty = 100*beep_compare/(beep_arr+1);
	return duty;
}



/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 

