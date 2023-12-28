const int en12_pin = 9;
const int dir_1a_pin = 8;
const int dir_2a_pin = 7;

void dc_motor_thuan(int output_1 = HIGH, int output_2 = LOW, int speed = 255, int delay_ms = 1000);
void dc_motor_nghich(int output_1 = LOW, int output_2 = HIGH, int speed = 255, int delay_ms = 1000);
void dc_motor_cham();
void dc_motor_nhanh();

void setup() {
  Serial.begin(115200);  // UART Start
  pinMode(en12_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);

  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
}

void loop() {
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
