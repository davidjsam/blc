
/*
*
* This sketch controls the Bescor MP-101 Pan/Tilt system using the Wii Nunchuck
*
* Copyright (c) 2013 Matt Alford, http://www.protechy.com
* Date: July 4, 2013
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
 
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses></http:>.
*  I am not responsible for any damage that may occur from the use of this software.
* Nunchuck Library http://playground.arduino.cc/Main/WiiChuckClass
*
*/
 
#include "Wire.h"
#include "WiiChuck.h" // The library used by the Nunchuck
 
#define left 11  //Pin 11 controls left pan
#define right 8 //Pin 3 controls right pan
#define up 10   //Pin 10 controls up tilt
#define down 9     //Pin 9 controls down tilt
 
WiiChuck chuck = WiiChuck();
 
int varx = 0;  // The x-axis variable to store the value coming from the Nunchuck
int vary = 0;  // The y-axis variable to store the value coming from the Nunchuck
int absx = 0;  // The x-axis variable used to store the absolute value.
int absy = 0;  // The y-axis variable used to store the absolute value.
bool cPressed =false;
bool zPressed=false;
void setup()
{
  Serial.begin(115200); // Opening the serial port
 
  TCCR2B = TCCR2B & 0b11111000 | 0x07;  //Adusting PWM frequencies for testing pins 11 and 3
  TCCR1B = TCCR1B & 0b11111000 | 0x05;  //Pins 9 and 10
 
  nunchuck_setpowerpins(); // use analog pins 2 & 3 as gnd & pwr (uncomment to use WiiChuck)
 
  chuck.begin();
  chuck.update();
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
  if (chuck.buttonZ) {
     Serial.print("Z");
  } else  {
     Serial.print("-");
  }

    Serial.print(", ");  

//not a function//  if (chuck.buttonC()) {
  if (chuck.buttonC) {
     Serial.print("C");
  } else  {
     Serial.print("-");
  }

   Serial.print(chuck.readJoyX());
   Serial.print(", ");  
   Serial.print(chuck.readJoyY());
   Serial.print(", ");  
   Serial.print((int)chuck.readAccelZ());
   Serial.print(", ");  
   Serial.println();
   delay(10);
  // 130 is the distance from 0 on the joystick
  varx = chuck.readJoyX();  // nunchuk.analogX is the value of the x-axis
  vary = chuck.readJoyY();  // nunchuk.analogY is the value of the y-axis
 
  // The values used for speed
  absx = varx;
  absx = abs(absx);  // Convert the x-axis value to an absolute value
  absy = vary;
  absy = abs(absy);
 
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
