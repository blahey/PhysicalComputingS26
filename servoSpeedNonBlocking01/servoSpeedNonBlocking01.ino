#include <Servo.h>
#include <elapsedMillis.h>

Servo cameraServo;  // create servo object to control a servo
const int cameraServoPin = 7;

elapsedMillis cameraMovementTimer;
int cameraMovementInterval = 50;

int cameraPosition = 90;
int cameraMaxPosition = 170;
int cameraMinPosition = 10;
int cameraDirection = 1; // clockwise = 1; counterclockwise = -1


void setup() {
  Serial.begin(9600);
  cameraServo.attach(myservoPin);  // attaches the servo on pin 9 to the servo object

}

void loop() {
  // put your main code here, to run repeatedly:

}

void sweepCamera() {
  if(cameraMovementTimer >= cameraMovementInterval) {
    cameraMovementTimer = 0; // reset the timer

  }

}