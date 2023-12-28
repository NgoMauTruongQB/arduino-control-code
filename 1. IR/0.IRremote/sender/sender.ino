#include <IRremote.h>

#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t NEC_command = 0x1C;
uint32_t noRepeats = 2;

void setup() {
}


void loop() {
  irsend.sendNEC(0x00, NEC_command, noRepeats);
  delay(1000);
}