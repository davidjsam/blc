/*
LANC SNIFFER
Version 1.0
Finds out LANC commands from a REMOTE
For the interface ciruit see 
Feel free to use this code in any way you want.
2011, Martin Koch

"LANC" is a registered trademark of SONY.
CANON calls their LANC compatible port "REMOTE".
*/

#define lancPin 2

int bitDuration = 104; //Duration of one LANC bit in microseconds. 

int lancBit[16];


void setup() {
	Serial.begin(115200); // Opening the serial port
 pinMode(lancPin, INPUT); //listens to the LANC line
        delay(5000); //Wait for camera to power up completly
        bitDuration = bitDuration- 8; //Reading the digital port takes about 8 microseconds so only 96 microseconds are left for each bit
}

void loop() {
  readLancCommand();
  delay(1000);
}

void readLancCommand() {
       
     while (pulseIn(lancPin, HIGH) < 5000) {   
      //"pulseIn, HIGH" catches any 0V TO +5V TRANSITION and waits until the LANC line goes back to 0V 
      //"pulseIn" also returns the pulse duration so we can check if the previous +5V duration was long enough (>5ms) to be the pause before a new 8 byte data packet
      //Loop till pulse duration is >5ms
     }

    //LOW after long pause means the START bit of Byte 0 is here
    delayMicroseconds(bitDuration);  //wait START bit duration
    delayMicroseconds(bitDuration/2); //wait until the middle of bit 0 of byte 0

    //Read the 8 bits of byte 0 
    //Note that the command bits come in in reverse order with the least significant, right-most bit (bit 0) first
    for (int i=7; i>-1; i--) {
      lancBit[i] = digitalRead(lancPin);  //read bits. 
      delayMicroseconds(bitDuration); 
    }
   
    //Byte 0 is read
      
     delayMicroseconds(10); //make sure to be in the stop bit before byte 1
      
      while (digitalRead(lancPin)) { 
        //Loop as long as the LANC line is +5V during the stop bit
      }

      //0V after the previous stop bit means the START bit of Byte 1 is here
      delayMicroseconds(bitDuration);  //wait START bit duration
      delayMicroseconds(bitDuration/2); //wait until the middle of bit 0 of byte 1
      
      //Read the 8 bits of Byte 1
      //Note that the command bits have to be read in reverse order with the least significant, right-most bit (bit 0) first
      for (int i=15; i>7; i--) {
        lancBit[i] = digitalRead(lancPin); //read bits 
 delayMicroseconds(bitDuration);
      }
 
      //Byte 1 is read

   
      //Print byte 0 and byte 1 to the Serial Monitor console of the Arduino developing environment
      Serial.println("BITS: ");
      for (int i=0; i<16; i++) {
        Serial.print((lancBit[i]-1)*-1); //invert the bits
        if (i==7) Serial.print(" ");                        
      }  
      Serial.println("");
}
