#include "WiiLancZoom.h"
#include "Wire.h"
#include "WiiChuck.h" // The library used by the Nunchuck
#include "avr/io.h"
#include "avr/wdt.h"

// Settings for wii pan and tilt
//************************************************************************************************
#define left 11  //Pin 11 controls left pan green
#define right 8 //Pin 8 controls right pan yellow
#define up 10   //Pin 10 controls up tilt black
#define down 9     //Pin 9 controls down tilt blue
//Black is Ground on 2.5 mm goes to GND
//Yellow is tip on 2.5mm and goes to lancPin 2
//Red is circle on 2.5 mm and goes to Vin

WiiChuck chuck = WiiChuck();

int varx = 0;  // The x-axis variable to store the value coming from the Nunchuck
int vary = 0;  // The y-axis variable to store the value coming from the Nunchuck
int absx = 0;  // The x-axis variable used to store the absolute value.
int absy = 0;  // The y-axis variable used to store the absolute value.
//************************************************************************************************

// Settings for wii zoom
//************************************************************************************************

#define lancPin 2
#define aPin 0
#define bitMicroSeconds 104
byte ledPinState = 1;

int commandCode = 0;
int counter=0;
WiiLancZoom lanc =WiiLancZoom();
//************************************************************************************************

void setup()
{
        //resetcleanup();
	Serial.println(" setup");
	Serial.begin(115200); // Opening the serial port
	TCCR2B = TCCR2B & 0b11111000 | 0x07;  //Adusting PWM frequencies for testing pins 11 and 3
	TCCR1B = TCCR1B & 0b11111000 | 0x05;  //Pins 9 and 10
	nunchuck_setpowerpins(); // use analog pins 2 & 3 as gnd & pwr (uncomment to use WiiChuck)
	chuck.begin();
	chuck.update();
	// define pin modes for tx, rx, led pins:
	pinMode(lancPin, INPUT);


}
static void resetcleanup(){
Serial.println(" Reset clean up ");
MCUSR &= ~_BV(WDRF);
WDTCSR |= _BV(WDCE) | _BV(WDE);
WDTCSR = 0;
}
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

void loop()  {
  
	chuck.update();  // The Nunchuck values update in the loop
	// 130 is the distance from 0 on the joystick
        Serial.print("joy x = ");
        Serial.print(chuck.readJoyX());
        Serial.print(",  y = ");
        Serial.print(chuck.readJoyY());
	Serial.print(", ");  
       // varx = 125,  
       // vary = 129, 
       // if (chuck.readJoyX()==125&&chuck.readJoyY()==129){
        //  Serial.println(" Resetting cause something is wrong ");

       //       wdt_enable(WDTO_1S); 
      //  }
	delay(10);
	if (chuck.buttonZ) {
		zoom(true);
	}else if (chuck.buttonC) {
		zoom(false);
	}  
	else  {
		panTilt();

	}
}

void panTilt(){
	varx = chuck.readJoyX();  // nunchuk.analogX is the value of the x-axis
	vary = chuck.readJoyY();  // nunchuk.analogY is the value of the y-axis

	// The values used for speed
	absx = varx;
	absx = abs(absx);  // Convert the x-axis value to an absolute value
	absy = vary;
	absy = abs(absy);
	Serial.println();
	// Tilt based on the input from the joystick
	if (vary > 18)
	{

		analogWrite(up, absy * 2.5);
		digitalWrite(down, LOW);
	}

	else if (vary < -18)
	{

		analogWrite(down, absy * 2.5);
		digitalWrite(up, LOW);
	}

	// Stop tilt
	else
	{
		analogWrite(up, LOW);
		analogWrite(down, LOW);
	}

	// Pan based on the input from the joystick
	if (varx > 10)
	{

		analogWrite(right, absx * 2);
		digitalWrite(left, LOW);

	}

	else if (varx < -10)
	{
		analogWrite(left, absx * 2);
		digitalWrite(right, LOW);

	}

	// Stop pan
	else
	{
		analogWrite(right, LOW);
		analogWrite(left, LOW);
	}
}

void zoom(bool zoom){

	if(zoom){
                Serial.print(" , zoom out  ");
		commandCode=20;
	}else {
                Serial.print(" , zoom in  ");
		commandCode=6;
	}
	//commandCode=speeds[map(analogRead(aPin),0,1023,0,17)]; //The analog input returns from 0 to 1023, but our array is from 0 to 17. This converts the values.
	
	Serial.print(", commandCode= ");
	Serial.println(commandCode);

	if (commandCode == 99) return;  // If the pot is in the middle, send nothing.
	lanc.sendCode(40,commandCode);
	lanc.sendCode(40,commandCode);
	//delay(1000);

}
