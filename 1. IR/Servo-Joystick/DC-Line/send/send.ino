#include <IRremote.h>

const int LINE_PIN = 3;

#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t NEC_command;
uint32_t noRepeats = 2;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int line = digitalRead(LINE_PIN);
  Serial.println(line);
  if (line == LOW) {
    NEC_command = 0x1C;
  } else {
    NEC_command = 0x1D;
  }
  
  irsend.sendNEC(0x00, NEC_command, noRepeats);
  delay(1500);
}