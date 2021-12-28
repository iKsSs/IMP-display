// IMP Projekt 2015/16
// Jakub Pastuszek
// xpastu00
// original

#include "lib.h"

/* Variable delay loop */
// t1=10, t2=20 -> 120us
// t1=10, t2=10 -> 65us
// t1=5,  t2=5  -> 25us
// t1=2,  t2=2  -> 6us
// t1=1,  t2=1  -> 4us
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
 }

//Write command to LCD
//param: cmd	command
//  	 cs		chip select
void GLCD_WriteCommand(unsigned char cmd, unsigned char cs)
{
	GLCD_WaitForStatus(0x80, cs);

	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3 << 11 );					//A0 = 0; R/W = 0

	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3C0003C0 );	//Data erase
	//command to write
	PTA->PDOR |= GPIO_PDOR_PDO( (cmd & 0x0F) << 6 );			//D3-0
	PTA->PDOR |= GPIO_PDOR_PDO( ((cmd & 0xF0) >> 4) << 26 );	//D7-4

	//CHIP Select
	if (cs == 1)
	{
		PTA->PDOR |= GPIO_PDOR_PDO( 1 << 24 );				//24(CS1) = 1
	}
	else
	{
		PTA->PDOR |= GPIO_PDOR_PDO( 1 << 25 );				//25(CS2) = 1
	}

	//enable
	PTE->PDOR |= GPIO_PDOR_PDO( 1 << 28 );					//E = 1

	WAIT;

	//disable
	PTE->PDOR &= ~GPIO_PDOR_PDO( 1 << 28 );					//E = 0

	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3 << 24 );				//CS1 = CS2 = 0
}

//Read status from LCD
//param: status		status to wait
//  	 cs			chip select
void GLCD_WaitForStatus(unsigned char status, unsigned char cs)
{
	unsigned char tmp;

	PTA->PDOR &= ~GPIO_PDOR_PDO( 1 << 10 );						//A0 = 0
	PTA->PDOR |= GPIO_PDOR_PDO( 1 << 11 );						//R/W = 1

	//port direction in for D7-4 and D3-0
	PTA->PDDR = GPIO_PDDR_PDD( 0x3000C00 );

	do
	{
		if (cs == 1)
		{
			PTA->PDOR |= GPIO_PDOR_PDO( 1 << 24 );				//24(CS1) = 1
		}
		else
		{
			PTA->PDOR |= GPIO_PDOR_PDO( 1 << 25 );				//25(CS2) = 1
		}

		//enable
		PTE->PDOR |= GPIO_PDOR_PDO( 1 << 28 );					//E = 1

		WAIT;

		tmp = (PTA->PDIR & 0x3C000000 ) >> 22;

		//disable
		PTE->PDOR &= ~GPIO_PDOR_PDO( 1 << 28 );					//E = 0

		PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3 << 24 );				//CS1 = CS2 = 0

	} while (tmp & status);

	PTA->PDOR &= ~GPIO_PDOR_PDO( 1 << 11 );						//R/W = 0
	PTA->PDDR = GPIO_PDDR_PDD( 0x3F000FC0 );
}

//Move start pixel on LCD
//param: x		position x
//  	 y  	position y
void GLCD_Goto(unsigned char x, unsigned char y) {
	if(x < (SCREEN_WIDTH/2)) {
		GLCD_WaitForStatus(0x80, 2); 					// wait for the chip
		GLCD_WriteCommand(COLUMN_ADDRESS_SET | x, 2);
		GLCD_WriteCommand(PAGE_ADDRESS_SET | y, 2);
	} else {
		GLCD_WaitForStatus(0x80, 1); 					// wait for the chip
		GLCD_WriteCommand(COLUMN_ADDRESS_SET | (x - (SCREEN_WIDTH/2)), 1);
		GLCD_WriteCommand(PAGE_ADDRESS_SET | y, 1);
	}
}

//Write data to position x/y on LCD
//param: x				position x
//  	 y  			position y
//		 dataToWrite	8 bit to write
void GLCD_WriteData(unsigned char x, unsigned char y, unsigned char dataToWrite) {

	GLCD_Goto(x, y);						// position for the write

	PTA->PDOR |= GPIO_PDOR_PDO( 1 << 10 );						//A0 = 1
	PTA->PDOR &= ~GPIO_PDOR_PDO( 1 << 11 );						//R/W = 0

	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3C0003C0 );	//Data erase
	//data to write
	PTA->PDOR |= GPIO_PDOR_PDO( (dataToWrite & 0x0F) << 6 );			//D3-0
	PTA->PDOR |= GPIO_PDOR_PDO( ((dataToWrite & 0xF0) >> 4) << 26 );	//D7-4

	if(x < (SCREEN_WIDTH/2))			// select the correct chip
	{
		PTA->PDOR |= GPIO_PDOR_PDO( 1 << 25 );				//CS2 = 1
	}
	else
	{
		PTA->PDOR |= GPIO_PDOR_PDO( 1 << 24 );				//CS1 = 1
	}

	//enable
	PTE->PDOR |= GPIO_PDOR_PDO( 1 << 28 );					//E = 1

	WAIT;

	//disable
	PTE->PDOR &= ~GPIO_PDOR_PDO( 1 << 28 );					//E = 0

	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3 << 24 );				//CS1 = CS2 = 0
}

//Read data from position x/y on LCD
//param: x			position x
//  	 y  		position y
unsigned char GLCD_ReadData(unsigned char x, unsigned char y) {

	unsigned char tmp;

	GLCD_Goto(x, y);

	PTA->PDOR |= GPIO_PDOR_PDO( 1 << 10 );						//A0 = 1
	PTA->PDOR |= GPIO_PDOR_PDO( 1 << 11 );						//R/W = 1

	//port direction in for D7-4 and D3-0
	PTA->PDDR = GPIO_PDDR_PDD( 0x3000C00 );

	if(x < (SCREEN_WIDTH/2))			// select the correct chip
	{
		PTA->PDOR |= GPIO_PDOR_PDO( 1 << 25 );				//CS2 = 1
	}
	else
	{
		PTA->PDOR |= GPIO_PDOR_PDO( 1 << 24 );				//CS1 = 1
	}

	//enable
	PTE->PDOR |= GPIO_PDOR_PDO( 1 << 28 );					//E = 1

	WAIT;

	//disable
	PTE->PDOR &= ~GPIO_PDOR_PDO( 1 << 28 );					//E = 0

	//enable
	PTE->PDOR |= GPIO_PDOR_PDO( 1 << 28 );					//E = 1

	WAIT;

	tmp = (PTA->PDIR & 0x3C0003C0 ) >> 22 | (PTA->PDIR & 0x3C0003C0 ) >> 6;

	//disable
	PTE->PDOR &= ~GPIO_PDOR_PDO( 1 << 28 );					//E = 0

	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3 << 24 );				//CS1 = CS2 = 0
	PTA->PDOR &= ~GPIO_PDOR_PDO( 1 << 11 );					//R/W = 0
	PTA->PDDR = GPIO_PDDR_PDD( 0x3F000FC0 );

	return tmp;
}

//Set pixel on LCD
//param: x			position x
//  	 y  		position y
//		 color 		0xFF/0x00
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color) {
	unsigned char temp;

	if(x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;

	temp = GLCD_ReadData(x, y/8);

	if(color)
		GLCD_WriteData(x, y/8, temp | (1 << (y % 8)));
	else
		GLCD_WriteData(x, y/8, temp & ~(1 << (y % 8)));
}

//Fill LCD screen (ON/OFF)
//param: color	0xFF/0x00
void GLCD_FillScreen(unsigned char color) {
	unsigned char j, i;
	for(j = 0; j < SCREEN_HEIGHT/8; j++)
	  for(i = 0; i < SCREEN_WIDTH; i++)
		  GLCD_WriteData(i, j, color);
}

//Try draw on LCD
void GLCD_Try() {
	unsigned char j, i;
	for(j = 0; j < SCREEN_HEIGHT/8; j++)
	  for(i = 0; i < SCREEN_WIDTH/2; i++)
	  {
		GLCD_WriteData(i, j, WHITE);
	  }
}

//Draw Chessboard on LCD
//param: shift	shift on x axis
//exmpl: shift = 0 -> chessboard from top left
//		 shift = 6 -> chessboard from bottom right
void GLCD_Draw_Chessboard(unsigned char shift) {
	unsigned char j, i;
	unsigned char maxH = SCREEN_HEIGHT/8;
	unsigned char maxW = SCREEN_WIDTH+shift;
	for(j = 0; j < maxH; j++)
	{
	  for(i = shift; i < maxW; i++)
	  {
		if ( j%2 == 0 )
		{
		  if ( (i/8)%2 == 0 )
		  {
			  GLCD_WriteData(i-shift, j, BLACK);
		  }
		  else
		  {
			  GLCD_WriteData(i-shift, j, WHITE);
		  }
		}
		else
		{
		  if ( (i/8)%2 == 0 )
		  {
			  GLCD_WriteData(i-shift, j, WHITE);
		  }
		  else
		  {
			  GLCD_WriteData(i-shift, j, BLACK);
		  }
		}

	  }
	}
}

//Initialize LCD
void GLCD_Init()
{
	unsigned char chip;
	for (chip = 1; chip <3; chip++)
	{
		GLCD_WriteCommand(RESET, chip);
		GLCD_WaitForStatus(0x10, chip);
//		GLCD_WriteCommand(DISPLAY_OFF, chip);
//		GLCD_WriteCommand(DISPLAY_START_LINE, chip);
//		GLCD_WriteCommand(STATIC_DRIVE_OFF, chip);
//		GLCD_WriteCommand(COLUMN_ADDRESS_SET | 0, chip);
//		GLCD_WriteCommand(PAGE_ADDRESS_SET | 0, chip);
//		GLCD_WriteCommand(DUTY_RATIO_32, chip);
//		GLCD_WriteCommand(ADC_CLOCKWISE, chip);
//		GLCD_WriteCommand(READ_MODIFY_WRITE, chip);
		GLCD_WriteCommand(DISPLAY_ON, chip);
	}
}

//Initialize Kit ports
void GLCD_InitPorts()
{
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTA_MASK;

	/* Set corresponding PTA pins for GPIO functionality */
	PORTA->PCR[6]  = ( 0|PORT_PCR_MUX(0x01) ); // D0 - 25
	PORTA->PCR[7]  = ( 0|PORT_PCR_MUX(0x01) ); // D1 - 27
	PORTA->PCR[8]  = ( 0|PORT_PCR_MUX(0x01) ); // D2 - 23
	PORTA->PCR[9]  = ( 0|PORT_PCR_MUX(0x01) ); // D3 - 28
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A0 - 24
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // RW - 26
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) ); // CS1 - 39
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) ); // CS2 - 40
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) ); // D4 - 37
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) ); // D5 - 35
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // D6 - 38
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) ); // D7 - 36

	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR  =  GPIO_PDDR_PDD( 0x3F000FC0 );
	PTA->PDOR |=  GPIO_PDOR_PDO( 0x03000400 );	//A0 = CS1 = CS2 = 1
	PTA->PDOR &= ~GPIO_PDOR_PDO( 0x3C000BC0 );	//R/W = D = 0

	/* Set corresponding PTE pin for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // E - 34

	/* Change corresponding PTE port pin as output */
	PTE->PDDR  =  GPIO_PDDR_PDD( 0x10000000 );
	PTE->PDOR &= ~GPIO_PDOR_PDO( 0x10000000 );	//E = 0
}
