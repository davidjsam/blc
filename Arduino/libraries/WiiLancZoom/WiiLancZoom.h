#ifndef WiiLancZoom_h
#define WiiLancZoom_h


#include "Arduino.h"
#include <Wire.h>
#include <math.h>


#define lancPin 2
#define aPin 0
#define bitMicroSeconds 104




class WiiLancZoom {

public:
	void begin() {

	}

	void sendCode(int type,int code) {
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


};


#endif