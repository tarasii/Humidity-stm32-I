#ifndef __EXT_GLCD_H
#define __EXT_GLCD_H

#include <stdint.h>

#define EXT_LCD_DELAY  1
#define EXT_LCD_DELAY_LONG  100

/* PIN CONFIG ------------------------------------------------------------------*/
#define DATA_PORT 		GPIOB
#define DATA_PIN0 		GPIO_Pin_8
#define DATA_PIN1 		GPIO_Pin_9
#define DATA_PIN2 		GPIO_Pin_10
#define DATA_PIN3 		GPIO_Pin_11
#define DATA_PIN4 		GPIO_Pin_12
#define DATA_PIN5 		GPIO_Pin_13
#define DATA_PIN6 		GPIO_Pin_14
#define DATA_PIN7 		GPIO_Pin_15

#define CTRL_PORT 		GPIOC
#define CTRL_E 				GPIO_Pin_0
#define CTRL_RW 			GPIO_Pin_1
#define CTRL_RS				GPIO_Pin_2	 

#define CRSLC_PORT 		GPIOB
#define CRSLC_PIN0 		GPIO_Pin_3
#define CRSLC_PIN1 		GPIO_Pin_4


void Init_Ext_GLCD(void);

void GLCD_ON(void);
void GLCD_OFF(void);
void ClearScreen(void);
void TestScreen(unsigned char value);
void TestNetScreen(void);
void PutChar(unsigned char value);
void GotoXY(unsigned char x, unsigned char y);
void WrapWrite_GLCD( unsigned char* message);
void Write_GLCD( unsigned char* message);
void TestChars(void);
void TestScroll(void);
void ClearFullScreen(void);

void GotoGXY(uint8_t gx, uint8_t y);
uint8_t GetByteXY(uint8_t gx, uint8_t y);
void PutByte(uint8_t gx, uint8_t y, uint8_t pb);
void PutPixel(uint8_t gx, uint8_t gy);
void drawLine(int x1, int y1, int x2, int y2);
void Rectangle(int x1, int y1, int x2, int y2);

#endif /* __EXT_GLCD_H */
