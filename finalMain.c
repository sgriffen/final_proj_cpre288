/**
 * main.c
 * @author:
 * @date:
 */

#include "cyBot_Sonar.h"
#include "uart.h"
#include "lab5_scan_data.h"
#include <stdio.h>
#include "adc.h"
#include "lcd.h"
#include "timer.h"
#include <math.h>
#include <string.h>
#include "servo.h"
#include "button.h"
#include "objectDetection.h"
// EVERYTHING CALIBRATED FOR BOT 8

struct dataPoint
{
    double irDistance; //In centimeters
    double sonarDistance; //In centimeters

};

struct detectedObject
{
    int startIndex;
    int endIndex;
};
void main()
{
    servo_init(); //Initialize servo (init altered to start at 0 degrees)
    lcd_init(); //Initialize LCD
    adc_init();
    TIMER3B_init();
    uart_init();

    detect();
}



