/*
V1.0.0  Oct 23, 2016
ADS1220RTD.cpp - Library for reading ADS1220 chip for 3wire RTD For Temperatures (-50C to 600C)
Quadratic Callendar Van Dusen Formula used; error from calculation increases as temperature gets more negative
See Notes below for getTempDegC function.  Error due to formula at -50C ~= 0.01C

Copyright (C) 2016 David Wiebe <dvwiebe@gmail.com> WieBeBrewing.com
This Library is licensed under a GPLv3 License.  See ADS1220RTD.h for License Details

*/


#include <ADS1220RTD_Brewno_Library_V2_1.h>

/*******************************Public Functions****************************************/
//Creation of new instance of class ADS1220
ADS1220RTD::ADS1220RTD(int address, unsigned char userGain, unsigned int userSPS, float user_a, float user_b) {
	
	slaveAddress = address; 
	gain = userGain;
	sps = userSPS / 10; //divide by 10 to keep number 8bits for shield program ( 20 <= userSPS <= 2000)
	a_parameter.f = user_a;
	b_parameter.f = user_b;
	
}
//RTD Initialization to be called to load settings to shield; to be called in Setup()
void ADS1220RTD::rtdInitialize() {
	int garbage;
		
	//Send gain and sps settings first
	Wire.beginTransmission(slaveAddress);
	Wire.write(0x00); //Instruction to record gain and sps settings
	Wire.write(gain);
	Wire.write(sps);
	Wire.endTransmission();

	//Send a and b parameters for temperature calculations
	Wire.beginTransmission(slaveAddress);
	Wire.write(0x02); //instruction to write a and b parameters to PIC variables
	Wire.write(a_parameter.b[0]); //transmit float in 4 bytes 
	Wire.write(a_parameter.b[1]); 
	Wire.write(a_parameter.b[2]);
	Wire.write(a_parameter.b[3]);
	Wire.write(b_parameter.b[0]); //transmit float in 4 bytes 
	Wire.write(b_parameter.b[1]);
	Wire.write(b_parameter.b[2]);
	Wire.write(b_parameter.b[3]);
	Wire.endTransmission();

	Wire.beginTransmission(slaveAddress);
	Wire.write(0x13); //Instruction to Initialize RTD
	Wire.endTransmission();
	Wire.requestFrom(slaveAddress, 1);

	while (Wire.available()) garbage = Wire.read(); //once byte received, rtd is initialized 	

}
//Performs internal calibration of ADS1220 offset voltage; this is performed upon RTD initialization
//but user may need to call if conditions change (eg. chip temperature changes drastic or if
//RTD initialized when sensor not plugged in)
void ADS1220RTD::offsetCalibration() {
	int garbage;
		
	Wire.beginTransmission(slaveAddress);
	Wire.write(0x13); //Instruction to re-initialize RTD which involves performing offset calibration
	Wire.endTransmission();
	Wire.requestFrom(slaveAddress, 1);

	while (Wire.available()) garbage = Wire.read(); //once byte received, rtd is initialized	
}
//Perform Shield Reset; this is called on shield power up and should not need to be called
//by the user
void ADS1220RTD::reset() {
	int garbage;
		
	Wire.beginTransmission(slaveAddress);
	Wire.write(0x14);
	Wire.endTransmission();
	
	Wire.requestFrom(slaveAddress, 1);

	while (Wire.available()) garbage = Wire.read(); //once byte received, shield is reset
	
}

long int ADS1220RTD::getRTDBytes(int index) {

	//local variables to read data
	int i = 0;
	int tempArray[3];
	long int tempBit32;
		
	//send instruction to shield that readings in bytes required
	Wire.beginTransmission(slaveAddress);
	Wire.write(index);
	Wire.endTransmission();

	Wire.requestFrom(slaveAddress, 3);
	//receive bytes from shield
	while (Wire.available()) {
		tempArray[i] = Wire.read();
		i++;
	}	

	tempBit32 = tempArray[0];
	tempBit32 = (tempBit32 << 8) | tempArray[1];
	tempBit32 = (tempBit32 << 8) | tempArray[2];

	//RTD readings will always be positive as voltage upstream of RTD always higher
	//however user may request chip offset reading which could be negative
	//as we received a 24 bit number, bit shift to carry negative sign if present

	tempBit32 <<= 8;
	tempBit32 >>= 8;

	return tempBit32;


}
//returns resistance in ohms for requested RTD; rtd value of 4 returns offset value
float ADS1220RTD::getResistance(int rtd) {
	
	//rtd number +5 for correct call to shield
	getFloatBytes(rtd + 5);
	return rtdFloatValues.f[rtd];
	

}
//returns temperature in degrees Celsius for requested RTD
float ADS1220RTD::getTemperatureC(int rtd) {
	//rtd number +10 for correct call to shield
	getFloatBytes(rtd + 10);
	return rtdFloatValues.f[rtd + 5];
	
}
//returns temperature in degress Farenheit for requested RTD
float ADS1220RTD::getTemperatureF(int rtd) {
	
	//rtd number +14 for correct call to shield
	getFloatBytes(rtd + 14);
	return rtdFloatValues.f[rtd + 9];
	
}

//change shield I2C slave address if needed; default slave address is 0x08
void ADS1220RTD::changeSlaveAddress(unsigned char newAddress) {
	
	Wire.beginTransmission(slaveAddress);
	Wire.write(0x08); //write instruction to change I2C slave address; coincidence this equal default address
	Wire.write(newAddress);
	Wire.endTransmission();
	
}

/*******************************Private Functions****************************************/
//Called by some of the public user invoked functions to read float values from shield
void ADS1220RTD::getFloatBytes(int index) {
	//local variables to read data
	int i = 0;
		
	Wire.beginTransmission(slaveAddress);
	Wire.write(index);
	Wire.endTransmission();

	//PIC float type is 4 bytes, so request 4 bytes
	Wire.requestFrom(slaveAddress, 4);

	while (Wire.available()) {
		rtdFloatValues.b[index-5][i] = Wire.read();
		i++;
	}	

}