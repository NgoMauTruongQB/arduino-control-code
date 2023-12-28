#include <Servo.h>
#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

// analogRead
int val;
int step_number = 0;

Servo myServo;
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
  // Đặt chế độ đầu ra và đầu vào cho các chân kết nối
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
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


