/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern RTC_DateTypeDef RTCDateStr;
extern RTC_TimeTypeDef RTCTimeStr;

void rtc_init(void);

#endif /* __LEDS_H */
