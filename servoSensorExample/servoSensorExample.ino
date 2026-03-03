/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = A0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
const int myservoPin = 9;
int servoPosition = 90;

void setup() {
  Serial.begin(9600);
  myservo.attach(myservoPin);  // attaches the servo on pin 9 to the servo object
  pinMode(potpin, INPUT);
}

void loop() {
  readLightSensor();
  controlServo();
  printVariableValues();

}

void readLightSensor() {
    val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  }


void controlServo() {
  servoPosition = map(val, 54, 974, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  servoPosition = constrain(servoPosition, 10, 170);
  myservo.write(servoPosition);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}

void printVariableValues() {
  Serial.print("val = ");
  Serial.print(val);
  Serial.print("\t");
  Serial.print("servoPosition = ");
  Serial.println(servoPosition);
}






