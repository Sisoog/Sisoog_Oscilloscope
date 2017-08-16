/*
 * Display.h
 *
 *  Created on: Aug 12, 2017
 *      Author: Mazarei
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <stdint-gcc.h>

typedef enum
{
	Unkwon=0,
	ili932x,
	ssd1289,
}LCDType_t;


void Tft_Init(void);
void Tft_drawPixel(int16_t x, int16_t y, uint16_t color);
LCDType_t Tft_GetType();


#endif /* DISPLAY_H_ */
