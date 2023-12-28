// #include "C:\\Users\\nguye\\OneDrive\\Documents\\Arduino\\libraries\\IRremote\\src\\IRremote.hpp"
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
const int speed = 15; // [5, 15]
Stepper stepper(STEPS, IN1, IN3, IN2, IN4);


void setup() {
  stepper.setSpeed(speed);



  Serial.begin(115200);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode()) {
    NEC_received_command = irrecv.decodedIRData.command;
    Serial.println(NEC_received_command);
    if (NEC_received_command == 0x1C) {
      stepper.step(0.25 * STEPS); //độ/360 * steps
      delay(200);
    }
    else if (NEC_received_command == 0x1D)  {
      stepper.step(-0.25 * STEPS);
      delay(200);
    }
    irrecv.resume();
  }
}
