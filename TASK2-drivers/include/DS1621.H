/*
 * DS1621 - library for DS1621 digital thermometer
 * This library is intended to be used with Aruino Wire library functions
 *
 * Version 1.0
 * Last updated - 5/9/17
 * John Karatka
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 ************************************************************************
 *
 * Wiring diagram for digital thermometer DS1621
 *
 *         ----
 *  1 SDA-|    |-VDD 8
 *  2 SCL-|    |-A0  7
 *  3 TOU-|    |-A1  6
 *  4 GND-|    |-A2  5
 *         ----
 */

#ifndef DS1621_H
#define DS1621_H


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum status
{
    OK = 0,
    DataTooLong,
    NACKaddress,
    NACKdata,
    Other,
    TimeOut
};

enum mode {
    shot_mode1 = 0x01 ,cont_conv = 0x00
};

class TempSense {
private:
	int address;
	int configSettings;
	int8_t temperature;

	void checkConvert();
	void updateTemp();
	void updateConfig();

public:
	TempSense(int TempSenseAddress);
	status write_register(uint8_t reg, uint8_t *data, uint8_t len);
	status read_register(uint8_t reg, uint8_t *data, uint8_t len);
	status start_conv();
	int16_t getTempF();
	int16_t getTempC();
	void setTrigger(int16_t tempTrigger);
	void setReset(int16_t tempReset);
	int getConfig();
};

#endif
