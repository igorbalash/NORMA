/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _LEDS_H_
#define _LEDS_H_

//===================================================================================

typedef enum {
	RED = 0,
	GREEN
} LedColor_t;

//===================================================================================

void leds_init(void);
void leds_ledOn(LedColor_t color);
void leds_ledOff(LedColor_t color);
void leds_ledToggle(LedColor_t color);

#endif // _LEDS_H_
