/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHT11_H
#define __DHT11_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"

/* Exported constants --------------------------------------------------------*/
#define MAX_TICS 10000
#define DHT11_OK 0
#define DHT11_NO_CONN 1
#define DHT11_CS_ERROR 2
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_Pin_5

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
uint8_t read_DHT11(uint8_t *buf);

#endif /* __DHT11_H */
