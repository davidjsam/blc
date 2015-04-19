/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "avr/io.h"
#include "avr/wdt.h"
#include "WiiLancZoom.h"

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10


// Settings for wii pan and tilt
//************************************************************************************************
#define left 7  //Pin 11 controls left pan brown
#define up 6   //Pin 10 controls up tilt red
#define down 5     //Pin 9 controls down tilt orange
#define right 4 //Pin 8 controls right pan yellow
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
int joystick[4];  // 2 element array holding Joystick readings
#define lancPin 2
#define aPin 0
#define bitMicroSeconds 104
byte ledPinState = 1;
int commandCode = 0;
int absx = 0;  // The x-axis variable used to store the absolute value.
int absy = 0;  // The x-axis variable used to store the absolute value.

WiiLancZoom lanc =WiiLancZoom();
//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //
  byte sz=sizeof(joystick);
  Serial.begin(115200); // Opening the serial port
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  wdt_enable(WDTO_2S);	
  pinMode(lancPin, INPUT);
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(sz);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)


    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);


  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop(void)
{

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        //done = radio.read( &got_time, sizeof(unsigned long) );
        done = radio.read( joystick, sizeof(joystick) );
        // Spew it
        printf("Got payload..");
        whatsinjoystick(joystick);
        if (joystick[2]==1) {
		zoom(true);
	}else if (joystick[3]==1) {
		zoom(false);
	}  
	else  {
		panTilt(joystick[0],joystick[1]);

	}
   
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }
      delay(10);

wdt_reset();
}

/*-----( Declare User-written Functions )-----*/
void panTilt(int varx,int vary){

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
void whatsinjoystick(int joystick[4]){
       Serial.print("joy x = ");
        Serial.print(joystick[0]);
        Serial.print(",  y = ");
        Serial.print(joystick[1]);
	Serial.print(", "); 
        Serial.print(",  Z = ");
        Serial.print(joystick[2]);
	Serial.print(", ");  
        Serial.print(",  C = ");
        Serial.print(joystick[3]);
	Serial.print(", ");  
}

