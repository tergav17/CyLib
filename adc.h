/*
 * adc.h
 *
 *  Created on: Oct 19, 2021
 *      Author: strinco
 */

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "lcd.h"
#include "Timer.h"

#define OPR_A 4.6873e-12
#define OPR_B -4.0831e-8
#define OPR_C 1.3369e-4
#define OPR_D -0.202
#define OPR_E 133.67

void adc_init(void);
int adc_read(void);

void adc_read_begin(void);
int adc_read_finish(void);

#endif /* ADC_H_ */
