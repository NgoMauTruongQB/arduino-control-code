#define STEPPER_PIN_1 5
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

// Khai báo chân kết nối cho các cảm biến và đèn LED
int VRX_pin = A0; // Joystick trục X
int VRY_pin = A1; // Joystick trục Y
int SW_pin = 6; // Nút nhấn trên joystick

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

int val;
int step_number = 0;
void setup() {
  Serial.begin(9600);
  // Thiết lập chế độ đầu vào hoặc đầu ra cho các chân kết nối
  pinMode(VRX_pin, INPUT);
  pinMode(VRY_pin, INPUT);
  pinMode(SW_pin, INPUT_PULLUP);
  // put your setup code here, to run once:
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
  // Serial.println(VRX_value);
  // Serial.println(VRY_value);
  
  // Xoay động cơ tương ứng với hướng của joystick
  if (direction == WEST) {
    Serial.println("WEST");
    for (int a = 0; a < 2048/8; a++){   // Quay thuận 45 độ
    OneStep(true);
    delay(2);
    }
  } else if (direction == NORTH) {
    Serial.println("NORTH");
    for (int a = 0; a < 2048/4; a++){ // Quay ngược 90 độ
      OneStep(false);
      delay(2);
    }
  } else if (direction == EAST) {
    Serial.println("EAST");
    for (int a = 0; a < 2048/16; a++){ // Quay thuận 22.5 độ
      OneStep(true);
      delay(2);
    }
  } else if (direction == SOUTH) {
    Serial.println("SOUTH");
    for (int a = 0; a < 2048/2; a++){ // Quay ngược 180 độ
      OneStep(false);
      delay(2);
    }
  }

  // Đọc giá trị từ nút nhấn trên joystick
  int SW_value = digitalRead(SW_pin);

  // Nếu nút nhấn được nhấn, tắt tất cả các đèn LED
  if (SW_value == LOW) {
    Serial.println("CENTER");
    for (int a = 0; a < 2048/16; a++){ // Quay ngược 45 độ
      OneStep(true);
      delay(2);
    }
  }

      delay(500);
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