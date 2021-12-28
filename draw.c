// IMP Projekt 2015/16
// Jakub Pastuszek
// xpastu00
// 10%

// Inspired by glcd_library_1_0

#include "draw.h"

//Draw line on LCD
//param: x1			start position x
//  	 y1  		start position y
//		 x2			end position x
//  	 y2  		end position y
//		 color 		0xFF/0x00
void GLCD_Line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color)
{
   signed int  x, y, addx, addy, dx, dy;
   signed long P;
   int i;

   if ((signed int)(x2 - x1) < 0)
	   dx = (signed int)(x1 - x2);
   else
	   dx = (signed int)(x2 - x1);

   if ((signed int)(y2 - y1) < 0)
	   dy = (signed int)(y1 - y2);
   else
	   dy = (signed int)(y2 - y1);
   x = x1;
   y = y1;

   if(x1 > x2)
      addx = -1;
   else
      addx = 1;
   if(y1 > y2)
      addy = -1;
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
    	  GLCD_SetPixel(x, y, color);

         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         GLCD_SetPixel(x, y, color);

         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
      }
   }
}

//Draw rectangle on LCD
//param: x1			start position x
//  	 y1  		start position y
//		 x2			end position x
//  	 y2  		end position y
//		 fill		0xFF/0x00
//		 color 		0xFF/0x00
void GLCD_Rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char fill, unsigned char color)
{
   if(fill)
   {
      int y, ymax;                          // Find the y min and max
      if(y1 < y2)
      {
         y = y1;
         ymax = y2;
      }
      else
      {
         y = y2;
         ymax = y1;
      }

      for(; y<=ymax; ++y)                    // Draw lines to fill the rectangle
         GLCD_Line(x1, y, x2, y, color);
   }
   else
   {
      GLCD_Line(x1, y1, x2, y1, color);      // Draw the 4 sides
      GLCD_Line(x1, y2, x2, y2, color);
      GLCD_Line(x1, y1, x1, y2, color);
      GLCD_Line(x2, y1, x2, y2, color);
   }
}

//Draw circle on LCD
//param: x			circumcentre position x
//  	 y  		circumcentre position y
//		 radius		circle radius
//		 fill		0xFF/0x00
//		 color 		0xFF/0x00
void GLCD_Circle(unsigned char x, unsigned char y, unsigned char radius, unsigned char fill, unsigned char color)
{
   signed char a, b;
   signed int P;
   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         GLCD_Line(x-a, y+b, x+a, y+b, color);
         GLCD_Line(x-a, y-b, x+a, y-b, color);
         GLCD_Line(x-b, y+a, x+b, y+a, color);
         GLCD_Line(x-b, y-a, x+b, y-a, color);
      }
      else
      {
         GLCD_SetPixel(a+x, b+y, color);
         GLCD_SetPixel(b+x, a+y, color);
         GLCD_SetPixel(x-a, b+y, color);
         GLCD_SetPixel(x-b, a+y, color);
         GLCD_SetPixel(b+x, y-a, color);
         GLCD_SetPixel(a+x, y-b, color);
         GLCD_SetPixel(x-a, y-b, color);
         GLCD_SetPixel(x-b, y-a, color);
      }

      if(P < 0)
         P+= 3 + 2*a++;
      else
         P+= 5 + 2*(a++ - b--);
    } while(a <= b);
}
