//ADC GPIO Chanel map
//*--------*------*
//| Chanel | GPIO |
//*--------*------*
//|   01   | PA01 |
//|   02   | PA02 |
//|   03   | PA03 |
//|   04   | PA04 |
//|   05   | PA05 |
//|   06   | PA06 |
//|   07   | PA07 |
//|   08   | PB08 |
//|   09   | PB09 |
//|   10   | PC00 |
//|   11   | PC01 |
//|   12   | PC02 |
//|   13   | PC03 |
//|   14   | PC04 |
//|   15   | PC05 |
//|   16   | TEMP |
//|   17   | VREF |
//*--------*------*

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "utils.h"
#include "pin.h"

/* Exported constants --------------------------------------------------------*/

#define ADC_CONV_BUFF_SIZE 24

#define FACTORY_CALIB_BASE        ((uint32_t)0x1FF80078)    /*!< Calibration Data Bytes base address */
#define FACTORY_CALIB_DATA        ((CALIB_TypeDef *) FACTORY_CALIB_BASE)
#define USER_CALIB_BASE           ((uint32_t)0x08080000)    /*!< USER Calibration Data Bytes base address */
#define USER_CALIB_DATA           ((CALIB_TypeDef *) USER_CALIB_BASE)
#define TEST_CALIB_DIFF           (int32_t) 50  /* difference of hot-cold calib
                                               data to be considered as valid */ 
#define ADC_GPIO_Pin_Ch01 GPIO_Pin_1
#define ADC_GPIO_Pin_Ch02 GPIO_Pin_2
#define ADC_GPIO_Pin_Ch03 GPIO_Pin_3
#define ADC_GPIO_Pin_Ch04 GPIO_Pin_4
#define ADC_GPIO_Pin_Ch05 GPIO_Pin_5
#define ADC_GPIO_Pin_Ch06 GPIO_Pin_6
#define ADC_GPIO_Pin_Ch07 GPIO_Pin_7
#define ADC_GPIO_Pin_Ch08 GPIO_Pin_0
#define ADC_GPIO_Pin_Ch09 GPIO_Pin_1
#define ADC_GPIO_Pin_Ch10 GPIO_Pin_0
#define ADC_GPIO_Pin_Ch11 GPIO_Pin_1
#define ADC_GPIO_Pin_Ch12 GPIO_Pin_2
#define ADC_GPIO_Pin_Ch13 GPIO_Pin_3
#define ADC_GPIO_Pin_Ch14 GPIO_Pin_4
#define ADC_GPIO_Pin_Ch15 GPIO_Pin_5

#define ADC_GPIO_Ch01 GPIOA
#define ADC_GPIO_Ch02 GPIOA
#define ADC_GPIO_Ch03 GPIOA
#define ADC_GPIO_Ch04 GPIOA
#define ADC_GPIO_Ch05 GPIOA
#define ADC_GPIO_Ch06 GPIOA
#define ADC_GPIO_Ch07 GPIOA
#define ADC_GPIO_Ch08 GPIOB
#define ADC_GPIO_Ch09 GPIOB
#define ADC_GPIO_Ch10 GPIOC
#define ADC_GPIO_Ch11 GPIOC
#define ADC_GPIO_Ch12 GPIOC
#define ADC_GPIO_Ch13 GPIOC
#define ADC_GPIO_Ch14 GPIOC
#define ADC_GPIO_Ch15 GPIOC

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

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint16_t myVREF;
    uint16_t TS_CAL_COLD;
    uint16_t reserved;
    uint16_t TS_CAL_HOT;
} CALIB_TypeDef;

typedef struct {
	uint32_t refAVG; 
	uint32_t tempAVG;
	int32_t temperature_C;
	float voltage_V;	
	uint32_t Chanel13AVG; 
	uint32_t Chanel04AVG; 
} ADC_Typedef;

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern uint16_t ADC_ConvertedValueBuff[ADC_CONV_BUFF_SIZE];
extern CALIB_TypeDef calibdata;
extern uint8_t ADC_RegCfg[];
extern ADC_Typedef ADC_RES;
extern bool flag_ADCDMA_TransferComplete;

void ADC_AcquireData(void);
void insrtionSort(uint16_t *numbers, uint32_t array_size); 
uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples);
void ADC_ProcessData(void);
void ADC_init(void);
void ADC_DMA_init(void);
void SetCalibData(void);
void ADC_Default_RegCongig(void);
void setADCDMA_TransferComplete(void);
void clearADCDMA_TransferComplete(void);
	
uint16_t adc_coretemp_simple(void);

#endif /* __ARC_H */
