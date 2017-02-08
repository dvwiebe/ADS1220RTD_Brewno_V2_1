//include ADS1220RTD_Brewno and Wire Libraries
#include <ADS1220RTD_Brewno_Library_V2_1.h>
#include <Wire.h>

/*
 * Initialize Instance of ADS1220RTD; this will cover all 4 RTD inputs for one shield
 * Below is a brief descirption of parameters; see library readme file for full details.
 * ADS1220RTD "someName"(shield address, Gain, SPS, a parameter, b parameter)
 *    shield address - i2c slave address, shield default is 0x08
 *    Gain - ADS1220 Gain Setting, 32 a typical setting
 *    SPS - ADS1220 Samples per Second setting, settings of 20, 90 and 180 typical
 *    a - a parameter for Callendar Van Dusen Formula; check your RTD. Most will be 0.0039083
 *    b - b parameter for Callendar Van Dusen Formula; check your RTD. Most will be -0.0000005775
 */

ADS1220RTD rtdShield(0x08, 32, 20, 0.0039083, -0.0000005775);

//Create variables as needed to record you
float resistance_1;
float resistance_2;
float temperatureF_1;
float temperatureF_2;

void setup() {
  //Put in some delay to allow power to stabilize
  delay(3000);
  
  //Start I2C bus; required for communication with shield
  Wire.begin();
  //Start Serial if required by user
  Serial.begin(9600);
  
  //Initialize RTD shield; only needs to be called once
  rtdShield.rtdInitialize(); 
}

void loop() {
  

  /*
   * User may call functions as needed; Bytes, Resistance or Temperature in C of F:
   *  ie. 
   *      rtdShield.getResistance(rtd);   //returns a long int
   *      rtdShield.getTemperatureC(rtd); //returns a float
   *      rtdShield.getTemperatureF(rtd); //returns a float
   *        where rtd = 0, 1, 2 or 3 for corresponding RTDS 1 thru 4 on the shield
   *        note that for the .getBytes and .getResistance call you can request 
   *        with an rtd ID of 4.  This will return the internal ADS1220 offset value. 
   *        The offset is available for information only.
   *              
   * Bytes is the raw information from the ADS1220 (bit shifted to 32 bit format) and 
   * should not typically be required.   
   */
  
  resistance_1 = rtdShield.getResistance(0);
  resistance_2 = rtdShield.getResistance(1);
  temperatureF_1 = rtdShield.getTemperatureF(0);
  temperatureF_2 = rtdShield.getTemperatureF(1);

  //due to some offset present in board circuit(primarily the switching IC) and any user installed
  //wiring, make sure to measure and add in offset for each RTD for the most accurate readings;
  //user only required to add offsets to values you will be using in your program,
  //see pseudo code further below  

  //Do something with readings
  Serial.print("RTD #1 Resistance: ");
  Serial.println(resistance_1, 1);  
  Serial.print("RTD #2 Resistance: ");
  Serial.println(resistance_2, 1);
  delay(1000);
  Serial.print("RTD #1 Temperature (F): ");
  Serial.println(temperatureF_1, 1);    
  Serial.print("RTD #2 Temperature (F): ");
  Serial.println(temperatureF_2, 1);  

}

//Optional: pseudo code provided for demonstration only
/*
 * void serialEvent(){
 *  Read serial event
 *  If instruction to recalibrate internal offset
 *    rtdShield.offsetCalibration();
 *  If instuction to reset shield
 *    rtdShield.reset(); //should not be needed; if user resets they will have to initialize the shield again
 *  If instruction to add offset to an RTD
 *    temperatureF_1 += user specified offset temperature
 * }
 *    
 */

   

