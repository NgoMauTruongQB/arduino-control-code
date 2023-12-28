const int en12_pin = 9;
const int dir_1a_pin = 8;
const int dir_2a_pin = 7;

void dc_motor_thuan(int output_1 = HIGH, int output_2 = LOW, int speed = 255, int delay_ms = 1000);
void dc_motor_nghich(int output_1 = LOW, int output_2 = HIGH, int speed = 255, int delay_ms = 1000);
void dc_motor_cham();
void dc_motor_nhanh();

const int trigPin = 5;
const int echoPin = 3;

// Biến cho sóng sin và giá trị tần số
float sinVal;
int toneVal;

// Biến để lưu thời gian phản hồi và khoảng cách
long duration;
float distance;

void setup() {
  Serial.begin(115200);  // UART Start
  pinMode(en12_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);

  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Gửi xung siêu âm
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Đọc thời gian phản hồi để tính toán khoảng cách
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2.0;

   // In ra giá trị khoảng cách trên Serial Monitor
  Serial.print("khoang cach = ");
  Serial.print(distance);
  Serial.print("cm\n");
  
  // Tính toán giá trị tần số và gửi nó qua Serial
  sinVal = sin(distance * 0.1); // Thay đổi hệ số để điều chỉnh giá trị tần số
  toneVal = 2000 + (int(sinVal * 1000));
  Serial.println("freq" + String(toneVal));

  // Gửi giá trị khoảng cách qua Serial
  Serial.println("dist" + String(distance));
  
  if (Serial.available() > 0) {
    String command = Serial.readString();
    if (command.length() > 0) {
      Serial.println("Received command: " + command);

      if (command.startsWith("thuan")) {
        dc_motor_thuan(LOW, HIGH);
        // Brake
        dc_motor_thuan(LOW, LOW, 0);
        delay(500);
      } else if (command.startsWith("nghich")) {
        dc_motor_nghich(LOW, HIGH);
        // Brake
        dc_motor_nghich(LOW, LOW, 0);
        delay(500);
      } else if (command.startsWith("nhanh")) {
        dc_motor_nhanh();
        delay(500);
      } else if (command.startsWith("cham")) {
        dc_motor_cham();
        delay(500);
      }
    }
  }
}

void dc_motor_thuan(int output_1, int output_2, int speed, int delay_ms) {
  if (output_1 != digitalRead(dir_1a_pin) || output_2 != digitalRead(dir_2a_pin)) {
    digitalWrite(dir_1a_pin, output_1);
    digitalWrite(dir_2a_pin, output_2);
  }
  if (speed != analogRead(en12_pin)) {
    analogWrite(en12_pin, speed);
  }
  delay(delay_ms);
}

void dc_motor_nghich(int output_1, int output_2, int speed, int delay_ms) {
  if (output_1 != digitalRead(dir_1a_pin) || output_2 != digitalRead(dir_2a_pin)) {
    digitalWrite(dir_1a_pin, output_2);
    digitalWrite(dir_2a_pin, output_1);
  }
  if (speed != analogRead(en12_pin)) {
    analogWrite(en12_pin, speed);
  }
  delay(delay_ms);
}

void dc_motor_cham() {
  analogWrite(en12_pin, 255);
  delay(200);
  analogWrite(en12_pin, 150);
  delay(200);
  analogWrite(en12_pin, 70);
  delay(200);
  analogWrite(en12_pin, 0);
  delay(200);
}

void dc_motor_nhanh() {
  analogWrite(en12_pin, 0);
  delay(200);
  analogWrite(en12_pin, 70);
  delay(200);
  analogWrite(en12_pin, 150);
  delay(200);
  analogWrite(en12_pin, 255);
  delay(200);
}
