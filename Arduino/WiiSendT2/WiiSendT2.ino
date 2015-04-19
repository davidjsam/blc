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
#include "Wire.h"
#include "WiiChuck.h" // The library used by the Nunchuck
#include "avr/io.h"
#include "avr/wdt.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);
WiiChuck chuck = WiiChuck();

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xE8E8F0F0E1LL, 0xE8E8F0F0D2LL };

int joystick[4];  // 2 element array holding Joystick readings
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
role_e role = role_ping_out;

void setup(void)
{
  //
  // Print preamble
  //
          wdt_enable(WDTO_2S);	

  	Serial.begin(115200); // Opening the serial port
          nunchuck_setpowerpins(); // use analog pins 2 & 3 as gnd & pwr (uncomment to use WiiChuck)
  chuck.begin();
  chuck.update();
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)


    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  

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
  // Ping out role.  Repeatedly send the current time
  //
      chuck.update();  // The Nunchuck values update in the loop
      int varx=chuck.readJoyX();
      int vary = chuck.readJoyY();
      joystick[0] = varx;
      joystick[1] = vary;
      joystick[2]= chuck.buttonZ;
      joystick[3]= chuck.buttonC;
      whatsinjoystick(joystick);
      if((varx>120)&&(vary>120)){Serial.println("Need a reset  "); wdt_enable(WDTO_15MS); delay(500); return ;}
      if((varx<-120)&&(vary<-120)){Serial.println("Need a reset  "); wdt_enable(WDTO_15MS); delay(500); return ;}
   
  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();
 
    bool ok = radio.write( joystick, sizeof(joystick) );

    if (ok)
      printf("ok...");
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Try again 1s later
   
  }

   delay(10);
wdt_reset();
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
