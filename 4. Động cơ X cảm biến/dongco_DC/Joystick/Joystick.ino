/*
 * Module L293D:
 *                                             ┏━━━━━━━━━╰╯━━━━━━━━━┑
 * to PWM pin (9)  <-- Enable 1,2            1 ┃                    ┃   9                 Vcc 1 --> to pin 5V
 * to GPIO pin (8) <-- Input 1               2 ┃                    ┃  10               Input 4
 * to motor        <-- Output 1              3 ┃                    ┃  11              Output 4
 * to GND          <-- HEAT SINK AND GND     4 ┃                    ┃  12     HEAT SINK AND GND
 *                     HEAT SINK AND GND     5 ┃                    ┃  13     HEAT SINK AND GND
 * to motor        <-- Output 2              6 ┃                    ┃  14              Output 3
 * to GPIO pin (7) <-- Input 2               7 ┃                    ┃  15               Input 3
 * to pin 5V       <-- Vcc 2                 8 ┃                    ┃  16            Enable 3,4
 *                                             ┗━━━━━━━━━━━━━━━━━━━━┙
 */
 // Khii gas

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

const int en12_pin = 9;
const int dir_1a_pin = 8;
const int dir_2a_pin = 7;

int pos, val;

void dc_motor_thuan(int output_1 = HIGH, int output_2 = LOW, int speed = 255, int delay_ms = 1000);
void dc_motor_nghich(int output_1 = LOW, int output_2 = HIGH, int speed = 255, int delay_ms = 1000);

void setup() {
  pinMode(VRX_pin, INPUT);
  pinMode(VRY_pin, INPUT);
  pinMode(SW_pin, INPUT_PULLUP);

  pinMode(en12_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);

  
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);

  Serial.begin(9600);
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
    Serial.println("WEST");
    dc_motor_thuan(LOW, HIGH);
    //brake
    dc_motor_thuan(LOW, LOW, 0);
  } else if (direction == NORTH) {
    Serial.println("NORTH");
    dc_motor_nghich(LOW, HIGH);
    //brake
    dc_motor_nghich(LOW, LOW, 0);
  } else if (direction == EAST) {
    Serial.println("EAST");
    dc_motor_thuan(LOW, HIGH);
    //brake
    dc_motor_thuan(LOW, LOW, 0);
  } else if (direction == SOUTH) {
    Serial.println("SOUTH");
    dc_motor_nghich(LOW, HIGH);
    //brake
    dc_motor_nghich(LOW, LOW, 0);
  }

  // Đọc giá trị từ nút nhấn trên joystick
  int SW_value = digitalRead(SW_pin);

  // Nếu nút nhấn được nhấn, tắt tất cả các đèn LED
  if (SW_value == LOW) {
    Serial.println("CENTER");
    dc_motor_thuan(LOW, HIGH);
    //brake
    dc_motor_thuan(LOW, LOW, 0);
  }

  delay(1000); 
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



void dc_motor_thuan(int output_1, int output_2, int speed, int delay_ms) {
  digitalWrite(dir_1a_pin, output_1);
  digitalWrite(dir_2a_pin, output_2);
  analogWrite(en12_pin, speed);
  delay(delay_ms);
}

void dc_motor_nghich(int output_1, int output_2, int speed, int delay_ms) {
  digitalWrite(dir_1a_pin, output_2); // Đảo ngược giá trị của output_1 và output_2
  digitalWrite(dir_2a_pin, output_1);
  analogWrite(en12_pin, speed);
  delay(delay_ms);
}

void dc_motor_cham(){
  analogWrite(en12_pin, 255);
  delay(200);
  analogWrite(en12_pin, 150);
  delay(200);
  analogWrite(en12_pin, 70);
  delay(200);
  analogWrite(en12_pin, 0);
  delay(200);
}

void dc_motor_nhanh(){
  analogWrite(en12_pin, 0);
  delay(200);
  analogWrite(en12_pin, 70);
  delay(200);
  analogWrite(en12_pin, 150);
  delay(200);
  analogWrite(en12_pin, 255);
  delay(200);
}