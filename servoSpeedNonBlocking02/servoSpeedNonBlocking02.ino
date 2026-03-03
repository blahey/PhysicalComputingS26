#include <Servo.h>
#include <elapsedMillis.h>

Servo cameraServo;  // create servo object to control a servo
const int cameraServoPin = 7;

Servo cameraShutterServo;
const int cameraServoShutterPin = 8;

elapsedMillis cameraMovementTimer;
int cameraMovementInterval = 60;

elapsedMillis cameraShutterMoveTimer;
int cameraShutterMoveInterval = 10;

int cameraMaxPosition = 180;
int cameraMinPosition = 0;
int cameraPosition = (cameraMaxPosition + cameraMinPosition) / 2;

int cameraPositionChange = 1;  // clockwise = Positve value; counterclockwise = Negative value

int cameraShutterStartPosition = 10;
int cameraShutterEndPosition = 120;
int cameraShutterPosition = cameraShutterStartPosition;

int shutterSwitchPin = 41;
bool shutterSwitchState = 0;
bool shutterServoActive = 0;


void setup() {
  Serial.begin(9600);
  cameraServo.attach(cameraServoPin);  // attaches the servo on pin 9 to the servo object
  cameraShutterServo.attach(cameraServoShutterPin);
  pinMode(shutterSwitchPin, INPUT_PULLUP);
}

void loop() {
  sweepCamera();
  readSensors();
  cameraOneMove();
}

void sweepCamera() {
  if (cameraMovementTimer >= cameraMovementInterval) {
    cameraMovementTimer = 0;  // reset the timer
    if (cameraPosition >= cameraMaxPosition || cameraPosition <= cameraMinPosition) {
      cameraPositionChange = cameraPositionChange * -1;
    }
    cameraPosition = cameraPosition + cameraPositionChange;
    cameraServo.write(cameraPosition);
  }
}

void readSensors() {
  shutterSwitchState = digitalRead(shutterSwitchPin);
  // Serial.println(shutterSwitchState);
}

void cameraOneMove() {
  if (shutterSwitchState == 0 && shutterServoActive == 0) {
    Serial.println("start move");
    cameraShutterPosition = cameraShutterPosition + 1;
    cameraShutterServo.write(cameraShutterPosition);
    shutterServoActive = 1;
  }
  if (shutterServoActive && cameraShutterPosition < cameraShutterEndPosition) {
    // Serial.println("second if logic satisfied");
    if (cameraShutterMoveTimer >= cameraShutterMoveInterval) {
      // Serial.println("third if logic satisfied");

      cameraShutterMoveTimer = 0;  // reset the timer
      cameraShutterPosition = cameraShutterPosition + 1;
      cameraShutterServo.write(cameraShutterPosition);
    }
  } else if (cameraShutterPosition >= cameraShutterEndPosition) {
    cameraShutterPosition = cameraShutterStartPosition;
    cameraShutterServo.write(cameraShutterPosition);
    shutterServoActive = 0;
    Serial.println("end move");
  }
}