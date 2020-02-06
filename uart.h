/*
 * uart.h
 *
 *  Created on: Feb 23, 2019
 *      Author: tvpham
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

void UART1_Handler();
void uart_init();
void uart_init2();
void uart_sendChar(char data);
char uart_receive();
void uart_sendStr(const char *data);



#endif /* UART_H_ */
