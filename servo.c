/*
 * servo.c
 *
 *  Created on: Mar 27, 2019
 *      Author: tvpham
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

//Values for BOT 8 given in comments, currently works for BOT 12


void servo_init()
{


    SYSCTL_RCGCGPIO_R |= 0x2; //Enable clock for port B
    SYSCTL_RCGCTIMER_R |= 0x2; // turns on clock for timer1

    GPIO_PORTB_DEN_R |= 0b00100000; //Enables port 5
    GPIO_PORTB_DIR_R |= 0b00100000; //Sets port 5 as output
    GPIO_PORTB_AFSEL_R |= 0b00100000;
    GPIO_PORTB_PCTL_R = 0x700000;   //T1CCP1


    TIMER1_CTL_R &= ~0x0100; //Disable timer 1 for setup
    TIMER1_CFG_R = 0x4; //Selects 16 bit size of timer
    TIMER1_TBMR_R |= 0b1010; //Enable PWM and periodic
    TIMER1_TBPR_R = 0x4;
    TIMER1_TBILR_R = 0xE200;
    TIMER1_TBPMR_R = 0x4;

    //0 Degrees is 0xC6C0 (BOT 8)
    //180 degrees is 0x5CF0 (BOT 8)
    //Slope is 69D0 (BOT 8)
    //90 Degrees is 0x91D8 (BOT 8)

    //90 degrees is 0x86D8 (BOT 12)
    //0 degrees 0xC0C0 (BOT 12)
    //180 is 0x4FF0 (BOT 12)
    //Slope is 70D0

    //90 degrees is 0x8CD8 (BOT 3)
    //0 degrees IS 0xC4C0 (BOT 3)
    //180 degrees is 0x5650
    //slope is 6E70

    //0 degrees is 0x11D8


    TIMER1_TBMATCHR_R = 0x8CD8;

    TIMER1_CTL_R |= 0x100;//Enable timer 1
    timer_waitMillis(1000);
}

void servo_move(double degrees)
{
    if (degrees > 180 || degrees < 0) //checks range
    {
        return;
    }
    double deg = 180 - degrees;

   // TIMER1_TBMATCHR_R = 0x69D0 * deg / 180 + 0x5CF0; //sets match value //BOT 8
  //  TIMER1_TBMATCHR_R = 0x6BD0 * deg / 180 + 0x5AF0;
    //  TIMER1_TBMATCHR_R = 0x6E70 * deg / 180 + 0x5650; //BOT 3
    TIMER1_TBMATCHR_R = 0x6E70 * deg / 180 + 0x5650; //Bot 01
}
