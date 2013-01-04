/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __18B20_H
#define __18B20_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "onewire.h"


//ds18B20
#define temperature_resolution 0.0625
#define tempminusconst 4096
#define tempminusthreshold 1000


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint16_t GetTemperature(uint8_t *idbuf);
float CalculateTemperature(uint16_t dirtytemp);
uint16_t GetSingleTemperature(void);

#endif /* __18B20_H */
