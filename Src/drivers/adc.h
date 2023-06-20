/************************************************************************************
                  ** H **
Module:
Author: Unic-Lab <https://unic-lab.ru/>
************************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

//===================================================================================

#include <stdint.h>

//===================================================================================

void adc_ext_volt_init(uint32_t* p_buf, uint16_t buf_size);
void adc_ext_volt_deinit(void);
void adc_current_sense_init(uint32_t* p_buf, uint16_t buf_size);
void adc_current_senset_deinit(void);

#endif // _ADC_H_
