/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _CURRENT_SENSE_H_
#define _CURRENT_SENSE_H_

//===================================================================================

void current_sense_start_measure(void);
void current_sense_stop_measure(void);

void current_sense_process_adc_data(void);

float current_sense_get_up_current(void);
float current_sense_get_down_current(void);
float current_sense_get_side_current(void);

//===================================================================================

#endif // _CURRENT_SENSE_H_
