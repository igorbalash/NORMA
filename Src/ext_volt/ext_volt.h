/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _EXT_VOLT_H_
#define _EXT_VOLT_H_

//===================================================================================

void ext_volt_start_measure(void);
void ext_volt_stop_measure(void);

void ext_volt_process_adc_data(void);

float ext_volt_get_main_volt(void);
float ext_volt_get_reserve_volt(void);

//===================================================================================

#endif // _EXT_VOLT_H_
