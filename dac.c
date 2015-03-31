#include "dac.h"

//example:
//dac_init(DAC_Channel_1, DAC_OUT1);
void dac_init(uint32_t DAC_Channel_X){

  GPIO_InitTypeDef GPIO_InitStructure; 
  DAC_InitTypeDef DAC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);     
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	

  /* Configure PA.04 (DAC_OUT1), PA.05 (DAC_OUT2) as analog */
	if (DAC_Channel_X == DAC_Channel_1) {
		GPIO_InitStructure.GPIO_Pin =  DAC_OUT1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(DAC_GPIO, &GPIO_InitStructure);
	}
	else{
		GPIO_InitStructure.GPIO_Pin =  DAC_OUT2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(DAC_GPIO, &GPIO_InitStructure);
	}

  /* DAC channel Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	/* DAC Channel Init */
	DAC_Init(DAC_Channel_X, &DAC_InitStructure);
	/* Enable DAC Channel */
	DAC_Cmd(DAC_Channel_X, ENABLE);
		
}

//example:
//dac_set(DAC_CHANEL1, 1023);
void dac_set(uint32_t DAC_Channel_X, uint16_t value){
	
	if (DAC_ALIGN == DAC_Align_12b_R) {
		value &= 0x3ff;
	} else {
		value &= 0xff;
	}
	
	if (DAC_Channel_X == DAC_Channel_1) {
		DAC_SetChannel1Data(DAC_ALIGN, value);
	} else {
		DAC_SetChannel2Data(DAC_ALIGN, value);
	}
	
}

//void example(void){
//	dac_init(DAC_Channel_1, DAC_OUT1);
//	dac_set(DAC_Channel_1, 1023);
//}

