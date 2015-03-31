/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "ext_glcd.h"
#include "onewire.h"
#include "18b20.h"
#include "dht11.h"
#include "ds2417.h"
#include "uart.h"
#include "pin.h"
#include "rtc.h"
#include "tim.h"
#include "delay.h"
#include "adc.h"
#include "dac.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//HCH1000+555
#define capasitive_coefficient  6492.0 //0,693*(51+2*560)*8
//#define zero_capacitance 315.0
#define zero_capacitance 285.0

//HSF1000+DS8555
#define preasure_ref 755.0
#define preasure_conv 1020

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
