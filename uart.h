#ifndef _UART_H
#define _UART_H

#include "utils.h"

#define DMAy_Channelx DMA1_Channel4

//+------+------+------+------+------+
//| UART1                            |
//+------+------+------+------+------+
//| CTS  | RTS  | TX   | RX   | CK   |
//+------+------+------+------+------+
//| PA11 | PA12 | PA9  | PA10 | PA8  |
//|      |      | PB6  | PB7  |      |
//+------+------+------+------+------+
//| UART2                            |
//+------+------+------+------+------+
//| PA0  | PA1  | PA2  | PA3  | PA4  |
//+------+------+------+------+------+
//| UART3                            |
//+------+------+------+------+------+
//| PB13 | PB14 | PB10 | PB11 | PB12 |
//|      |      | PC10 | PC11 | PC12 |
//+------+------+------+------+------+

void USART_open(USART_TypeDef* USARTx, uint32_t baud);
void USART_DMA_send(USART_TypeDef* USARTx, char *buf, uint8_t len);
void USART_close(USART_TypeDef* USARTx);
void USART_print(USART_TypeDef* USARTx, char *buf, uint8_t len);
int uart_putc(USART_TypeDef* USARTx, uint32_t c);
int uart_getc(USART_TypeDef* USARTx);

#endif
