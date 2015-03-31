#include "uart.h"

void USART_open(USART_TypeDef* USARTx, uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructure; 

  assert_param(IS_USART_123_PERIPH(USARTx));

  if (USARTx == USART1) {

    // Turn on clocks
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  } //else {
	
  if (USARTx == USART2) {

    // Turn on clocks
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

  } 
	
  if (USARTx == USART3) {

    // Turn on clocks
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

  }
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//return -1;

}

void USART_close(USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));
	
  if (USARTx == USART1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
	}
  if (USARTx == USART2) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
	}
  USART_Cmd(USARTx, DISABLE);
	
    //return 0;
}

void USART_DMA_send(USART_TypeDef* USARTx, char *buf, uint8_t len){
	//
	DMA_InitTypeDef DMA_InitStructure;
	
	// DMA на запись
	DMA_DeInit(DMAy_Channelx); //DMA1_Channel4
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USARTx->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	// старт цикла отправки
	USART_ClearFlag(USARTx, USART_FLAG_TC | USART_FLAG_TXE);
	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);	
	DMA_Cmd(DMAy_Channelx, ENABLE);
	
	// wait until transmission complite from dma 
	if (DMAy_Channelx == DMA1_Channel1) { 	
		while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET) {
		}
	}
	if (DMAy_Channelx == DMA1_Channel2) { 	
		while (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET) {
		}
	}
	if (DMAy_Channelx == DMA1_Channel3) { 	
		while (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET) {
		}
	}
	if (DMAy_Channelx == DMA1_Channel4) { 	
		while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET) {
		}
	}
	
	DMA_Cmd(DMAy_Channelx, DISABLE);
	USART_DMACmd(USARTx, USART_DMAReq_Tx, DISABLE);
}

int uart_putc(USART_TypeDef* USARTx, uint32_t c)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
  USARTx->DR =  (c & 0xff);
  return 0;
}

void USART_print(USART_TypeDef* USARTx, char *buf, uint8_t len){
	uint8_t i;
	for (i=0; i<len; i++){
		if( !buf[i] ) break;
		USART_SendData(USARTx, buf[i]);
		Delay(5);
	}
	
}

int uart_getc (USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
  return  USARTx->DR & 0xff;
}



