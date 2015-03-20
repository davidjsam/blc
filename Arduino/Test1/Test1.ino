int sm_buttonPin = 2;
int bg_buttonPin = 3;
int potPin_0 = A0;
int potPin_2 = A2;
int potPin_3 = A3;
int potPin_4 = A4;
int potPin_5 = A5;
int potPin_1 = A1;

void setup()
{
pinMode(sm_buttonPin, INPUT);
pinMode(bg_buttonPin, INPUT);

Serial.begin(9600);    
delay(100);  // wait for things to stabilize      

}

int small_read = 0;
int big_read = 0;

void loop()
{
      delay(1000);  // wait for things to stabilize      

Serial.print(digitalRead(sm_buttonPin));
Serial.print(" ");
Serial.print(digitalRead(bg_buttonPin));
Serial.print(" A1 ");
Serial.print(analogRead(potPin_1));
Serial.print(" A2 ");
Serial.print(analogRead(potPin_2));
Serial.print(" A3 ");
Serial.print(analogRead(potPin_3));
Serial.print(" A4 ");
Serial.print(analogRead(potPin_4));
Serial.print(" A5 ");
Serial.print(analogRead(potPin_5b));
Serial.print(" A0 ");
Serial.println(analogRead(potPin_0));
}
