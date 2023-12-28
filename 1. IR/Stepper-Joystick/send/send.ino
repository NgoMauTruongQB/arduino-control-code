#include <IRremote.h>

#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t NEC_command;
uint32_t noRepeats = 2;

int SW_pin = 5; // Nút nhấn trên joystick
const int VRX_pin = A0;
const int VRY_pin = A1;

void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  Serial.begin(115200);
}

int x_pos, y_pos;
void loop() {
  x_pos = analogRead(VRX_pin);
  y_pos = analogRead(VRY_pin);
  Serial.print("(");
  Serial.print(x_pos);
  Serial.print(", ");
  Serial.print(y_pos);
  Serial.println(")");


  // Đọc giá trị từ nút nhấn trên joystick
  int SW_value = digitalRead(SW_pin);

  if (x_pos <= 300) {
    Serial.println("South");
    NEC_command = 0x1D;
  } else if (x_pos >= 700) {
    Serial.println("North");
    NEC_command = 0x1C;
  } else if (y_pos <= 300) {
    Serial.println("West");
    NEC_command = 0x1B;
  } else if (y_pos >= 700) {
    Serial.println("East");
    NEC_command = 0x1A;
  }

  if (SW_value == LOW) {
    Serial.println("CENTER");
    // Gạt phải: Chậm dần
    NEC_command = 0x1E;
  }

  irsend.sendNEC(0x00, NEC_command, noRepeats);
  delay(300);
}