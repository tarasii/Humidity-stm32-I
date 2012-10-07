/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UTILS_H
#define __UTILS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include <stdio.h>
//#include <misc.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define bool _Bool
#define FALSE 0
#define TRUE !FALSE

#define BUTTON_GPIO_PORT	GPIOA
#define USER_GPIO_PIN		GPIO_Pin_0

#define LD_PORT 		GPIOB
#define LD_GREEN 		GPIO_Pin_7
#define LD_BLUE     GPIO_Pin_6

/* Exported macro ------------------------------------------------------------*/
/* MACROs for SET, RESET or TOGGLE Output port */
#define GPIO_HIGH(a,b) 		a->BSRRL = b
#define GPIO_LOW(a,b)			a->BSRRH = b
#define GPIO_TOGGLE(a,b) 	a->ODR ^= b 

/* Exported functions ------------------------------------------------------- */
void Delay(uint32_t nTime);

#endif /* __UTILS_H */
