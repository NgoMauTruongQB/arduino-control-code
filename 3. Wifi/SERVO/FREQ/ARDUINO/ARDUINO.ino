#include <Servo.h>

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
  myServo.attach(9);
  myServo.write(0);
  // Đặt chế độ đầu ra và đầu vào cho các chân kết nối
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
