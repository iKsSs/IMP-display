// IMP Projekt 2015/16
// Jakub Pastuszek
// xpastu00

#ifndef __LIB
#define __LIB

#include "MK60D10.h"

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

#define DISPLAY_ON 0xAF
#define DISPLAY_OFF 0xAE
#define DISPLAY_START_LINE 0xC0
#define PAGE_ADDRESS_SET 0xB8
#define COLUMN_ADDRESS_SET 0x00
#define ADC_CLOCKWISE 0xA0
#define ADC_COUNTERCLOCKWISE 0xA1
#define STATIC_DRIVE_ON 0xA5
#define STATIC_DRIVE_OFF 0xA4
#define DUTY_RATIO_16 0xA8
#define DUTY_RATIO_32 0xA9
#define READ_MODIFY_WRITE 0xE0
#define END_READ_MODIFY 0xEE
#define RESET 0xE2

#define SCREEN_WIDTH	122
#define SCREEN_HEIGHT 	32

#define BLACK	0xFF
#define WHITE	0x00

#define FILL	1
#define NO_FILL	0

#define WAIT	delay(1,1)

void delay(int t1, int t2);
void GLCD_WriteCommand(unsigned char, unsigned char);
void GLCD_WaitForStatus(unsigned char,unsigned char);
void GLCD_WriteData(unsigned char, unsigned char, unsigned char);
unsigned char GLCD_ReadData(unsigned char, unsigned char);
void GLCD_SetPixel(unsigned char, unsigned char, unsigned char);
void GLCD_FillScreen(unsigned char);
void GLCD_Try();
void GLCD_Draw_Chessboard(unsigned char);
void GLCD_Init();
void GLCD_InitPorts();

#endif
