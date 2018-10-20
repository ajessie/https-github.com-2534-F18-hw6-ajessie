/*
 * graphics_HAL.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

void make_5digit_NumString(unsigned int num, char *string)
{
    string[0]= (        num  / 10000) +'0';
    string[1]= ((num%10000) / 1000) +'0';
    string[2]= ((num%1000) / 100) +'0';
    string[3]= ((num%100) / 10) +'0';
    string[4]= ((num%10) / 1) +'0';
    string[5]= 0;
}

void drawXY(Graphics_Context *g_sContext_p, unsigned int x, unsigned int y)
{
    char string[6];

    Graphics_drawString(g_sContext_p, "x=", -1, 10, 5, true);
    make_5digit_NumString(x, string);
    Graphics_drawString(g_sContext_p, string, -1, 30, 5, true);

    Graphics_drawString(g_sContext_p, "y=", -1, 10, 15, true);
    make_5digit_NumString(y, string);
    Graphics_drawString(g_sContext_p, string, -1, 30, 15, true);
}


void draw_Base(Graphics_Context *g_sContext_p)
{
    Graphics_Rectangle R;
    R.xMin = 0;
    R.xMax = 127;
    R.yMin = 32;
    R.yMax = 96;

    Graphics_drawRectangle(g_sContext_p, &R);
    Graphics_fillCircle(g_sContext_p, 63, 63, 10);
    Graphics_drawString(g_sContext_p, "circle move #:", -1, 10, 100, false);
    Graphics_drawString(g_sContext_p, "000", -1, 10, 110, true);
}

void make_3digit_NumString(unsigned int num, char *string)
{
    string[0]= (num/100)+'0';
    string[1]= ((num%100) / 10) + '0';
    string[2]= (num%10)+'0';
    string[3] =0;

}


void MoveCircle(Graphics_Context *g_sContext_p, bool moveToLeft, bool moveToRight)
{
    static unsigned int x = 63;
    static unsigned int moveCount = 0;
    char string[4];

    if ((moveToLeft && (x>20)) || (moveToRight && (x<110)))
    {

        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
        Graphics_fillCircle(g_sContext_p, x, 63, 10);

        if (moveToLeft)
            x = x-10;
        else
            x = x+10;

        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(g_sContext_p, x, 63, 10);

        moveCount++;
        make_3digit_NumString(moveCount, string);
        Graphics_drawString(g_sContext_p, string, -1, 10, 110, true);
    }

}
void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}


void InitGraphics(Graphics_Context *g_sContext_p) {

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);
}
