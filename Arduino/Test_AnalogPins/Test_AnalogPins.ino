int ledPin = 13;   
int potPin_0 = A0;
int potPin_2 = A2;
int potPin_3 = A3;
int potPin_4 = A4;
int potPin_5 = A5;
int potPin_1 = A1;

void setup()
{
pinMode(ledPin, OUTPUT);

Serial.begin(115200);    
delay(100);  // wait for things to stabilize      

}



void loop()
{
      delay(1000);  // wait for things to stabilize      


Serial.print(" A1 ");
Serial.print(analogRead(potPin_1));
Serial.print(" A2 ");
Serial.print(analogRead(potPin_2));
Serial.print(" A3 ");
Serial.print(analogRead(potPin_3));
Serial.print(" A4 ");
Serial.print(analogRead(potPin_4));
Serial.print(" A5 ");
Serial.print(analogRead(potPin_5));
Serial.print(" A0 ");
Serial.println(analogRead(potPin_0));
}
