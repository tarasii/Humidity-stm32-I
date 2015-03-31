#include "tim.h"

uint16_t get_GPIO_PinSource(uint32_t GPIO_Pin_x){
	uint16_t GPIO_PinSource_x;
	if (GPIO_Pin_x == GPIO_Pin_0) {
		GPIO_PinSource_x = GPIO_PinSource0;
	} else if (GPIO_Pin_x == GPIO_Pin_1) {
		GPIO_PinSource_x = GPIO_PinSource1;
	} else if (GPIO_Pin_x == GPIO_Pin_2) {
		GPIO_PinSource_x = GPIO_PinSource2;
	} else if (GPIO_Pin_x == GPIO_Pin_3) {
		GPIO_PinSource_x = GPIO_PinSource3;
	} else if (GPIO_Pin_x == GPIO_Pin_4) {
		GPIO_PinSource_x = GPIO_PinSource4;
	} else if (GPIO_Pin_x == GPIO_Pin_5) {
		GPIO_PinSource_x = GPIO_PinSource5;
	} else if (GPIO_Pin_x == GPIO_Pin_6) {
		GPIO_PinSource_x = GPIO_PinSource6;
	} else if (GPIO_Pin_x == GPIO_Pin_7) {
		GPIO_PinSource_x = GPIO_PinSource7;
	} else if (GPIO_Pin_x == GPIO_Pin_8) {
		GPIO_PinSource_x = GPIO_PinSource8;
	} else if (GPIO_Pin_x == GPIO_Pin_9) {
		GPIO_PinSource_x = GPIO_PinSource9;
	} else if (GPIO_Pin_x == GPIO_Pin_10) {
		GPIO_PinSource_x = GPIO_PinSource10;
	} else if (GPIO_Pin_x == GPIO_Pin_11) {
		GPIO_PinSource_x = GPIO_PinSource11;
	} else if (GPIO_Pin_x == GPIO_Pin_12) {
		GPIO_PinSource_x = GPIO_PinSource12;
	} else if (GPIO_Pin_x == GPIO_Pin_13) {
		GPIO_PinSource_x = GPIO_PinSource13;
	} else if (GPIO_Pin_x == GPIO_Pin_14) {
		GPIO_PinSource_x = GPIO_PinSource14;
	} else if (GPIO_Pin_x == GPIO_Pin_15) {
		GPIO_PinSource_x = GPIO_PinSource15;
	}
	return GPIO_PinSource_x;
}

void TIM_Init(TIM_Init_TypeDef* TIM_InitStruct, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x){
	if (GPIO_Pin_x == GPIO_Pin_0 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_5 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_15 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_6 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_4 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_6 && GPIOx == GPIOC) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_6 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_1;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI1FP1;
		TIM_InitStruct->TIMx = TIM4;
	}else if (GPIO_Pin_x == GPIO_Pin_1 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_2;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI2FP2;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_3 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_2;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI2FP2;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_7 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_2;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI2FP2;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_5 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_2;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI2FP2;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_7 && GPIOx == GPIOC) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_2;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI2FP2;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_7 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_2;
		TIM_InitStruct->TIM_InputTriggerSource = TIM_TS_TI2FP2;
		TIM_InitStruct->TIMx = TIM4;
	}else if (GPIO_Pin_x == GPIO_Pin_2 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_3;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_10 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_3;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_0 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_3;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_8 && GPIOx == GPIOC) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_3;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_8 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_3;
		TIM_InitStruct->TIMx = TIM4;
	}else if (GPIO_Pin_x == GPIO_Pin_3 && GPIOx == GPIOA) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_4;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_11 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_4;
		TIM_InitStruct->TIMx = TIM2;
	}else if (GPIO_Pin_x == GPIO_Pin_1 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_4;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_9 && GPIOx == GPIOC) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_4;
		TIM_InitStruct->TIMx = TIM3;
	}else if (GPIO_Pin_x == GPIO_Pin_9 && GPIOx == GPIOB) {
		TIM_InitStruct->TIM_Channel_x = TIM_Channel_4;
		TIM_InitStruct->TIMx = TIM4;
	}
	
	TIM_InitStruct->GPIO_PinSource_x = get_GPIO_PinSource(GPIO_Pin_x);
	
	if (TIM_InitStruct->TIMx == TIM2) {
		TIM_InitStruct->GPIO_AF_TIMx = GPIO_AF_TIM2;
		TIM_InitStruct->RCC_APB1Periph = RCC_APB1Periph_TIM2;
		TIM_InitStruct->IRQn = TIM2_IRQn;
	} else if (TIM_InitStruct->TIMx == TIM3) {
		TIM_InitStruct->GPIO_AF_TIMx = GPIO_AF_TIM3;
		TIM_InitStruct->RCC_APB1Periph = RCC_APB1Periph_TIM3;
		TIM_InitStruct->IRQn = TIM3_IRQn;
	} else if (TIM_InitStruct->TIMx == TIM4) {
		TIM_InitStruct->GPIO_AF_TIMx = GPIO_AF_TIM4;
		TIM_InitStruct->RCC_APB1Periph = RCC_APB1Periph_TIM4;
		TIM_InitStruct->IRQn = TIM4_IRQn;
	}
}

void tim_init_cnt(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x){
	TIM_TimeBaseInitTypeDef TIM_InitBaseStructure;
 	TIM_ICInitTypeDef TIM_ICInitStruct;
  //NVIC_InitTypeDef NVIC_InitStructure; //create NVIC structure
 
 	TIM_Init_TypeDef TIM_InitStruct;
	
	
	TIM_Init(&TIM_InitStruct, GPIOx, GPIO_Pin_x);
	
	RCC_APB1PeriphClockCmd(TIM_InitStruct.RCC_APB1Periph, ENABLE);
			
  pin_mode(GPIOx, GPIO_Pin_x, GPIO_MODE_AF40_PP_UP);
  GPIO_PinAFConfig(GPIOx, TIM_InitStruct.GPIO_PinSource_x, TIM_InitStruct.GPIO_AF_TIMx);
		
	TIM_TimeBaseStructInit(&TIM_InitBaseStructure);
//	TIM_InitBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM_InitStruct.TIMx, &TIM_InitBaseStructure);
	
  TIM_ICStructInit(&TIM_ICInitStruct);
  TIM_ICInitStruct.TIM_Channel = TIM_InitStruct.TIM_Channel_x;
 	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling,
	TIM_ICInit(TIM_InitStruct.TIMx, &TIM_ICInitStruct);

  TIM_ITConfig(TIM_InitStruct.TIMx, TIM_IT_CC1, ENABLE);
	
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM_InitStruct.TIMx, ENABLE);
	
	NVIC_EnableIRQ(TIM_InitStruct.IRQn);	
}

void tim_init_pwm_cnt(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x){
	TIM_TimeBaseInitTypeDef TIM_InitBaseStructure;
 	TIM_ICInitTypeDef TIM_ICInitStruct;
  //NVIC_InitTypeDef NVIC_InitStructure; //create NVIC structure
  
 	TIM_Init_TypeDef TIM_InitStruct;
	
	
	TIM_Init(&TIM_InitStruct, GPIOx, GPIO_Pin_x);
	
	RCC_APB1PeriphClockCmd(TIM_InitStruct.RCC_APB1Periph, ENABLE);
			
  pin_mode(GPIOx, GPIO_Pin_x, GPIO_MODE_AF40_PP_UP);
  GPIO_PinAFConfig(GPIOx, TIM_InitStruct.GPIO_PinSource_x, TIM_InitStruct.GPIO_AF_TIMx);
	
	
	TIM_TimeBaseStructInit(&TIM_InitBaseStructure);
//	TIM_InitBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM_InitStruct.TIMx, &TIM_InitBaseStructure);
	
  TIM_ICStructInit(&TIM_ICInitStruct);
  TIM_ICInitStruct.TIM_Channel = TIM_InitStruct.TIM_Channel_x;
 	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling,
	TIM_PWMIConfig(TIM_InitStruct.TIMx, &TIM_ICInitStruct); 

	TIM_SelectInputTrigger(TIM_InitStruct.TIMx, TIM_InitStruct.TIM_InputTriggerSource);
	TIM_SelectSlaveMode(TIM_InitStruct.TIMx, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM_InitStruct.TIMx, TIM_MasterSlaveMode_Enable);

  TIM_ITConfig(TIM_InitStruct.TIMx, TIM_IT_CC2, ENABLE);
	
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM_InitStruct.TIMx, ENABLE);
	
	NVIC_EnableIRQ(TIM_InitStruct.IRQn);	
	//NVIC_EnableIRQ(TIM2_IRQn);	
}

void tim_init_pwm_out(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, uint32_t period, uint32_t dirty_cucle){

	TIM_TimeBaseInitTypeDef TIM_InitBaseStructure;
	TIM_OCInitTypeDef TIM_OCInit;
	
 	TIM_Init_TypeDef TIM_InitStruct;
	
	TIM_Init(&TIM_InitStruct, GPIOx, GPIO_Pin_x);
	
	RCC_APB1PeriphClockCmd(TIM_InitStruct.RCC_APB1Periph, ENABLE);
			
  pin_mode(GPIOx, GPIO_Pin_x, GPIO_MODE_AF40_PP_UP);
  GPIO_PinAFConfig(GPIOx, TIM_InitStruct.GPIO_PinSource_x, TIM_InitStruct.GPIO_AF_TIMx);
		
	TIM_OCInit.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInit.TIM_Pulse = dirty_cucle;
	TIM_OCInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInit.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM_InitStruct.TIMx, &TIM_OCInit);
	TIM_OC3PreloadConfig(TIM_InitStruct.TIMx, TIM_OCPreload_Enable);

	TIM_TimeBaseStructInit(&TIM_InitBaseStructure);
	TIM_InitBaseStructure.TIM_Prescaler = 0;
	TIM_InitBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitBaseStructure.TIM_Period = period;
	TIM_InitBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM_InitStruct.TIMx, &TIM_InitBaseStructure);
	
	TIM_Cmd(TIM_InitStruct.TIMx, ENABLE);
	
}


