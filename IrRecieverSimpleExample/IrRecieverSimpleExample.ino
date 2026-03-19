// Arduino and KY-022 module
// Example from: https://forum.arduino.cc/t/ir-remote-control-with-ky-022/984621 




   Pin connections (Arduino Uno):

    KY-022
    G-LEFT    Gnd (R1)
    R-MIDDLE  5V
    Y-RIGHT   D2  To Uno

    IR transeiver with keypad


*/
#define ProjectName "IR_remote.ino"
#include <IRremote.h>   // LIBRARY version 3.3

// IR REMOTE
const int IR_RECEIVE_PIN = 2;


void setup() {
  //Serial
  Serial.begin(115200);
  Serial.println("Starting.....");
  Serial.println(F("."));
  Serial.print(F("File   : ")), Serial.println(__FILE__);
  Serial.print(F("Project: ")), Serial.println(ProjectName);
  Serial.print(F("Ready to receive IR signals on pin #"));
  Serial.println(IR_RECEIVE_PIN);

  // IR remote
  IrReceiver.begin(IR_RECEIVE_PIN);
  //IrReceiver.enableIRIn();

  // LED
  pinMode(LED_BUILTIN, OUTPUT);

}// end of setup -----

void loop() {

  if (IrReceiver.decode()) {
    long newIRValue = IrReceiver.decodedIRData.command;
    Serial.print(F("new Value ")); Serial.println(newIRValue);
    //Determine what has been pressed on the remote
    digitalWrite(LED_BUILTIN, HIGH); delay(200);
    Serial.print(F("HEX  ")); Serial.println(newIRValue, HEX);
    digitalWrite(LED_BUILTIN, LOW); delay(200);

    //Prepare to receive the next value
    IrReceiver.resume();

    if (newIRValue == 0xA) {
      Serial.println("received: MODE");
    }
    if (newIRValue == 0x10) {
      Serial.println("received: \"7\"");
    }

  }
}// endof LOOP -----