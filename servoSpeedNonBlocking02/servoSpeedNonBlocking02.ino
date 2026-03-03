#include <Servo.h>
#include <elapsedMillis.h>

Servo cameraServo;  // create servo object to control a servo
const int cameraServoPin = 7;

Servo cameraShutterServo;
const int cameraServoShutterPin = 8;

elapsedMillis cameraMovementTimer;
int cameraMovementInterval = 60;

elapsedMillis cameraShutterMoveTimer;
int cameraShutterMoveInterval;

int cameraMaxPosition = 180;
int cameraMinPosition = 0;
int cameraPosition = (cameraMaxPosition + cameraMinPosition) / 2;

int cameraPositionChange = 1; // clockwise = Positve value; counterclockwise = Negative value

int cameraShutterStartPosition = 90;
int cameraShutterEndPosition = 120;

int shutterSwitchPin = 41;
bool shutterSwitchState = 0;
bool shutterServoActive = 0;


void setup() {
  Serial.begin(9600);
  cameraServo.attach(cameraServoPin);  // attaches the servo on pin 9 to the servo object

}

void loop() {
  sweepCamera();
  readSensors();
  cameraOneMove();
}

void sweepCamera() {
  if(cameraMovementTimer >= cameraMovementInterval) {
    cameraMovementTimer = 0; // reset the timer
    if (cameraPosition >= cameraMaxPosition || cameraPosition <= cameraMinPosition) {
      cameraPositionChange = cameraPositionChange * -1;
    }
    cameraPosition = cameraPosition + cameraPositionChange;
    cameraServo.write(cameraPosition);
  }
}

void readSensors() {

}

void cameraOneMove() {
  if (digitalRead)

}