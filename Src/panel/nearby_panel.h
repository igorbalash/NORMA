/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _NEARBY_PANEL_H_
#define _NEARBY_PANEL_H_

//===================================================================================

typedef enum {
	PANNEL_NO_CONNECT = 0,
	PANNEL_CONNECT
} NearbyPanelState_t;

//===================================================================================

void nearby_panel_init(void);
NearbyPanelState_t nearby_panel_get_state(void);

//===================================================================================

#endif // _NEARBY_PANEL_H_
