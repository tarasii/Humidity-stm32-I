//Timer GPIO Chanel map
//+------+------+------+------+
//| TIM2                      |
//+------+------+------+------+
//| CH1  | CH2  | CH3  | CH4  |
//+------+------+------+------+
//| PA0  | PA1  | PA2  | PA3  |
//| PA5  |      |      |      |
//| PA15 | PB3  | PB10 | PB11 |
//+------+------+------+------+
//| TIM3                      |
//+------+------+------+------+
//| PA6  | PA7  | PB0  | PB1  |
//| PB4  | PB5  |      |      |
//| PC6  | PC7  | PC8  | PC9  |
//+------+------+------+------+
//| TIM4                      |
//+------+------+------+------+
//| PB6  | PB7  | PB8  | PB9  |
//+------+------+------+------+

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "pin.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	TIM_TypeDef* 	TIMx;
	uint16_t 			TIM_Channel_x;
	uint16_t 			GPIO_PinSource_x;
	uint32_t 			RCC_APB1Periph;
	uint8_t 			GPIO_AF_TIMx;
	uint16_t 			TIM_InputTriggerSource;
	IRQn_Type 		IRQn;
} TIM_Init_TypeDef;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void tim_init_cnt(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x);
void tim_init_pwm_cnt(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x);
void tim_init_pwm_out(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, uint32_t period, uint32_t dirty_cucle);

#endif /* __TIM_H */
