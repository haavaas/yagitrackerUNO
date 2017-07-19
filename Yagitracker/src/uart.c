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
#include "motor.h"
volatile unsigned int last_rec_pos;
volatile unsigned int last_rec_tar;


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

char uart_receive_char(int channel){
	if(channel == 0){
		while(!(UCSR0A & (1<<RXC0))); //Wait for sending queue
		return UDR0;
	}else if(channel == 1){
		while(!(UCSR1A & (1<<RXC1))); //Wait for sending queue
		return UDR1;
	}
	else{
	return 0;
	}
}

void uart_init(void){
	DDRD &= ~(1<<PD2);
	DDRD |= (1<<PD3);
	UBRR0H = (MYUBBR>>8);
	UBRR0L = MYUBBR; // Define the registers
	UCSR0C = (1<<USBS0) | (3<<UCSZ00); // Set 8 bit
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Enable TX and RX
	
	
	UCSR1A |= (1<<RXC1);
	UBRR1H = (MYUBBR>>8);
	UBRR1L = MYUBBR; // Define the registers
	UCSR1C = (1<<USBS0) | (3<<UCSZ00); // Set 8 bit
	UCSR1B = (1<<RXCIE1)|(1<<RXEN0) | (1<<TXEN0); // Enable TX and RX
}

ISR(USART1_RX_vect){
	UCSR1B &= ~(1<<RXCIE1);
	uart_send_char('-',0);
	char message[255];
	if(uart_receive_char(1) == 'ÿ' && uart_receive_char(1) == 'ÿ'){
		short msg_size = uart_receive_char(1);
		if(msg_size == 0){
			goto mark;
		}
		short msg_id = uart_receive_char(1);
		short msg_cmd = uart_receive_char(1);
		uart_receive_char(1);
		uart_receive_char(1);
		uint8_t _data[20];  
		for(int i = 0; i < msg_size-7; i++){
			_data[i] = uart_receive_char(1);

			//uart_send_char(_data[i],0);
		}
		if(msg_cmd == CMD_ACK_STAT){
			last_rec_pos = (_data[7]<<8) + _data[6];
			last_rec_tar = (_data[5]<<8) + _data[4];
			sprintf(message, "STAT message: error%x status%x\n",_data[8],_data[9]);
			int i = 0;
			while(message[i] != '\0' && i<255){
				uart_send_char(message[i], 0);
				i++;
			}
		} else if(msg_cmd == CMD_ACK_RAM_READ || msg_cmd == CMD_ACK_EEP_READ){
			uart_send_char(_data[3], 0);
			if(msg_size == 12){
				uart_send_char(_data[4], 0);
			}
			uart_send_char('\n',0);
		} else if(msg_cmd == CMD_ACK_I_JOG){
		sprintf(message, "I_JOG message: error%x status%x\n",_data[0],_data[1]);
		int i = 0;
		while(message[i] != '\0' && i<255){
			uart_send_char(message[i], 0);
			i++;
		}
		}else if(msg_cmd == CMD_ACK_RAM_WRITE){
		sprintf(message, "RAM message: error%x status%x\n",_data[0],_data[1]);
		int i = 0;
		while(message[i] != '\0' && i<255){
			uart_send_char(message[i], 0);
			i++;
		}
	}
		sprintf(message, "Message: CMD%x ID%d Size=%d \n",msg_cmd, msg_id, msg_size);
		int j = 0;
		while(message[j] != '\0' && j<255){
			uart_send_char(message[j], 0);
			j++;
		}
	}
	mark:
	uart_send_char('\n',0);
	uart_send_char('/',0);
	UCSR1B |= (1<<RXCIE1);
}

/*ISR(USART1_RX_vect){
	uart_send_char(uart_receive_char(1),0);
}*/