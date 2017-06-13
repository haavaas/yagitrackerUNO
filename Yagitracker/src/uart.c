/*
 * uart.c
 *
 * Created: 31.08.2016 15:02:29
 *  Author: haavaas
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define FOSC 16000000
#define F_CPU 16000000
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdint.h>
#define BAUD 115200
#define MYUBBR FOSC/16/BAUD-1
#include "uart.h"

volatile unsigned char value;


/*ISR(USART_RXC_vect){
   value = UDR;             //read UART register into value
}*/
int uart_send(char value, FILE __stream){
	while(!(UCSR0A & (1<<UDRE0))); //Wait for sending queue

	UDR0 = value;
	return 1;
}
char uart_receive(FILE __stream){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
void uart_init(void)
{
	UBRR0H = (MYUBBR>>8);
	UBRR0L = MYUBBR; // Define the registers
	UCSR0C = (1<<USBS0) | (3<<UCSZ00); // Set 8 bit
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Enable TX and RX
}
//ISR(USART0_RXC_vect ){
//	PORTB<<uart_receive();
//}
