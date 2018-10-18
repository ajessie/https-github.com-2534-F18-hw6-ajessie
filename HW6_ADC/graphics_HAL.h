/*
 * graphics_HAL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef GRAPHICS_HAL_H_
#define GRAPHICS_HAL_H_

#include <ti/grlib/grlib.h>

void draw_Base(Graphics_Context *g_sContext_p);
void MoveCircle(Graphics_Context *g_sContext_p, bool moveToLeft, bool moveToRight);
void InitGraphics(Graphics_Context *g_sContext_p);

#endif /* GRAPHICS_HAL_H_ */
