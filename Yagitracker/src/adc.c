/*
 * adc.c
 *
 * Created: 13.06.2017 11.47.22
 *  Author: user
 */ 
#include <avr/io.h>
#include "asf.h"

volatile int last_adc_value;

int adc_init(void){
	DDRA &= ~((1<<0)|(1<<1));
	PORTA |= (1<<0)|(1<<1);
	

	
	ADCSRA = 0b10101111; //Set adc enable, auto trigger enable, interrupt enable, divition factor 128
	ADCSRB &= ~0b00000111; // Set free running auto trigger
	ADMUX = 0b01100000; // Set 5V ref, left adjusted at ADC0 
	sei();
	ADCSRA |= (1<<ADSC);
}

ISR(ADC_vect){
	last_adc_value = ADC;
}