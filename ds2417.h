/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS2417_H
#define __DS2417_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "onewire.h"
#include "ds2417.h"


#define START_YEAR 1970

#define DAY_PER_YEAR 365 
#define SEC_PER_DAY 86400 
#define SEC_PER_HOUR 3600 
#define SEC_PER_MINUTE 60 

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void ConvertDateTime(uint32_t datetime, RTC_TimeTypeDef* RTC_TimeStruct, RTC_DateTypeDef* RTC_DateStruct);
void owSetDate(uint8_t *idbuf);
uint32_t owGetDate(uint8_t *idbuf);

#endif /* __DS2417_H */
