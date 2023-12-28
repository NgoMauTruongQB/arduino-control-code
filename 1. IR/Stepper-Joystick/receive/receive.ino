#include <IRremote.h>

#define IR_RECEIVE_PIN 2
IRrecv irrecv(IR_RECEIVE_PIN);
uint32_t NEC_received_command;

#include <Stepper.h>
#define STEPS 2048
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
const int speed = 15;  // [5, 15]
Stepper stepper(STEPS, IN1, IN3, IN2, IN4);


void setup() {
  stepper.setSpeed(speed);


  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode()) {
    NEC_received_command = irrecv.decodedIRData.command;

    if (NEC_received_command == 0x1A) {
      stepper.step(0.2 * STEPS);
    } else if (NEC_received_command == 0x1B) {
      stepper.step(-0.2 * STEPS);
    } else if (NEC_received_command == 0x1C) {
      for (int i = 0; i < 5; i++) {
        stepper.step(0.1 * STEPS);
        delay(50);
      }
    } else if (NEC_received_command == 0x1D) {
      for (int i = 0; i < 5; i++) {
        stepper.step(-0.1 * STEPS);
        delay(50);
      }
    } else if (NEC_received_command == 0xFF) {
      stepper.step(0);
      delay(100);
    }
    irrecv.resume();
  }
}
