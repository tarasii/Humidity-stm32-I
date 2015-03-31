 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHT11_H
#define __DHT11_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "pin.h"
#include "tim.h"

/* Exported constants --------------------------------------------------------*/
#define MAX_TICS 10000
#define DHT11_OK 0
#define DHT11_NO_CONN 1
#define DHT11_CS_ERROR 2
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_Pin_5

//#define DHT11_PORT GPIOC
//#define DHT11_PIN GPIO_Pin_2

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
uint8_t DHT11_RawRead(uint8_t *buf);
float DHT22_Humidity(uint8_t *buf);
float DHT22_Temperature(uint8_t *buf);
uint8_t DHT11_Humidity(uint8_t *buf);
uint8_t DHT11_Temperature(uint8_t *buf);
uint8_t DHT11_pwm_Read(uint8_t *buf, uint32_t *dt, uint32_t *cnt);

#endif /* __DHT11_H */
