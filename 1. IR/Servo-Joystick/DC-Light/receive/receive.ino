#include <IRremote.h>

#define IR_RECEIVE_PIN 2
IRrecv irrecv(IR_RECEIVE_PIN);
uint32_t NEC_received_command;

#define EN12_PIN 9
#define DIR_1A_PIN 8
#define DIR_2A_PIN 7

void dc_motor(int output_1, int output_2, int speed, int delay_ms) {
  digitalWrite(DIR_1A_PIN, output_1);
  digitalWrite(DIR_2A_PIN, output_2);
  analogWrite(EN12_PIN, speed);
  delay(delay_ms);
}

void setup() {
  pinMode(EN12_PIN, OUTPUT);
  pinMode(DIR_1A_PIN, OUTPUT);
  pinMode(DIR_2A_PIN, OUTPUT);

  Serial.begin(115200);
  irrecv.enableIRIn();
}
void loop() {
  if (irrecv.decode()) {
    NEC_received_command = irrecv.decodedIRData.command;
    Serial.println(NEC_received_command);

    if (NEC_received_command == 0x1C) {
      dc_motor(LOW, HIGH, 255, 500);
    } else if (NEC_received_command == 0x1D) {
      dc_motor(HIGH, LOW, 255, 500);
    }
    irrecv.resume();
  }
  dc_motor(LOW, LOW, 0, 1000);
}
