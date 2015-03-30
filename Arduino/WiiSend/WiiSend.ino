/* YourDuinoStarter Example: nRF24L01 Transmit Joystick values
 - WHAT IT DOES: Reads Analog values on A0, A1 and transmits
   them over a nRF24L01 Radio Link to another transceiver.
 - SEE the comments after "//" on each line below
 - CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   - 
   Analog Joystick or two 10K potentiometers:
   GND to Arduino GND
   VCC to Arduino +5V
   X Pot to Arduino A0
   Y Pot to Arduino A1
   
 - V1.00 11/26/13
   Based on examples at http://www.bajdi.com/
   Questions: terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[2];  // 2 element array holding Joystick readings
#include "Wire.h"
#include "WiiChuck.h"
WiiChuck chuck = WiiChuck();

void setup()   /****** SETUP: RUNS ONCE ******/
{
 	Serial.println(" setup");
	Serial.begin(115200); // Opening the serial port
	nunchuck_setpowerpins(); // use analog pins 2 & 3 as gnd & pwr (uncomment to use WiiChuck)
	chuck.begin();
	chuck.update();
  radio.begin();
  radio.openWritingPipe(pipe);
}//--(end setup )---
//To power the WiiChuck Adapter
static void nunchuck_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
	DDRC |= _BV(pwrpin) | _BV(gndpin);
	PORTC &=~ _BV(gndpin);
	PORTC |=  _BV(pwrpin);
	delay(100);  // wait for things to stabilize      
}

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
        joystick[0] = chuck.readJoyX();
        joystick[1] = chuck.readJoyY();
  	chuck.update(); 
        Serial.print("joy x = ");
        Serial.print(chuck.readJoyX());
        Serial.print(",  y = ");
        Serial.print(chuck.readJoyY());
	Serial.print(", ");  
        radio.write( joystick, sizeof(joystick) );
        Serial.println("finished writing ");  

delay(10);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********


