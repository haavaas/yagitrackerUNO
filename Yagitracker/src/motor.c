/*
 * motor.c
 *
 * Created: 13.06.2017 10.25.24
 *  Author: user
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "motor.h"
#include "uart.h"

unsigned short checksum_1;
unsigned short checksum_2;
uint8_t _settings_1byte[] = {RAM_ACK_Policy, 0x02, RAM_ACC_Ratio, 0x00, RAM_Calibration_Difference, 0x00}; //Will also be written to EEPROM if the DO_EEP_WRITE in motor.h is set, RAM addresses will automatically be converted to EEP addresses if this is done
uint8_t _settings_2byte[] = {RAM_POS_Kp, 0x00, 0x02, RAM_POS_Kd, 0x00, 0x01, RAM_POS_Ki, 0x00, 0x00};

void A1_16_init(void){
	DDRD &= ~(1<<2);			//set the RXD input
	PORTD |= (1<<2);		//pull-high the RXD pinout
	A1_16_Basic(0x01,CMD_ROLLBACK);
	_delay_ms(5000);
	//A1_16_WriteData(254, CMD_RAM_WRITE, 0x00, 0x01);
	for(int i = 0; i < sizeof(_settings_1byte); i+=2){
		if(_settings_1byte[i] >= 0 && _settings_1byte[i] <= 47 ){
			A1_16_WriteData(0x01, CMD_RAM_WRITE, _settings_1byte[i], _settings_1byte[i+1]);
			uart_send_char('h',0);
			_delay_ms(20);
			if(DO_EEP_WRITE){
				A1_16_WriteData(0x01, CMD_EEP_WRITE, _settings_1byte[i]+6, _settings_1byte[i+1]);
				_delay_ms(20);
			}
		} else {uart_send_char(_settings_1byte[i],0);}
	}
	for(int i = 0; i < sizeof(_settings_2byte); i+=3){
		if(_settings_2byte[i] >= 12 && _settings_2byte[i] <= 36 ){
			A1_16_WriteData2(0x01, CMD_RAM_WRITE, _settings_2byte[i], _settings_2byte[i+1], _settings_2byte[i+2]);
			if(DO_EEP_WRITE){
				A1_16_WriteData2(0x01, CMD_EEP_WRITE, _settings_2byte[i]+6, _settings_2byte[i+1], _settings_2byte[i+2]);
			}
		}
	}
	//A1_16_Basic(0x01,0x08);
	_delay_ms(20);
}

void A1_16_SetPosition(unsigned char _pID, unsigned char _CMD,  unsigned char _playtime, unsigned int _position){
	static unsigned int _data[5];
	static int _i = 0;
	
	if(_CMD == CMD_S_JOG){
		_data[0] = _playtime;
		_data[1] = _position&0xff;
		_data[2] = (_position&0xff00)>>8;
		_data[3] = 0x00;					//set:0(position control), 1(speed control), 2(torque off), 3(position servo on)
		_data[4] = _pID;
	}
	else if (_CMD == CMD_I_JOG){
		_data[0] = _position&0xff;
		_data[1] = (_position&0xff00)>>8;
		_data[2] = 0x00;					//set:0(position control), 1(speed control), 2(torque off), 3(position servo on)
		_data[3] = _pID;
		_data[4] = _playtime;
	}
	checksum_1 = (0x0c)^_pID^_CMD;		//package_size^pID^CMD
	for(_i = 0;_i < 5;_i++) checksum_1 ^= _data[_i];
	checksum_1 &= 0xfe;
	checksum_2 = (~checksum_1)&0xfe;
	
	char _header[7] = {0xff, 0xff, 0x0c, _pID, _CMD, checksum_1, checksum_2};
	/*uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0x0c);				
	uart_send_char(_pID);
	uart_send_char(_CMD);
	uart_send_char(checksum_1);
	uart_send_char(checksum_2);
	
	for(_i = 0;_i < 5;_i++) uart_send_char(_data[_i]);
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

void A1_16_WriteData(unsigned char _pID, unsigned char _CMD, unsigned char _addr_start, char _data_write){
	checksum_1 = (10^_pID^_CMD^_addr_start^0x01^_data_write)&0xfe;
	checksum_2 = (~checksum_1)&0xfe;
	uart_send_char(0xff, 1); 
	uart_send_char(0xff, 1); 
	uart_send_char(10, 1); 	//package size
	uart_send_char(_pID, 1); 
	uart_send_char(_CMD, 1); 
	uart_send_char(checksum_1, 1); 
	uart_send_char(checksum_2, 1); 
	uart_send_char(_addr_start, 1); 
	uart_send_char(0x01, 1); 			//length of data
	uart_send_char(_data_write, 1); 
}

void A1_16_WriteData2(unsigned char _pID, unsigned char _CMD, unsigned char _addr_start, char BYTE_1, char BYTE_2){
	checksum_1 = (11^_pID^_CMD^_addr_start^0x02^BYTE_1^BYTE_2)&0xfe;
	checksum_2 = (~checksum_1)&0xfe;
	uart_send_char(0xff, 1); 
	uart_send_char(0xff, 1); 
	uart_send_char(11, 1); 			//package size
	uart_send_char(_pID, 1); 
	uart_send_char(_CMD, 1); 
	uart_send_char(checksum_1, 1); 
	uart_send_char(checksum_2, 1); 
	uart_send_char(_addr_start, 1); 
	uart_send_char(0x02, 1); //length of data
	uart_send_char(BYTE_1, 1); 
	uart_send_char(BYTE_2, 1); 
}

void A1_16_ReadData(unsigned char _pID, unsigned char _CMD, unsigned char _addr_start, char _data_length){
	checksum_1 = (9^_pID^_CMD^_addr_start^_data_length)&0xfe;
	checksum_2 = (~checksum_1)&0xfe;
	uart_send_char(0xff, 1);
	uart_send_char(0xff, 1);
	uart_send_char(9, 1); 	//package size
	uart_send_char(_pID, 1);
	uart_send_char(_CMD, 1);
	uart_send_char(checksum_1, 1);
	uart_send_char(checksum_2, 1);
	uart_send_char(_addr_start, 1);
	uart_send_char(_data_length, 1);
}