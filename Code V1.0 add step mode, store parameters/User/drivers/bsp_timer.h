
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _bsp_timer_H
#define	_bsp_timer_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
#define CPU_IDLE()
  
/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
  volatile uint32 count;	/* ������ */
  volatile uint8 flag;    /* ��ʱ�����־  */
}SOFT_TMR;

/* Exported constants --------------------------------------------------------*/
extern uint16 BeepTime;
extern uint16 TimeCount;
extern uint16 DelaySettingTime;    //used to be delay time
extern uint8 TimeOutFlag;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* ���ⲿ���õĺ������� */

void bsp_InitTimer(void);
void Timer2_Init(uint16 arr, uint16 psc);
void Timer3_Init(uint16 arr, uint16 psc);
void TIM4_PWM_Init(uint16 arr, uint16 psc);
void Timer5_Init(uint16 arr, uint16 psc);
void bsp_StartTimer(uint8 _id, uint32 _period);
void bsp_StopTimer(uint8 _id);
uint8 bsp_CheckTimer(uint8 _id);
void DelayMs(uint32  n);
#ifdef __cplusplus
}
#endif

#endif /* _bsp_timer_H */

