/*
 * touch.cpp
 * version 3.0
 * Created: 23/01/2016 19:57:59
 * Last Update 13/05/16
 *  Author: Yaron Dori
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "touch.h"
#include "spiUnit.h"
#include "registers.h"

void touch::begin()
{
	PDD2 = 0;//irq
	
	//POD2 = 0;
	PDD7 = 1;//cs
	T_SS = 1;
	POD2 =1;
	
}

uint16_t touch::getX()
{
	uint8_t data1, data2;
	uint16_t dx;
	T_SS = 0;
	spi.transceiver(0x90);
	data1 = spi.transceiver(0x00);
	data2 = spi.transceiver(0x00);
	dx = (((data1<<8)+data2)>>3) &0xFFF;
	T_SS = 1;
	return (dx);
}

uint16_t touch::getY()
{
	uint8_t data1, data2;
	uint16_t dy;
	T_SS = 0;
	spi.transceiver(0xD0);
	data1 = spi.transceiver(0x00);
	data2 = spi.transceiver(0x00);
	dy = (((data1<<8)+data2)>>3) &0xFFF;
	T_SS = 1;
	return (dy);
}


void touch::readxy()
{
//	PDD2 = 1;//lock irq
	_delay_ms(10);
	spi.setClock(SPI_CLK_1MHZ);
	_x=getX();
	_y=getY();
		
	_x=(_x-270)/10.5;
	_y=(_y-300)/14;
	
	if (_x<0) _x=0;
	if (_x>320) _x=320;
	
	if (_y<0) _y=0;
	if (_y>320) _y=320;
	
	spi.setClock(SPI_CLK_8MHZ);
}

touch lcdtouch;
