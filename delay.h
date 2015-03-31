/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

extern __IO uint32_t TimingDelay;
extern __IO uint16_t systick_ms, toggle_ms;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint16_t uint16_time_diff(uint16_t now, uint16_t before);
void Delay(uint32_t nTime);
void TimingDelay_Decrement(void);
void SysTick_conf(void);

#endif /* __DELAY_H */

