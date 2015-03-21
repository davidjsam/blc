/* 
 * Arduino LANC zoom controller - AKA Zoomduino
 * By Angus Findlay
 * Using code from Brady Marks of Furious Green Lab
 * Connections:
 * LANC 2.5mm plug sleeve to Arduino Ground
 * LANC 2.5mm plug  ring  to Arduino Vin (input to regulator, NOT 5V)
 * LANC 2.5mm plug   tip  to Arduino Digital Pin 2
 * Potentiometer   wiper  to Arduino Analog Pin 0
 * Potentiometer   outer  to Arduino 5V and Ground, switch polarity to reverse zoom direction. 
 *                        This provides a voltage divider to determine which code to send the camera.
 *
 * This code has only been tested with a Canon XH-A1, and is provided entirely without warranty or guarantee.
 * I hope it's useful for people, but if it isn't, I won't be held responsible.
 * Released under the GPL.
 * 
 * Angus Findlay
 * angus@(domain on the next line)
 * www.findlayproductions.ca
 * December 28, 2008
 *
 */

/*
 * Copyright (C) 2006 Free Software Foundation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * See file LICENSE for further informations on licensing terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * @author: Brady Marks <brady@furiousgreencloud.com>
 *   documentation of LANC from http://www.boehmel.de/lanc.htm
 * @date: 2008-05-22
 * @locations: Furious Green Lab, Vancouver, British Columbia, Canada 
 */
int speeds[18]= /*These are the commands sent to the camera. 99 tells the code to send nothing.*/
{
  0x0e,0x0c,0x0a,0x08,0x06,0x04,0x02,0x00, /*Zoom tele, from fastest to slowest*/
  99,99,99, /*Dead zone*/
  0x10,0x12,0x14,0x16,0x18,0x1a,0x1e}; /*Zoom wide, from slowest to fastest*/

#define lancPin 2
#define aPin 0
int potPin_0 = A0;
#define bitMicroSeconds 104

byte ledPinState = 1;

void setup() {
    Serial.begin(115200); // Opening the serial port

  Serial.println(" setup");
  // define pin modes for tx, rx, led pins:
  pinMode(lancPin, INPUT);
  pinMode(aPin,INPUT);

}

int commandCode = 0;
int counter=0;
void loop() {
  
  
  commandCode=speeds[map(analogRead(aPin),0,1023,0,17)]; //The analog input returns from 0 to 1023, but our array is from 0 to 17. This converts the values.
  Serial.print(counter);
  Serial.print(" A0 --> ");
  Serial.print(analogRead(potPin_0));
      Serial.print(" CommandCode  --> ");
          Serial.print(commandCode);
   Serial.print(" pin 2 --> ");
  Serial.println(digitalRead(lancPin));
  
 if (commandCode == 99) return;  // If the pot is in the middle, send nothing.
 SendCode(40,commandCode);
  SendCode(40,commandCode);
  //delay(1000);
  counter++;
}


void SendCode(int type,int code) {
  frameStartBitWait();
  writeByte(lancPin,type,bitMicroSeconds);    // Video Cam Control
  lowWait(lancPin);
  writeByte(lancPin,code,bitMicroSeconds);  // Tele/Wide etc
}

void lowWait(int pin) {
  byte commandCode = digitalRead(pin);
  while (commandCode) {
    commandCode = digitalRead(pin);
  }
}

void frameStartBitWait() {
  // finds thge start of a telegram/frame
  unsigned long usec = pulseIn(lancPin,HIGH);
  while (usec < 5864) { // 6230 experimentally with a RTV900
    usec = pulseIn(lancPin,HIGH);
    // DEBUG
    // Serial.print(usec);
    // Serial.println(" microseconds");
     //
  }

  //
  // Serial.print("frame start after ");
   //Serial.print(usec);
   //Serial.println(" microseconds");
   //
}

byte readByte(int pin,unsigned long uSec /* bit width*/ ) {
  byte result = 0;
  delayMicroseconds(uSec * 1.5); // skips the Start Bit and Land in the midlle of the first byte

  for (int i = 0; i < 8; i++) {
    if (digitalRead(pin) == LOW) { // == *LOW* because bits inverted in LANC 
      result++;
    }
    result <<= 1;
    delayMicroseconds(uSec);
  }
  delayMicroseconds(0.5*uSec);  
  return result; // return happens at end of last (8ths) bit
}

void writeByte(int pin, byte value, unsigned uSec /* bit width */) {
  delayMicroseconds(uSec); // wait for stop bit
  pinMode(pin,OUTPUT);
  for (int i = 0; i < 8; i++) {
    boolean bit = value & 0x1;
    digitalWrite(pin,!bit); // NOT (!) pin because all data is inverted in LANC
    value >>= 1;
    delayMicroseconds(uSec);
  }
  pinMode(pin,INPUT); // return happends at end of last (8th) bit
}


