int smDirectionPin = 7; //Direction pin
int smStepPin = 6; //Stepper pin
 
void setup(){
  /*Sets all pin to output; the microcontroller will send them(the pins) bits, it will not expect to receive any bits from thiese pins.*/
  pinMode(smDirectionPin, OUTPUT);
  pinMode(smStepPin, OUTPUT);
 
  Serial.begin(9600);
}
 
void loop(){
  /*Here we are calling the rotate function to turn the stepper motor*/
  rotate(800, 0.5); //The motor rotates 800 steps clockwise with a speed of 0.1 (slow)
  delay(5000);
  rotate(1600, 0.5); //The motor rotates 1600 steps clockwise with a speed of 0.5 (medium)
  delay(5000);
  rotate(-1600, 1); //The motor rotates 1600 steps counter clockwise with a speed of 1 (fast)
  delay(5000);
}
 
/*The rotate function turns the stepper motor. Tt accepts two arguments: 'steps' and 'speed'*/
void rotate(int steps, float speed){
  /*This section looks at the 'steps' argument and stores 'HIGH' in the 'direction' variable if */
  /*'steps' contains a positive number and 'LOW' if it contains a negative.*/
  int direction;
 
  if (steps > 0){
    direction = HIGH;
  }else{
    direction = LOW;
  }
 
  speed = 1/speed * 70; //Calculating speed
  steps = abs(steps); //Stores the absolute value of the content in 'steps' back into the 'steps' variable
 
  digitalWrite(smDirectionPin, direction); //Writes the direction (from our if statement above), to the EasyDriver DIR pin
 
  /*Steppin'*/
  for (int i = 0; i < steps; i++){
    digitalWrite(smStepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(smStepPin, LOW);
    delayMicroseconds(speed);
  }
}
