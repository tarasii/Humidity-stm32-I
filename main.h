/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "ext_glcd.h"
#include "onewire.h"

//HCH1000+555
#define capasitive_coefficient  6492.0 //0,693*(51+2*560)*8
#define zero_capacitance 315.0

//ds18B20
#define temperature_resolution 0.0625
#define tempminusconst 4096
#define tempminusthreshold 1000

//HSF1000+DS8555
#define preasure_ref 755.0
#define preasure_conv 1020

//configureADC
#define MAX_TEMP_CHNL 12
#define ADC_CONV_BUFF_SIZE 20

#define FACTORY_CALIB_BASE        ((uint32_t)0x1FF80078)    /*!< Calibration Data Bytes base address */
#define FACTORY_CALIB_DATA        ((CALIB_TypeDef *) FACTORY_CALIB_BASE)
#define USER_CALIB_BASE           ((uint32_t)0x08080000)    /*!< USER Calibration Data Bytes base address */
#define USER_CALIB_DATA           ((CALIB_TypeDef *) USER_CALIB_BASE)
#define TEST_CALIB_DIFF           (int32_t) 50  /* difference of hot-cold calib
                                               data to be considered as valid */ 

#define HOT_CAL_TEMP 110
#define COLD_CAL_TEMP  25

#define DEFAULT_HOT_VAL 0x362
#define DEFAULT_COLD_VAL 0x2A8

/* Theorically BandGAP 1.224volt */
#define VREF 		1.224L
/*
	ADC Converter 
	LSBIdeal = VREF/4096 or VDA/4096
*/
#define ADC_CONV 	4096

#define IDD_MEASURE_PORT	GPIOA
#define IDD_MEASURE             GPIO_Pin_4

#define owdevnum 2

#define START_YEAR 1970

#define DAY_PER_YEAR 365 
#define SEC_PER_DAY 86400 
#define SEC_PER_HOUR 3600 
#define SEC_PER_MINUTE 60 

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint16_t myVREF;
    uint16_t TS_CAL_COLD;
    uint16_t reserved;
    uint16_t TS_CAL_HOT;
} CALIB_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void TimingDelay_Decrement(void);
void Init_ext_GLCD_GPIOs (void);
void Init_GPIOs (void);
void RCC_Configuration(void);
void Init_tim_cnt(void);
uint16_t uint16_time_diff(uint16_t now, uint16_t before);
void configureADC_Temp(void);
void DAC_Config(void);
void configureDMA(void);
void processTempData(void);
uint16_t GetTemperature(uint8_t *idbuf);
float CalculateTemperature(uint16_t dirtytemp);
void acquireTemperatureData(void);
void setADCDMA_TransferComplete(void);
void clearADCDMA_TransferComplete(void);
void ConvertDateTime(uint32_t datetime, RTC_TimeTypeDef* RTC_TimeStruct, RTC_DateTypeDef* RTC_DateStruct);
void owSetDate(uint8_t *idbuf);
uint32_t owGetDate(uint8_t *idbuf);


#endif /* __MAIN_H */
