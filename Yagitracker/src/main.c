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


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	board_init();
	uart_init();
	adc_init();
	A1_16_Ini();
	while(1){
		//UDR0 = 'a';
		//uart_send_char('a');
		char pot_value[5];
		itoa(last_adc_value, pot_value, 10);
		for(int i = 0; i<5; i++){
			uart_send_char(pot_value[i], 0);
		}
		uart_send_char('\n', 0);
		ADCSRA |= (1<<ADSC);
		_delay_ms(2000);
		A1_16_Basic(1, CMD_STAT);
		_delay_ms(2000);
		A1_16_SetPosition(0x01,CMD_I_JOG,0,0x0000);
		_delay_ms(2000);
		A1_16_Basic(1, CMD_STAT);
		_delay_ms(2000);
		A1_16_SetPosition(0x01,CMD_I_JOG,0,0x03ff);
		_delay_ms(2000);
		A1_16_Basic(1, CMD_STAT);
		_delay_ms(2000);
		A1_16_SetPosition(0x01,CMD_I_JOG,0,0x0000);
		_delay_ms(2000);
		A1_16_Basic(1, CMD_STAT);
		_delay_ms(2000);
		A1_16_SetPosition(0x01,CMD_I_JOG,0,0x03ff);
	}
}
