/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// IMP Projekt 2015/16
// Jakub Pastuszek
// xpastu00
// original

#include "MK60D10.h"
#include "lib.h"
#include "font.h"
#include "draw.h"

/* Auxiliary global variable */
unsigned long int counter = 0;

static int i = 0;

void test1() {
	GLCD_FillScreen(BLACK);

	delay(1000,500);

	GLCD_Draw_Chessboard(0);

	delay(2000,500);

	GLCD_FillScreen(WHITE);

	GLCD_text57("IMP PROJEKT 2015/16\nJAKUB PASTUSZEK\nXPASTU00", 0, 0);

	GLCD_Rect(100,10,120,30,NO_FILL,BLACK);
	GLCD_Circle(110,20,8,FILL,BLACK);

	GLCD_Circle(75,22,6,NO_FILL,BLACK);
	GLCD_Line(69,16,81,28,BLACK);
	GLCD_Line(69,28,81,16,BLACK);

	GLCD_text36("HOTOVO", 0, 3);

	for (;;) {
		GLCD_text36("!", 6, 3);
		delay(2000,500);
		GLCD_text36(" ", 6, 3);
		delay(2000,500);
    }
}

int main(void)
{
	GLCD_InitPorts();
	GLCD_Init();

	//GLCD_FillScreen(WHITE);

	//GLCD_Try();

	test1(); //--- never ends ---

	for (;;) {
		counter++;
    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
