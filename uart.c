/*
 * uart.c
 *
 *  Created on: Feb 19, 2019
 *      Author: tvpham
 */

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


#define BIT00x01
#define BIT10x02
#define BIT20x04
#define BIT30x08
#define BIT40x10
#define BIT50x20
#define BIT60x40
#define BIT70x80

char msg[20];
int len = 0;

void uart_init();
void uart_sendChar(char data);

char uart_receive(void)
{
    char data = 0;
    while(UART1_FR_R & UART_FR_RXFE)
    {

    }

    data = (char)(UART1_DR_R & 0xFF);

    return data;

}
void UART1_Handler2(void)
 {
    char data = 0;
                   int i = 0;
                   int len = 0;
                   for (i=0; i<20; i++){
                       data = uart_receive();
                       if ((data == '\r')){
                           msg[i] = '\n';
                           len++;
                           break;
                       }

                       msg[i] = data;
                       lcd_clear();
                       lcd_printf("%d", len+1);
                       len++;

                 }
                   int j;
                   lcd_clear();
                   for(j=0; j<len; j++){
                       if (msg[j] == '\r')
                       {
                           msg[j] = '\n';
                       }

                       lcd_putc(msg[j]);
                       uart_sendChar(msg[j]);

                   }



}
void UART1_Handler(void)
{
       lcd_clear();
       int i;

       char data = 0;
       lcd_printf("%d", len+1);
       data = uart_receive();
       msg[len] = data;
       len++;
       printf("%c", data);
       if (len == 20 || data == '\r'){
           lcd_clear();

           if (data == '\r'){
               len--;
           }
           for (i=0; i < len; i++)
           {
               lcd_putc(msg[i]);
               printf("%c", msg[i]);
           }
           len = 0;
   }

}


void uart_init2()
{

       SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010; //Turns on clock for port B
       SYSCTL_RCGCUART_R= SYSCTL_RCGCUART_R| 0x02; //Turns on clock for UART1

       GPIO_PORTB_AFSEL_R = 0x03; //Enables alternate functions
       GPIO_PORTB_PCTL_R = 0x00000011; //Assigns UART signals to appropriate pins

       GPIO_PORTB_DEN_R |= (BIT1 | BIT0); //Sets pins 0 and 1 to digital
       GPIO_PORTB_DIR_R &= ~BIT0; //Set pin 0 to input
       GPIO_PORTB_DIR_R |= BIT1; //Set pin 1 to output

       UART1_CTL_R &= ~UART_CTL_UARTEN; //Disables UART while its set up

       UART1_IBRD_R = 8; //16,000,000 / (16 * 9600) = 104.16666
       UART1_FBRD_R = 44;  // .1666*64+.5 = 11.16666
       UART1_CC_R = 0; //Use system clock as UART clock source

       UART1_LCRH_R = 0b01100000; //Sets frame format

       UART1_ICR_R |= 0b00010000; // Clear RX interrupt status flag
       UART1_IM_R  |= 0b00010000; // Enable RX interrupts

       NVIC_PRI1_R |= 0x00002000;//Set UART0 IRQ priority to 1(bits 15-13)
       NVIC_EN0_R |= 0b1000000; //Enables interrupt for UART1


       IntRegister(INT_UART1, UART1_Handler); //Binds interrupt to its handler

       UART1_CTL_R |= 0000001100000001; //Re-enable UART1
      // UART1_CTL_R = UART1_CTL_R |0x01;
       IntMasterEnable(); //Allows CPU to service interrupts

}
void uart_init()
{
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010; //Turns on clock for port B
    SYSCTL_RCGCUART_R= SYSCTL_RCGCUART_R| 0x02; //Turns on clock for UART1

    GPIO_PORTB_AFSEL_R |= 0x03; //Enables alternate functions
    GPIO_PORTB_PCTL_R |= 0x00000011; //Assigns UART signals to appropriate pins

    GPIO_PORTB_DEN_R |= (BIT1 | BIT0); //Sets pins 0 and 1 to digital
    GPIO_PORTB_DIR_R &= ~BIT0; //Set pin 0 to input
    GPIO_PORTB_DIR_R |= BIT1; //Set pin 1 to output

    UART1_CTL_R &= ~UART_CTL_UARTEN; //Disables UART while its set up

    UART1_IBRD_R = 8; //16,000,000 / (16 * 9600) = 104.16666
    UART1_FBRD_R = 44;  // .1666*64+.5 = 11.16666
    UART1_CC_R = 0; //Use system clock as UART clock source

    UART1_LCRH_R = 0b01100000; //Sets frame format


 //   IntRegister(INT_UART1, UART1_Handler); //Binds interrupt to its handler

    UART1_CTL_R |= 0000001100000001; //Re-enable UART1
   // UART1_CTL_R = UART1_CTL_R |0x01;
    IntMasterEnable(); //Allows CPU to service interrupts
}

void uart_sendChar(char data)
{
    while(UART1_FR_R & 0x20)
    {

    }

    UART1_DR_R = data;

}

void uart_sendStr(const char *data)
{
    //until we reach a null character
    while (*data != '\0')
    {
        //send the current character
        uart_sendChar(*data);
        // increment the pointer.
        data++;
    }

}





