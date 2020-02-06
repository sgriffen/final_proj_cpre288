/*
 * adc.c
 *
 *  Created on: Mar 5, 2019
 *      Author: tvpham
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include <math.h>

void adc_init()
{
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010; //enable port b clock
    SYSCTL_RCGCADC_R |= 0x1; // Enable ADC0 clock

    GPIO_PORTB_AFSEL_R |= 0b00010000;
    GPIO_PORTB_DIR_R &= 0b11101111;
    GPIO_PORTB_DEN_R &= 0b11101111;
    GPIO_PORTB_AMSEL_R |= 0b00010000; //Sets pin 4 of port B to analog
    GPIO_PORTB_ADCCTL_R = 0x0;


    GPIO_PORTB_ADCCTL_R = 0x0; //Use SysClk as ADC clk. Default, OK if omitted
    ADC0_ACTSS_R &= 0xFFFE; //Disable ADC0 SS0 by clearing bit 0
    ADC0_EMUX_R &= 0x0; //Set SS0 to trigger using ADCPSSI reg.
    ADC0_SSMUX0_R |= 0x0000000A; // Sample channel 1, then 9

    ADC0_SSCTL0_R = 0x00000000; // First clear register to 0
    ADC0_SSCTL0_R |= 0x00000060;

    ADC0_ACTSS_R |= 0b000000001; //Re-enable ADC0 SS0 by setting bit 0
                         // Set bits 1 and 2 for first sample

}

int adc_read(void)
{
    ADC0_PSSI_R = 0b0001;
    while(((ADC0_RIS_R & ADC_RIS_INR0)) ==0){

    }

    ADC0_ISC_R = 0x01;

    return ADC0_SSFIFO0_R;
}

int adc_avgValue()
{
    int i;
    int sum = 0;

    for (i=0; i<200; i++)
    {
        sum += adc_read();
    }
    return sum / 200;
}

double adc_convertToCm()
{
    //BOT 3
//           int sensorValue = adc_avgValue(); //Finds average value
//           double coefficient =12527; //Generated from excel
//           double otherPart =  pow((double)sensorValue, 0.9966); //Generated from excel
//           double finalValue = coefficient * otherPart; //Final conversion to cm
//           return finalValue;

    //BOT 8
//           int sensorValue = adc_avgValue(); //Finds average value
//                      double coefficient = 61326086.9421912; //Generated from excel
//                      double otherPart =  pow((double)sensorValue, -2.08330217653321); //Generated from excel
//                      double finalValue = coefficient * otherPart; //Final conversion to cm
//                      return finalValue;

    //BOT 1
           int sensorValue = adc_avgValue(); //Finds average value
           double coefficient = 1578519.16516506; //Generated from excel
           double otherPart =  pow((double)sensorValue, -1.507724506); //Generated from excel
           double finalValue = coefficient * otherPart; //Final conversion to cm
           return finalValue;
}
