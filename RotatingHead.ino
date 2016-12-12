#include <Servo.h>

Servo myservo;
int lineVal;
int pos;
int lineSensorPin = 0;
int servoPin = 12;
int ledPin =13;

void setup()
{
myservo.attach(servoPin);
myservo.write(0); // initial position
pinMode(lineSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
Serial.begin(9600);
}
void loop()
{
lineVal = analogRead(lineSensorPin);
Serial.println(lineVal);

if (lineVal>300) {

 for (pos = 90; pos <= 110; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 110; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
    digitalWrite(ledPin,HIGH); //turn relay ON
  }
}
  else{ 
    digitalWrite(ledPin,LOW); //turn relay OFF}

} // end of if

}