/*
V1.0.0  Oct 23, 2016
ADS1220RTD_Shield_Library_V2.h - Library for reading ADS1220 chip for 3wire RTD For Temperatures (-50C to 600C)
using Brewno V2 RTD Shield for Arduino Uno
Quadratic Callendar Van Dusen Formula used; error from calculation increases as temperature gets more negative
See notes in ADS1220RTD.cpp getTempDegC function.  Error due to formula at -50C ~= 0.01C

Copyright (C) 2016 David Wiebe <dvwiebe@gmail.com> WieBeBrewing.com
This Library is licensed under a GPLv3 License.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See <http://www.gnu.org/licenses/> for a copy of the GNU General Public License
*/

#ifndef ADS1220RTD_Brewno_Library_V2_1_h
#define ADS1220RTD_Brewno_Library_V2_1_h

#include <Arduino.h>
#include <Wire.h>

class ADS1220RTD
{
public:
	//ADS1220RTD class initialization
	ADS1220RTD(int address, unsigned char userGain, unsigned int userSPS, float user_a, float user_b);
	//RTD shield Initialization Function
	void rtdInitialize();
	//Perform offset calibration of ADS1220 chip
	void offsetCalibration();
	//Reset RTD Shield; called on shield power up and should not be needed by user.
	void reset();	
	//Read Functions the user may call 
	long int getRTDBytes(int index);
	float getResistance(int rtd);
	float getTemperatureC(int rtd);
	float getTemperatureF(int rtd);	
	void changeSlaveAddress(unsigned char newAddress);

private:
	//Called by other funcitons to recieve float values from shield
	void getFloatBytes(int index);	 
		
	/*
	* rtdUnion details - holds all float values for all RTDs so that these values
	* can be converted back forth to 8-bit chunks; 4 Bytes in Each Row
	*
	* Row Index Assignment
	* [0 thru 4] - RTD Resistance (5th spot is for the Offset value if user requests it)
	* [5 thru 8] - RTD Temperature in deg C
	* [9 thru 12] - RTD Temperature in deg F
	*
	*/
	union rtdUnion
	{
		float f[13];
		unsigned char b[13][4];
	}rtdFloatValues;

	union paramterUnion
	{
		float f;
		unsigned char b[4];
	}a_parameter, b_parameter;	

	//shield settings initialized by user when creating instance of class
	int slaveAddress; //the arduino Wire.h library requires an int type
	unsigned char gain;
	unsigned char sps;	

};

#endif