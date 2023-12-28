/* 
Servo: D9
*/

#include <Servo.h>

Servo myServo;

void setup() {
  Serial.begin(115200);  // UART Start
  myServo.attach(9);
  myServo.write(0);
}

void loop() {
  if (Serial.available()) {
    String command = "";
    while (Serial.available()) {
      command = Serial.readString();
    }
    Serial.println("Received command: " + command);

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
}