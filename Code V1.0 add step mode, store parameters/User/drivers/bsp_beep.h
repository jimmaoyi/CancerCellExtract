
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _bsp_beep_H
#define	_bsp_beep_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/

#define beep    0x01
#define motor   0x02
  
#define EMe(BitVal)  		GPIO_WriteBit(GPIOC,GPIO_Pin_7,BitVal)
#define MV1(BitVal)  		GPIO_WriteBit(GPIOB,GPIO_Pin_10,BitVal)
#define MV2(BitVal)  		GPIO_WriteBit(GPIOB,GPIO_Pin_1,BitVal)
#define MV4(BitVal)  		GPIO_WriteBit(GPIOC,GPIO_Pin_5,BitVal)
#define MV3(BitVal)  		GPIO_WriteBit(GPIOB,GPIO_Pin_0,BitVal)
#define EMd(BitVal)  		GPIO_WriteBit(GPIOC,GPIO_Pin_8,BitVal)
#define EMa(BitVal)  		GPIO_WriteBit(GPIOC,GPIO_Pin_3,BitVal)
#define EMb(BitVal)  		GPIO_WriteBit(GPIOA,GPIO_Pin_12,BitVal)
#define EMc(BitVal)  		GPIO_WriteBit(GPIOC,GPIO_Pin_9,BitVal)
#define EMf(BitVal)  		GPIO_WriteBit(GPIOC,GPIO_Pin_6,BitVal)    
  
  
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern uint16_t beep_arr;			//蜂鸣器pwm的重载值
extern uint16_t beep_psc;		//蜂鸣器pwm的预分频值
extern uint16_t beep_compare;//蜂鸣器pwm的比较值（决定占空比）
/* Exported functions ------------------------------------------------------- */
  
extern void bsp_InitMotor(void);

extern void BEEP_Init(void);
extern void BEEP_Off(void);
extern void BEEP_On(void);
extern void BEEP_SetFreq(uint16_t freq);
extern void BEEP_SetDuty(uint8_t duty);
extern uint8_t BEEP_GetDuty(void);
extern uint16_t BEEP_GetFreq(void);


#ifdef __cplusplus
}
#endif

#endif /* _bsp_beep_H */
