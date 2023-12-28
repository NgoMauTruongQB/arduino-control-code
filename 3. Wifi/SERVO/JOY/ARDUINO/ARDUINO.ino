#include <Servo.h>

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
  myServo.attach(9);
  myServo.write(0);
  pinMode(VRX_pin, INPUT);
  pinMode(VRY_pin, INPUT);
  pinMode(SW_pin, INPUT_PULLUP);
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

  // Đọc giá trị từ Serial để kiểm soát động cơ servo
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    if (command.startsWith("servo")) {
      // Lấy giá trị góc từ lệnh, ví dụ: "servo90"
      int angle = command.substring(5).toInt();

      // Giới hạn giá trị góc trong khoảng 0 đến 180 độ
      angle = constrain(angle, 0, 180);

      // Di chuyển servo đến góc tương ứng
      myServo.write(angle);
      delay(500);
      myServo.write(0);
      delay(500);

      Serial.println("Moved servo to angle: " + String(angle));
    }
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
