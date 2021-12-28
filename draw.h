// IMP Projekt 2015/16
// Jakub Pastuszek
// xpastu00

#ifndef __DRAW
#define __DRAW

#include "lib.h"

void GLCD_Line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color);
void GLCD_Rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char fill, unsigned char color);
void GLCD_Circle(unsigned char x, unsigned char y, unsigned char radius, unsigned char fill, unsigned char color);

#endif
