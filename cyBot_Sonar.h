/*
 * sonar.h
 *
 * A Sonar library for the 288 CyBots
 *
 * Put this in the root of your project folder along with
 * cyBot_Sonar.lib
 *
 * Be sure to include Timer.h in the project as well.
 *
 *  Created on: Apr 1, 2019
 *      Author: Omair Ijaz, Sam Stifter
 */

#ifndef CYBOT_SONAR_H_
#define CYBOT_SONAR_H_


#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

/**
 * Initializes GPIO and TIMER for use as sonar. MUST be called
 * prior to using the ping_read function.
 */
void TIMER3B_init(void);

/**
 * Tells the ping device to start a reading (send_ping pulses
 * the GPIO high for 5 microseconds). Then computes the difference
 * in timer values and returns the time converted to centimeters.
 */
double ping_read(void);



#endif /* CYBOT_SONAR_H_ */
