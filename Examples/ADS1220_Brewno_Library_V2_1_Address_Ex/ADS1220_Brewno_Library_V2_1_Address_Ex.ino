//include ADS1220RTD_Brewno and Wire Libraries
#include <ADS1220RTD_Brewno_Library_V2_1.h>
#include <Wire.h>

//The Brewno shield should be the only I2C device attached when running this script.
//have the Brewno shield attached when you upload this script
//After running, you will need to power down the Arduino and shield for the new address
//to take affect

//Default shield address is 0x08, if this has been updated, change to current shield address
int oldAddress = 0x08;

//New address requested by user; do not use an address reserved by I2C protocol for other purposes
//For example, do not use: 0x00, 0x01, 0x02 (google for a list of other reserved addresses)
int newAddress = 0x18;

/*
 * Initialize Instance of ADS1220RTD; don't worry about the normal required values for RTD readings
 * as the following script will not initialize the ADS1220 chip.  The sole purpose of this program is to update
 * the I2C slave address of the Brewno shield.  You do need to input the correct CURRENT I2C slave address
 * in the oldAddress variable above.  If this is a new shield, it will be the default value of 0x08
  */
ADS1220RTD rtdShield(oldAddress,0,0,0,0);

void setup() {
  //Put in some delay to allow power to stabilize
  delay(500);
  
  //Start I2C bus; required for communication with shield
  Wire.begin();
  //Start Serial to receive confirmation script has run
  Serial.begin(9600);

  //use the .changeSlaveAddress function
  rtdShield.changeSlaveAddress(newAddress);
}

void loop() {
  delay(500);
  Serial.print("Program Complete, new I2C slave address set to: ");
  Serial.print(newAddress, HEX);
  Serial.println("HEX");
  Serial.println("Power down Arduino and Shield for new address to take affect.");

  while(1);

  //power off the arduino and shield; the new I2C address will take affect on power up.
  //be sure to note your new slave address somewhere and update your programs as needed.

}
