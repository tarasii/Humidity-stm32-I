#include "dac.h"

uint16_t DAC_SIGNAL_Sinus[DAC_SIGNAL_SINUS_LENGTH] = {
	2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
	4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
	2047, 1647, 1263, 909, 599, 344, 155, 38,
	0, 38, 155, 344, 599, 909, 1263, 1647
};

uint16_t DAC_SIGNAL_Triangle[DAC_SIGNAL_TRIANGLE_LENGTH] = {
	0, 256, 512, 768, 1024, 1279, 1535, 1791,
	2047, 2303, 2559, 2815, 3071, 3326, 3582, 3838,
	4095, 3838, 3582, 3326, 3071, 2815, 2559, 2303,
	2047, 1791, 1535, 1279, 1024, 768, 512, 256
};

uint16_t DAC_SIGNAL_Sawtooth[DAC_SIGNAL_SAWTOOTH_LENGTH] = {
	0, 132, 264, 396, 528, 660, 792, 924,
	1057, 1189, 1321, 1453, 1585, 1717, 1849, 1981,
	2113, 2245, 2377, 2509, 2641, 2773, 2905, 3037,
	3170, 3302, 3434, 3566, 3698, 3830, 3962, 4095
};


//example:
//dac_init(DAC_Channel_1);
void dac_init(uint32_t DAC_Channel_X){

  DAC_InitTypeDef DAC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	

  /* Configure PA.04 (DAC_OUT1), PA.05 (DAC_OUT2) as analog */
	if (DAC_Channel_X == DAC_Channel_1) {
		pin_mode(DAC_GPIO, DAC_OUT1, GPIO_MODE_IN_AN);
	}
	else{
		pin_mode(DAC_GPIO, DAC_OUT2, GPIO_MODE_IN_AN);
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

void DAC_DMA_Config(void){
	DAC_InitTypeDef DAC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_DAC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	pin_mode(DAC_GPIO, DAC_OUT1, GPIO_MODE_IN_AN);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//TIM_TimeBaseStructure.TIM_Period = 0x300;
	TIM_TimeBaseStructure.TIM_Period = 0x150;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & DAC->DHR12R1;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &DAC_SIGNAL_Sawtooth;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = DAC_SIGNAL_SAWTOOTH_LENGTH;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel2, ENABLE);

	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = 0;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO; //DAC triger on TIM2
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);

	DAC_DMACmd(DAC_Channel_1, ENABLE);

	//dac_set(DAC_Channel_1, 600);

	TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE); //DMA update on TIM2

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //timer test on PA09
	
	TIM_Cmd(TIM2, ENABLE);	

	NVIC_EnableIRQ(TIM2_IRQn);	//IRQ for timer test
	
}

//void example(void){
//	dac_init(DAC_Channel_1, DAC_OUT1);
//	dac_set(DAC_Channel_1, 1023);
//}

