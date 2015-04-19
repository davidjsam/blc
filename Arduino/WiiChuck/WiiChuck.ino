#include <math.h>

#include "Wire.h"
#include "WiiChuck.h"
//#include "nunchuck_funcs.h"

#define MAXANGLE 90
#define MINANGLE -90


WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

void setup() {
  TCCR2B = TCCR2B & 0b11111000 | 0x07;  //Adusting PWM frequencies for testing pins 11 and 3
  TCCR1B = TCCR1B & 0b11111000 | 0x05;  //Pins 9 and 10
  nunchuck_setpowerpins();
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  chuck.calibrateJoy();
}
static void nunchuck_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
	DDRC |= _BV(pwrpin) | _BV(gndpin);
	PORTC &=~ _BV(gndpin);
	PORTC |=  _BV(pwrpin);
	delay(100);  // wait for things to stabilize      
}

void loop() {
    delay(10);
    chuck.update();
    Serial.print(chuck.readJoyX());
    Serial.print(", ");  
    Serial.print(chuck.readJoyY());
    Serial.print(", ");  
    Serial.print(chuck.readPitch());
    Serial.print(", ");  
    Serial.print((int)chuck.readAccelX());
    Serial.print(", ");  
    Serial.print((int)chuck.readAccelY());
    Serial.print(", ");  
    Serial.print((int)chuck.readAccelZ());
    Serial.println();
}
