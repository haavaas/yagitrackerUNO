/*
 * uart.h
 *
 * Created: 31.08.2016 15:02:43
 *  Author: haavaas
 */ 


#ifndef UART_H_
#define display_H_
#include <stdio.h>


void uart_init(void);
volatile unsigned char value;
int uart_send(char value, FILE __stream);
char uart_receive(FILE __stream);


#endif /* UART_H_ */