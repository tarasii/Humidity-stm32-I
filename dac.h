/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DAC_H
#define __DAC_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"

/* Exported constants --------------------------------------------------------*/
//PA.04 (DAC_OUT1), PA.05 (DAC_OUT2)
#define DAC_OUT1 		GPIO_Pin_4
#define DAC_OUT2 		GPIO_Pin_5
#define DAC_GPIO 		GPIOA

//#define DAC_ALIGN   DAC_Align_8b_R
#define DAC_ALIGN   DAC_Align_12b_R


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void dac_init(uint32_t DAC_Channel_X);
void dac_set(uint32_t DAC_Channel_X, uint16_t value);
#endif /* __DAC_H */
