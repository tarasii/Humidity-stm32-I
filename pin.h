/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PIN_H
#define __PIN_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"

/* Exported constants --------------------------------------------------------*/
#define GPIO_MODE_IN                          0x00		//0000 0000
#define GPIO_MODE_IN_UP                    		0x40		//0100 0000
#define GPIO_MODE_IN_DOWN                   	0xc0		//1100 0000

#define GPIO_MODE_OUT2_PP                     0x05		//0000 0101
#define GPIO_MODE_OUT2_PP_UP                  0x45		//0100 0101
#define GPIO_MODE_OUT2_PP_DOWN                0xc5		//1100 0101
#define GPIO_MODE_OUT2_OD                     0x15		//0001 0101
#define GPIO_MODE_OUT2_OD_UP                  0x55		//0101 0101
#define GPIO_MODE_OUT2_OD_DOWN                0xd5		//1101 0101

#define GPIO_MODE_OUT10_PP                    0x09		//0000 1001
#define GPIO_MODE_OUT10_PP_UP                 0x49		//0100 1001
#define GPIO_MODE_OUT10_PP_DOWN               0xc9		//1100 1001
#define GPIO_MODE_OUT10_OD                    0x19		//0001 1001
#define GPIO_MODE_OUT10_OD_UP                 0x59		//0101 1001
#define GPIO_MODE_OUT10_OD_DOWN               0xd9		//1101 1001

#define GPIO_MODE_OUT40_PP                    0x0d		//0000 1101
#define GPIO_MODE_OUT40_PP_UP                 0x4d		//0100 1101
#define GPIO_MODE_OUT40_PP_DOWN               0xcd		//1100 1101
#define GPIO_MODE_OUT40_OD                    0x1d		//0001 1101
#define GPIO_MODE_OUT40_OD_UP                 0x5d		//0101 1101
#define GPIO_MODE_OUT40_OD_DOWN               0xdd		//1101 1101

#define GPIO_MODE_AF2_PP                      0x06		//0000 0110
#define GPIO_MODE_AF2_PP_UP                   0x46		//0100 0110
#define GPIO_MODE_AF2_PP_DOWN                 0xc6		//1100 0110
#define GPIO_MODE_AF2_OD                      0x16		//0001 0110
#define GPIO_MODE_AF2_OD_UP                   0x56		//0101 0110
#define GPIO_MODE_AF2_OD_DOWN                 0xd6		//1101 0110

#define GPIO_MODE_AF10_PP                     0x0a		//0000 1010
#define GPIO_MODE_AF10_PP_UP                  0x4a		//0100 1010
#define GPIO_MODE_AF10_PP_DOWN                0xca		//1100 1010
#define GPIO_MODE_AF10_OD                     0x1a		//0001 1010
#define GPIO_MODE_AF10_OD_UP                  0x5a		//0101 1010
#define GPIO_MODE_AF10_OD_DOWN                0xda		//1101 1010

#define GPIO_MODE_AF40_PP                     0x0e		//0000 1110
#define GPIO_MODE_AF40_PP_UP                  0x4e		//0100 1110
#define GPIO_MODE_AF40_PP_DOWN                0xce		//1100 1110
#define GPIO_MODE_AF40_OD                     0x1e		//0001 1110
#define GPIO_MODE_AF40_OD_UP                  0x5e		//0101 1110
#define GPIO_MODE_AF40_OD_DOWN                0xde		//1101 1110

#define GPIO_MODE_IN_AN                       0x03		//0000 0011

#define LD_PORT 		GPIOB
#define LD_GREEN 		GPIO_Pin_7
#define LD_BLUE     GPIO_Pin_6

#define BUTTON_GPIO_PORT	GPIOA
#define USER_GPIO_PIN		GPIO_Pin_0

/* Exported macro ------------------------------------------------------------*/
#define GPIO_HIGH(a,b) 		a->BSRRL = b
#define GPIO_LOW(a,b)			a->BSRRH = b
#define GPIO_TOGGLE(a,b) 	a->ODR ^= b

/* Exported functions ------------------------------------------------------- */
void pin_mode(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, uint8_t Pin_Mode);
void init_RCC_for_all_GPIO(void);
void led_init(void);
void button_init_irq (void);
void button_init (void);
#endif /* __PIN_H */
