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



void detect()
{


    timer_waitMillis(300);

   struct dataPoint data[181];
   struct detectedObject detectedObjects[10];

    char msg[30]; //Message to be sent to putty
    int i = 0;
    sprintf(msg, "Degrees\tIR Distance\tSonarDistance\n\r"); //Header for putty message
    uart_sendStr(msg); //Send header message to putty

    int objectFound = 0; //Keeps track of objects found
    int objectIndex = 0;

    for (i=0; i<=180; i++) //Check values for all angles 0-180
    {
        servo_move(i); //Move servo
        timer_waitMillis(50);
        data[i].irDistance = adc_convertToCm();
        data[i].sonarDistance = ping_read();

        if (data[i].irDistance <= 50 && abs(data[i].sonarDistance - data[i].irDistance) <= 5) //If sensors are close in value, object is detected
        {
            if (objectFound == 0)
            {
                detectedObjects[objectIndex].startIndex = i; //If object newly detected, assign current angle to start of object
                objectFound++;
            }
            else
            {
                detectedObjects[objectIndex].endIndex = i + objectFound; //increments the end index for every consecutive object detection
                objectFound++;
            }
        }
        else if(data[i].irDistance > 50 || abs(data[i].sonarDistance - data[i].irDistance) > 5)
        {
            if (objectFound >= 4) //If object was previously detected
            {
                detectedObjects[objectIndex].endIndex = i; //Increase object count and reset to no object being detected
                objectIndex++;
                objectFound = 0;
            }
            else if (objectFound < 4 && objectFound > 0)
            {
                objectFound = 0;
            }
        }

//        lcd_printf("%d degrees\nPING %lf cm\nIR %lf cm", i, data[i].sonarDistance, data[i].irDistance);
//        timer_waitMillis(100);
//
//        sprintf(msg, "%-7d %-15.2lf %-10.2lf\n\r", i, data[i].sonarDistance, data[i].irDistance);
//        uart_sendStr(msg);
    }

    int smallestObjectIndex = 0;

    //Law of cosines
    double a = data[detectedObjects[0].startIndex].sonarDistance;
    double b = data[detectedObjects[0].endIndex].sonarDistance;
    double otherThing = -2*a*b*cos((detectedObjects[0].endIndex - detectedObjects[0].startIndex) * (3.14/180));
    double currentMin = sqrt((a*a) + (b*b) + otherThing);

    for (i=0; i<objectIndex; i++)
    {
         a = data[detectedObjects[i].startIndex].sonarDistance;
         b = data[detectedObjects[i].endIndex].sonarDistance;
         otherThing = -2*a*b*cos((detectedObjects[i].endIndex - detectedObjects[i].startIndex) * (3.14/180));
         double currentWidth = sqrt((a*a) + (b*b) + otherThing);
        // lcd_printf("Object %d has width of %d", i, currentWidth);

        if (currentWidth < currentMin)
        {
            smallestObjectIndex = i;
            currentMin = currentWidth;
        }
    }
    lcd_printf("%d objects found\nSmallest object is %d\nwidth is %lf", objectIndex, smallestObjectIndex, currentMin);
//    servo_move((detectedObjects[smallestObjectIndex].endIndex + detectedObjects[smallestObjectIndex].startIndex) / 2);
    servo_move((detectedObjects[smallestObjectIndex].endIndex + detectedObjects[smallestObjectIndex].startIndex) / 2.0);
}



