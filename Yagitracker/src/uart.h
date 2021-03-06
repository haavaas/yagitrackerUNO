/*
 * uart.h
 *
 * Created: 31.08.2016 15:02:43
 *  Author: haavaas
 */ 


#ifndef UART_H_
#define display_H_
#include <stdio.h>

volatile unsigned int last_rec_pos;
volatile unsigned int last_rec_tar;
void uart_init(void);
int uart_send(char value, FILE __stream);
void uart_send_char(char value, int channel);
char uart_receive(FILE __stream);
char uart_receive_char(int channel);


#endif /* UART_H_ */