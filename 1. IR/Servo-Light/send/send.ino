#include <IRremote.h>

const int LIGHT_PIN = A0;
int darkness;

#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t NEC_command;
uint32_t noRepeats = 2;

void setup() {
  Serial.begin(115200);
}
void loop() {
  darkness = analogRead(LIGHT_PIN);
  Serial.println(darkness);
  if (darkness >= 650) {
    NEC_command = 0x1C;
  } else {
    NEC_command = 0x1D;
  }
  
  irsend.sendNEC(0x00, NEC_command, noRepeats);
  delay(2000);
}