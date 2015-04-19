#include <Servo.h>
#include <math.h>
#include "Wire.h"
#include "WiiChuck.h"

Servo panServo;      
Servo tiltServo;      

int servoPanPosition = 90;
int servoTiltPosition = 90;
//int joystickPanPin = A4;
//int joystickTiltPin = A5;
int joystickPanSpeed = 0;
int joystickTiltSpeed = 0;
int servoPanPin = 9;
int servoTiltPin = 10;
WiiChuck chuck = WiiChuck();

void setup()
{
  pinMode(servoPanPin, OUTPUT);
  pinMode(servoTiltPin, OUTPUT);
  panServo.attach(servoPanPin);
  tiltServo.attach(servoTiltPin);
  nunchuck_setpowerpins();
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
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

void loop()
{
   panTiltUsingWii();
  // testServo();
}
void panTiltUsingWii(){
    delay(10);
    chuck.update();
    int varx =chuck.readJoyX();
    int vary =chuck.readJoyY();
    Serial.print(varx);
    Serial.print(", ");  
    Serial.print(vary);
    Serial.println(", "); 
  joystickPanSpeed = (vary) / 50;
    // -512 to provide equal +/- numbers
  joystickTiltSpeed = (varx) / -50;
    // negative 50 to reverse direction
  servoPanPosition = constrain((servoPanPosition + joystickPanSpeed), 1, 180);
  servoTiltPosition = constrain((servoTiltPosition + joystickTiltSpeed), 1, 180);
    // constarin function to not exceed servo limits

  panServo.write(servoPanPosition);
  tiltServo.write(servoTiltPosition);

  delay(10);
    // adjustable for overall speed
}
void testServo(){
   panServo.write(45);  // Turn Servo Left to 45 degrees
   delay(1000);          // Wait 1 second
   panServo.write(0);   // Turn Servo Left to 0 degrees
   delay(1000);          // Wait 1 second
   panServo.write(90);  // Turn Servo back to center position (90 degrees)
   delay(1000);          // Wait 1 second
   panServo.write(135); // Turn Servo Right to 135 degrees
   delay(1000);          // Wait 1 second
   panServo.write(180); // Turn Servo Right to 180 degrees
   delay(1000);          // Wait 1 second
   panServo.write(90);  // Turn Servo back to center position (90 degrees)
   delay(1000);   
   Serial.println("pan is done");
   tiltServo.write(45);  // Turn Servo Left to 45 degrees
   delay(1000);          // Wait 1 second
   tiltServo.write(0);   // Turn Servo Left to 0 degrees
   delay(1000);          // Wait 1 second
   tiltServo.write(90);  // Turn Servo back to center position (90 degrees)
   delay(1000);          // Wait 1 second
   tiltServo.write(135); // Turn Servo Right to 135 degrees
   delay(1000);          // Wait 1 second
   tiltServo.write(180); // Turn Servo Right to 180 degrees
   delay(1000);          // Wait 1 second
   tiltServo.write(90);  // Turn Servo back to center position (90 degrees)
   delay(1000);  
   Serial.println("tilt is done");   
}
