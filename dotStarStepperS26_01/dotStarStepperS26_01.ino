

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
 */

#include <elapsedMillis.h>

//LED Configuration


#include "FastLED.h"            // Library for efficient LED control
#define DATA_PIN    11          // MOSI (Serial Peripheral Interface (SPI) Output pin) was 11
#define CLOCK_PIN   13          // SCK (Serial Peripheral Interface (SPI) Clock pin)
#define LED_TYPE    DOTSTAR     //APA 102-2020
#define COLOR_ORDER BGR         // This order can vary between LED types 
const int NUM_LEDS  =  16;         // Number of LEDs you are communicating with
CRGB leds[NUM_LEDS];            // Define the array of LEDs
#define BRIGHTNESS  100         //max 255 - 
                                // keeping brightness lower will make it reasonable to power 
                                // directly off of the USB power and can produce richer colors
#define FRAMES_PER_SECOND  120  // used for some FastLED animation/timing

// End LED Configuration


// Stepper Configuration

#include <AccelStepper.h>
//AccelStepper myStepper(1, 15, 14);  // 1 sets the MotorInterfaceType to DRIVER; pin 15 = step; pin 14 = direction
AccelStepper myStepper(4, 8, 10, 9, 7); // define motor pins (8, 10, 9, 11) and interface mode (4) ***ORDER OF THESE PINS MATTERS!***

// const int ms1Pin = 20;          // define pins for stepping mode
// const int ms2Pin = 19;          // stepping modes change the step resolution of the motor
// const int ms3Pin = 18;          // higher resolution comes at the expense of higher speeds and torque
// const int enablePin = 21;
// End Stepper Configuration


// Timing Configuration

elapsedMillis myTimer01;
unsigned int timesUp01 = 100;
signed int pos = 0; // used to keep track of the location of an LED
signed int posChange = 1 ; // used to shift the position of the LED

void setup() {
  Serial.begin(9600);
  while(!Serial);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(12)>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  myStepper.setMaxSpeed(800); // sets the maximum steps per second, which determines how fast the motor will turn
  myStepper.setAcceleration(500); // sets the acceleration rate in steps per second
  myStepper.setSpeed(100);
  
  //pinMode(resetPin, OUTPUT);
  //pinMode(sleepPin, OUTPUT);
  // pinMode(ms1Pin, OUTPUT);        // set step mode pins as outputs
  // pinMode(ms2Pin, OUTPUT);
  // pinMode(ms3Pin, OUTPUT);
  // pinMode(enablePin, OUTPUT);
  

  // digitalWrite(ms1Pin, HIGH);     // full step (LOW, LOW, LOW)
  // digitalWrite(ms2Pin, LOW);      // runs smoother in half step mode (HIGH, LOW, LOW) but might need to increase current for required torque
  // digitalWrite(ms3Pin, LOW);
  // digitalWrite(resetPin, HIGH);
  // digitalWrite(sleepPin, HIGH);
  // digitalWrite(enablePin, LOW);
}

void loop() {  
  //simpleFillAll();
  simpleLedMotion();
  ledMotion01();
  //basicScript();
  //randomMotion01();
  //randStepper();
  //angleInput();
  //stepsInput();
}

void simpleFillAll() {    // This picks a random hue, assigns it to each LED and updates the display.
  int hue = random(256);
  for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue, 255, 255);
        }
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
  pos = constrain(pos, 0, 15); // protect against sending a value outside of the scope of the array
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
    if ((pos >= (NUM_LEDS - 1) ) || (pos <= 0)) {
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

void randStepper()
{
    if (myStepper.distanceToGo() == 0)
    {
  // Random change to speed, position and acceleration
  // Make sure we dont get 0 speed or accelerations
  //delay(1000);
  myStepper.moveTo(rand() % 4000);
  myStepper.setMaxSpeed((rand() % 800) + 1);
  myStepper.setAcceleration((rand() % 500) + 1);
    }
    myStepper.run();
}

void angleInput() {
  int angle;
  if(Serial.available() > 0) {
    angle = Serial.parseInt();
    
  }
  if(Serial.read() == 10) { // newline character
    angle = map(angle, 0, 360, 0, 400); // Set the output range to match the steps/revolution of the motor.
    myStepper.moveTo(angle);
    myStepper.runToPosition();
  }
  delay(1);
}

void stepsInput() { 
  int steps;
  if(Serial.available() > 0) {
    steps = Serial.parseInt();
    
  }
  if(Serial.read() == 10) { // newline character
    myStepper.moveTo(steps);
    myStepper.runToPosition();
  }
  delay(1);
}
