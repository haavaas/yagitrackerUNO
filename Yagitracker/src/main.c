/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "uart.h"
#include "motor.h"
#include "adc.h"
int last_written_to_motor = -1;


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	board_init();
	uart_init();
	adc_init();
	DDRB |= (1<<PB7);
	sei();
	A1_16_init();
	long a =0;
	char to_screen[255];
	A1_16_Basic(0x01, CMD_STAT);
	_delay_ms(100);
	A1_16_SetPosition(0x01,CMD_I_JOG,100,512);
	while(1){
		_delay_ms(50);
		//UDR0 = 'a';
		//uart_send_char('a');
		char pot_value[4];
		//ADCSRA |= (1<<ADSC);
		//A1_16_Basic(1, CMD_STAT);
		//PORTB |= (1<<PB7);
		//_delay_ms(2000);
		//A1_16_SetPosition(0x01,CMD_I_JOG,200,0x0000);
		//_delay_ms(2);
		//A1_16_Basic(1, CMD_STAT);
		//PORTB &= ~(1<<PB7);
		//_delay_ms(50);
		//A1_16_SetPosition(0x01,CMD_I_JOG,200,0x03ff);
		//uart_receive_char(1);
		//A1_16_ReadData(0x01,0x02,0x07,0x01);
		if(last_adc_value != last_written_to_motor){
			A1_16_SetPosition(0x01,CMD_I_JOG,0,last_adc_value);
			last_written_to_motor = last_adc_value;
			/*itoa(last_adc_value, pot_value, 10);
			for(int i = 0; i<4; i++){
				uart_send_char(pot_value[i], 0);
			}
			uart_send_char('\n', 0);*/
		}
		a++;
		if(a==10000){
			sprintf(to_screen, "Real=%x, target=%x, adc=%x, error:%x\n", last_rec_pos, last_rec_tar, last_written_to_motor, last_written_to_motor - last_rec_pos);
			int i = 0;
			/*while(to_screen[i] != '\0' && i<255){
			uart_send_char(to_screen[i], 0);
			i++;
			}*/
			A1_16_Basic(0x01, CMD_STAT);
			//A1_16_WriteData(0x01, CMD_RAM_WRITE, 47, 0);
			//A1_16_ReadData(0x01, CMD_RAM_READ, RAM_POS_Kp, 1);
			a=0;
		}
	}
}

