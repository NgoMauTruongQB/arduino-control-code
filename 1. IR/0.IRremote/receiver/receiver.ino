#include <IRremote.h>


#define IR_RECEIVE_PIN 2
IRrecv irrecv(IR_RECEIVE_PIN);
uint32_t IR_receive_value;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode()) {
    IR_receive_value = irrecv.decodedIRData.decodedRawData;
    Serial.print("protocol: ");
    Serial.println(irrecv.decodedIRData.protocol);
    Serial.print("decodedRawData: ");
    Serial.println(irrecv.decodedIRData.decodedRawData);
    Serial.print("command: ");
    Serial.println(irrecv.decodedIRData.command);
    Serial.println();
    if (irrecv.decodedIRData.command == 0x1C) {
      // do something
    }
    else if (irrecv.decodedIRData.command == 0x1D) {
      // do something else
    }
    irrecv.resume();
  }
}