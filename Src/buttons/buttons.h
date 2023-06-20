/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

//===================================================================================

typedef enum {
	UP_BTN,
	DOWN_BTN,
	STOP_BTN
} ButtonType_t;

typedef enum {
	NOT_PRESSED_BTN,
	PRESSED_BTN
} ButtonState_t;

//===================================================================================

void buttons_init(void);
ButtonState_t buttons_getState(ButtonType_t buttonType);

//===================================================================================

#endif // _BUTTONS_H_
