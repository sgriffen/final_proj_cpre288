/*
 * adc.h
 *
 *  Created on: Mar 5, 2019
 *      Author: tvpham
 */

#ifndef ADC_H_
#define ADC_H_

void adc_init();
int adc_read();
int adc_avgValue();
double adc_convertToCm();

#endif /* ADC_H_ */
