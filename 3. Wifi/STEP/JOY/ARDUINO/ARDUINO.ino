#include <Servo.h>

#define STEPPER_PIN_1 12
#define STEPPER_PIN_2 11
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 9

// analogRead
int val;
int step_number = 0;

Servo myServo;
// Khai báo chân kết nối cho các cảm biến và đèn LED
// analog
int VRX_pin = A0; // Joystick trục X
int VRY_pin = A1; // Joystick trục Y
// digital
int SW_pin = 5; // Nút nhấn trên joystick

// Các hướng có thể của joystick
const int NORTH = 0;
const int NORTH_EAST = 1;
const int EAST = 2;
const int SOUTH_EAST = 3;
const int SOUTH = 4;
const int SOUTH_WEST = 5;
const int WEST = 6;
const int NORTH_WEST = 7;
const int CENTER = 8;

// Các biến để lưu giá trị đọc từ joystick và hướng hiện tại
int VRX_value, VRY_value;
int direction = -1, direction_cache;

// Hàm chuyển đổi giá trị từ joystick sang hướng
int coor_to_direction(int VRX_value, int VRY_value);

// Các hàm kiểm tra phạm vi giá trị của joystick
bool is_large_coor(int coor);
bool is_middle_coor(int coor);
bool is_small_coor(int coor);

void setup() {
  Serial.begin(115200);  // UART Start
  pinMode(VRX_pin, INPUT);
  pinMode(VRY_pin, INPUT);
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
}

void loop() {
  // Đọc giá trị từ joystick
  VRX_value = analogRead(VRX_pin);
  VRY_value = analogRead(VRY_pin);

  // Xác định hướng từ giá trị joystick
  direction = coor_to_direction(VRX_value, VRY_value);

  // In giá trị đọc từ trục Y của joystick ra Serial Monitor

  // Xoay động cơ tương ứng với hướng của joystick
  if (direction == WEST) {
    Serial.println("joyWEST");
  } else if (direction == NORTH) {
    Serial.println("joyNORTH");
  } else if (direction == EAST) {
    Serial.println("joyEAST");
  } else if (direction == SOUTH) {
    Serial.println("joySOUTH");
  }

  // Đọc giá trị từ nút nhấn trên joystick
  int SW_value = digitalRead(SW_pin);

  // Nếu nút nhấn được nhấn, tắt tất cả các đèn LED
  if (SW_value == LOW) {
    Serial.println("joyPUSH");
  }

  if (Serial.available()) {
    String command = "";
    while (Serial.available()) {
      command = Serial.readString();
    }
    Serial.println("Received command: " + command);

    if (command.startsWith("t")) {
      int angle = command.substring(1).toInt();
      for (int a = 0; a < int(2048 / 360 * angle); a++) {
        OneStep(true);
        delay(2);
      }
    } else {
      int angle = command.substring(1).toInt();
      for (int a = 0; a < int(2048 / 360 * angle); a++) {
        OneStep(false);
        delay(2);
      }
    }
    delay(500);
  }

  delay(1000); // Đợi một chút trước khi lặp lại
}

// Hàm chuyển đổi giá trị từ trục X và Y của joystick sang hướng
int coor_to_direction(int VRX_value, int VRY_value) {
  if (is_large_coor(VRX_value) && is_middle_coor(VRY_value))
    return NORTH;
  if (is_large_coor(VRX_value) && is_large_coor(VRY_value))
    return NORTH_EAST;
  if (is_middle_coor(VRX_value) && is_large_coor(VRY_value))
    return EAST;
  if (is_small_coor(VRX_value) && is_large_coor(VRY_value))
    return SOUTH_EAST;
  if (is_small_coor(VRX_value) && is_middle_coor(VRY_value))
    return SOUTH;
  if (is_small_coor(VRX_value) && is_small_coor(VRY_value))
    return SOUTH_WEST;
  if (is_middle_coor(VRX_value) && is_small_coor(VRY_value))
    return WEST;
  if (is_large_coor(VRX_value) && is_small_coor(VRY_value))
    return NORTH_WEST;
  return CENTER; // Trả về hướng trung tâm nếu không phát hiện hướng nào khác
}

// Các hàm kiểm tra phạm vi giá trị của joystick
bool is_large_coor(int coor) {
  return coor > 614;
}
bool is_middle_coor(int coor) {
  return (coor <= 614) && (coor >= 400);
}
bool is_small_coor(int coor) {
  return coor < 400;
}

void OneStep(bool dir){
  if(dir){
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  }
  else{
    switch(step_number){
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
    }
  }
  step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}

