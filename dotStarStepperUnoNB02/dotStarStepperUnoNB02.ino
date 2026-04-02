/*
 * Author: Byron Lahey
 * This code serves as an example of an integration of a DotStar (APA 102) LED strip 
 * and a stepper motor (using an A4988 stepper motor driver board).
 * 
 * This code builds on standard example code for each of the independent components. 
 * 
 * V01: 9/7/2021 -initial draft
 * V02: 9/8/2021 - added comments, cleaned up unused code
 * V03: 9/8/2021 -added randStepper() function. This is a "non-blocking" function, 
 *                allowing the LED animation and stepper movements to operate at
 *                the same time.
 * V04: Date???  -changes not recorded
 * V05: 9/19/2022 - Added setup for 28BYJ-48 Stepper Motor with ULN2003 Driver
 *                - Reference for hardware: https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/
 *                - Reference for SPI on Teensy: https://www.pjrc.com/teensy/td_libs_SPI.html

 * dotStarStepperMega04: 3/20/2023
                  - updated example functions to emphasize non-blocking variations
                  - implemented non-blocking replacement for parseInt() on non-blocking versions of functions using serial input

According to the data sheet, when the 28BYJ-48 motor is operated in full-step mode, each step corresponds to a rotation of 11.25°. This means there are 32 steps per revolution (360°/11.25° = 32).
In addition, the gearbox inside the motor has a 64:1 gear reduction. This results in 2048 (32*64) steps per revolution.
 */


//LED Configuration

#include "FastLED.h"           // Library for efficient LED control
#define DATA_PIN 11            // MOSI (Serial Peripheral Interface (SPI) Output pin)
#define CLOCK_PIN 13           // SCK (Serial Peripheral Interface (SPI) Clock pin)
#define LED_TYPE DOTSTAR       //APA 102-2020
//#define LED_TYPE NEOPIXEL       //ws2812

#define COLOR_ORDER BGR        // This order can vary between LED types
const int NUM_LEDS = 16;       // Number of LEDs you are communicating with
CRGB leds[NUM_LEDS];           // Define the array of LEDs
#define BRIGHTNESS 100         //max 255 - \
                               // keeping brightness lower will make it reasonable to power \
                               // directly off of the USB power and can produce richer colors
#define FRAMES_PER_SECOND 120  // used for some FastLED animation/timing

// End LED Configuration


// Stepper Configuration

#include <AccelStepper.h>
AccelStepper myStepper(4, 8, 10, 9, 7);  // define motor pins (20, 22, 21, 23) and interface mode (4)


int steps = 0;
int currentSteps = 0;
int previousSteps = 0;
// End Stepper Configuration


// Timing Configuration
#include <elapsedMillis.h>
elapsedMillis myTimer01;
unsigned int timesUp01 = 100;
signed int pos = 0;        // used to keep track of the location of an LED
signed int posChange = 1;  // used to shift the position of the LED

// Alternative to parseInt
// From: https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
// Example 4 - Receive a number as text and convert it to an int

const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data

boolean newData = false;

int dataNumber = 0;  // new for this version
// end Alternative to parseInt



void setup() {
  Serial.begin(9600);
  //while(!Serial);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(12)>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  myStepper.setMaxSpeed(800);      // sets the maximum steps per second, which determines how fast the motor will turn
  myStepper.setAcceleration(100);  // sets the acceleration rate in steps per second
  myStepper.setSpeed(50);

}

void loop() {
  //simpleFillAll();
  //rgbColor();
  simpleLedMotion();
  //ledMotion01();
  //basicScript();    // this function uses blocking code - don't use with LEDs (for reference only)
  //randomMotion01(); // this function uses blocking code - don't use with LEDs (for reference only)
  randStepper();
  //angleInput();     // this function uses blocking code - don't use with LEDs (for reference only)
  //angleInputNB();
  //stepsInput();       // this function uses blocking code - don't use with LEDs (for reference only)
  //stepsInputNB();
}

void simpleFillAll() {  // This picks a random hue, assigns it to each LED and updates the display.
  int hue = random(256);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
}

void rgbColor() {  // This picks a random hue, assigns it to each LED and updates the display.
  
    leds[0] = CRGB(0, 0, 255);
  
  FastLED.show();
}

void simpleLedMotion() {
  if (myTimer01 > timesUp01) {
    myTimer01 = 0;
    leds[pos] = CHSV(255, 255, 255);
    FastLED.show();
    leds[pos] = CHSV(0, 0, 0);  // set the current LED to black, but don't update the display yet
    pos++;
    if (pos >= (NUM_LEDS)) {
      pos = 0;
    }
  }
}

void ledMotion01() {
  pos = constrain(pos, 0, 15);  // protect against sending a value outside of the scope of the array
  if (myTimer01 > timesUp01) {
    myTimer01 = 0;
    leds[pos] = CHSV(255, 255, 255);
    FastLED.show();
    //leds[pos] = CHSV(0, 0, 0);  // set the current LED to black, but don't update the display yet
    fadeToBlackBy(leds, NUM_LEDS, 100);
    Serial.print(pos);
    Serial.print("\t");
    Serial.println(posChange);
    pos += posChange;
    if ((pos >= (NUM_LEDS - 1)) || (pos <= 0)) {
      posChange = posChange * -1;
    }
  }
}

void basicScript() {
  myStepper.moveTo(5000);
  myStepper.runToPosition();
  myStepper.moveTo(-5000);
  myStepper.runToPosition();
}

void randomMotion01() {
  myStepper.moveTo(random(4000));
  myStepper.runToPosition();
}

void randStepper() {
  if (myStepper.distanceToGo() == 0) {
    // Random change to speed, position and acceleration
    // Make sure we dont get 0 speed or accelerations
    //delay(1000);
    myStepper.moveTo(rand() % 4000);
    myStepper.setMaxSpeed((rand() % 400) + 1);
    myStepper.setAcceleration((rand() % 200) + 1);
  }
  myStepper.run();
}

void angleInput() {
  int angle;
  if (Serial.available() > 0) {
    angle = Serial.parseInt();
  }
  if (Serial.read() == 10) {              // newline character
    angle = map(angle, 0, 360, 0, 2038);  // Set the output range to match the steps/revolution of the motor.
    myStepper.moveTo(angle);
    myStepper.runToPosition();
  }
  delay(1);
}

void angleInputNB() {  //non-blocking version of angle input
  steps = recvWithEndMarker(); // I'm reusing the steps, currentSteps and previousSteps variables rather than duplicating their function with "angle" versions. 
  currentSteps = steps;
  if (currentSteps != previousSteps) {
    int angle = steps;
    Serial.print("angle = ");
    Serial.println(angle);
    angle = map(angle, 0, 360, 0, 2038);  // Set the output range to match the steps/revolution of the motor.
    myStepper.moveTo(angle);
    previousSteps = currentSteps;
  }
  myStepper.run();
}

void stepsInput() {
  int steps;
  if (Serial.available() > 0) {
    steps = Serial.parseInt();
    Serial.print("steps = ");
    Serial.println(steps);
  }
  if (Serial.read() == 10) {  // newline character
    Serial.println("stepping");
    myStepper.moveTo(steps);
    myStepper.runToPosition();
  }
  delay(1);
}

void stepsInputNB() {
  steps = recvWithEndMarker();
  currentSteps = steps;
  if (currentSteps != previousSteps) {
    Serial.print("steps = ");
    Serial.println(steps);
    myStepper.moveTo(steps);
    previousSteps = currentSteps;
  }
  myStepper.run();
}


int recvWithEndMarker() {
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
      receivedChars[ndx] = '\0';  // terminate the string
      ndx = 0;
      newData = true;
      int myNumber = atoi(receivedChars);
      return myNumber;
    }
  }
}
