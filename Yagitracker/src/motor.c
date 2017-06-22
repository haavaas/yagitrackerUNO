/*
 * motor.c
 *
 * Created: 13.06.2017 10.25.24
 *  Author: user
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "motor.h"
#include "uart.h"

unsigned short checksum_1;
unsigned short checksum_2;

void A1_16_Ini(void){
	DDRD &= ~(1<<2);			//set the RXD input
	PORTD |= (1<<2);		//pull-high the RXD pinout
}

void A1_16_SetPosition(unsigned char _pID, unsigned char _CMD,  unsigned char _playtime, unsigned int _position){
	static unsigned int _data[5];
	static int _i = 0;
	
	if(_CMD == CMD_S_JOG){
		_data[0] = _playtime;
		_data[1] = _position&0xff;
		_data[2] = (_position&0xff00)>>8;
		_data[3] = 0;					//set:0(position control), 1(speed control), 2(torque off), 3(position servo on)
		_data[4] = _pID;
	}
	else if (_CMD == CMD_I_JOG){
		_data[0] = _position&0xff;
		_data[1] = (_position&0xff00)>>8;
		_data[2] = 0;					//set:0(position control), 1(speed control), 2(torque off), 3(position servo on)
		_data[3] = _pID;
		_data[4] = _playtime;
	}
	checksum_1 = (0x0c)^_pID^_CMD;		//package_size^pID^CMD
	for(_i = 0;_i < 5;_i++) checksum_1 ^= _data[_i];
	checksum_1 &= 0xfe;
	checksum_2 = (~checksum_1)&0xfe;
	
	char _header[7] = {0xff, 0xff, 0x0c, _pID, _CMD, checksum_1, checksum_2};
	/*Serial1.write(0xff);
	Serial1.write(0xff);
	Serial1.write(0x0c);				
	Serial1.write(_pID);
	Serial1.write(_CMD);
	Serial1.write(checksum_1);
	Serial1.write(checksum_2);
	
	for(_i = 0;_i < 5;_i++) Serial1.write(_data[_i]);
	*/
	for(int i = 0; i < 7; i++){
		uart_send_char(_header[i], 1);
	}
	for(int i = 0; i < 5; i++){
		uart_send_char(_data[i], 1);
	}
}


void A1_16_Basic(unsigned char _pID, unsigned char _CMD){
	checksum_1 = (7^_pID^_CMD)&0xfe;
	checksum_2 = (~checksum_1)&0xfe;
	uart_send_char(0xff, 1);          //header
	uart_send_char(0xff, 1);          //header
	uart_send_char(7, 1);				//package size
	uart_send_char(_pID, 1);
	uart_send_char(_CMD, 1);
	uart_send_char(checksum_1, 1);
	uart_send_char(checksum_2, 1);
}