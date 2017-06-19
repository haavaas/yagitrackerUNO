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
//#define MYUBBR FOSC/16/BAUD-1
#define MYUBBR 8
#include "uart.h"



/*ISR(USART_RXC_vect){
   value = UDR;             //read UART register into value
}*/
int uart_send(char value, FILE __stream){
	while(!(UCSR0A & (1<<UDRE0))); //Wait for sending queue

	UDR0 = value;
	return 1;
}

void uart_send_char(char value, int channel){
	if(channel == 0){
		while(!(UCSR0A & (1<<UDRE0))); //Wait for sending queue
		UDR0 = value;
	}else if(channel == 1){
		while(!(UCSR1A & (1<<UDRE1))); //Wait for sending queue
		UDR1 = value;
	}
}

char uart_receive(FILE __stream){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
void uart_init(void)
{
	DDRD |= (1<<PD1);
	DDRD |= (1<<PD3);
	UBRR0H = (MYUBBR>>8);
	UBRR0L = MYUBBR; // Define the registers
	UCSR0C = (1<<USBS0) | (3<<UCSZ00); // Set 8 bit
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Enable TX and RX


	UBRR1H = (MYUBBR>>8);
	UBRR1L = MYUBBR; // Define the registers
	UCSR1C = (1<<USBS0) | (3<<UCSZ00); // Set 8 bit
	UCSR1B = (1<<RXEN0) | (1<<TXEN0); // Enable TX and RX
}
//ISR(USART0_RXC_vect ){
//	PORTB<<uart_receive();
//}
