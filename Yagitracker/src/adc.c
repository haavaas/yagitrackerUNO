/*
 * adc.c
 *
 * Created: 13.06.2017 11.47.22
 *  Author: user
 */ 
#include <avr/io.h>
#include "asf.h"
#include "uart.h"
volatile int last_adc_value;

int adc_init(void){
	DDRF &= ~(1<<0);
	last_adc_value = 52;

	
	ADCSRA = 0b10101111; //Set adc enable, auto trigger enable, interrupt enable, divition factor 128
	ADCSRB &= ~0b00000111; // Set free running auto trigger
	ADMUX = 0b01000001; // Set 5V ref, left adjusted at ADC0 
	ADCSRA |= (1<<ADSC);
	return 1;
}

ISR(ADC_vect){
	last_adc_value = ADC;
	last_adc_value = last_adc_value&0x03ff;
}