// Stepper Configuration (Arduino Uno wiring)
// Motor: 28BYJ-48 Stepper Motor  
// Driver: ULN2003 Driver
//
// Pin map aligned to the Uno-form-factor circuit layout:
// D8 -> IN1
// D9 -> IN2
// D10 -> IN3
// D11 -> IN4
// 5V -> + power input
// GND -> - power input
//


#include <AccelStepper.h>


AccelStepper myStepper(4, 8, 10, 9, 11); // define motor pins (8, 10, 9, 11) and interface mode (4) ***ORDER OF THESE PINS MATTERS!***

const int stepsPerRevolution = 2048;  


int steps = 0;

// Uno only has A0-A5; use A0 if you enable variableStepper().
const int potPin = A0;

// Alternative to parseInt
// From: https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
// Example 4 - Receive a number as text and convert it to an int
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
int dataNumber = 0;

void setup() {
  Serial.begin(9600);

  myStepper.setMaxSpeed(4000);     // steps per second
  myStepper.setAcceleration(500);  // steps per second^2
  myStepper.setSpeed(100);

}

void loop() {
  //randStepper();
  variableStepper();
  // angleInputNB();
  // stepsInputNB();
}

void randStepper() {
  if (myStepper.distanceToGo() == 0) {
    // Random target position, speed, and acceleration.
    myStepper.moveTo(rand() % 8000);
    myStepper.setMaxSpeed((rand() % 2000) + 50);
    myStepper.setAcceleration((rand() % 1000) + 1);
  }
  myStepper.run();
}

void variableStepper() {
  int potVal = analogRead(potPin);
  float speed = map(potVal, 1023, 10, -800, 800);
  speed = constrain(speed, -800, 800);
  myStepper.setSpeed(speed);
  myStepper.runSpeed();
}

void angleInputNB() {  // non-blocking angle input
  recvWithEndMarker();

  if (newData) {
    int angle = dataNumber;
    Serial.print("angle = ");
    Serial.println(angle);

    angle = map(angle, 0, 360, 0, stepsPerRevolution);
    myStepper.moveTo(angle);
    newData = false;
  }
  myStepper.run();
}

void stepsInputNB() {  // non-blocking raw step input
  recvWithEndMarker();

  if (newData) {
    steps = dataNumber;
    Serial.print("steps = ");
    Serial.println(steps);
    myStepper.moveTo(steps);
    newData = false;
  }
  myStepper.run();
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';
      ndx = 0;
      newData = true;
      dataNumber = atoi(receivedChars);
    }
  }
}
