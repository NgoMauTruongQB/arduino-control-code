#include "DHT.h" // Sử dụng thư viên DHT
#define DHT11Pin 4 // Chọn chân kết nối với DHT11 là chân số 4
#define DHTType DHT11 // Chọn loại cảm biến DHT11

#define STEPPER_PIN_1 5
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

DHT HT(DHT11Pin, DHTType);

float humi; // Biến để lưu giá trị độ ẩm
float tempC; // Biến để lưu giá trị nhiệt độ theo độ C
float tempF; // Biến để lưu giá trị nhiệt độ theo độ F

int step_number = 0;

void setup() {
  Serial.begin(9600);
  HT.begin(); // Khởi động cảm biến DHT
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
}

void loop() {
  humi = HT.readHumidity(); // Đọc giá trị độ ẩm từ cảm biến
  tempC = HT.readTemperature(); // Đọc giá trị nhiệt độ theo độ C từ cảm biến
  tempF = HT.readTemperature(true); // Đọc giá trị nhiệt độ theo độ F từ cảm biến
  
  // In giá trị độ ẩm và nhiệt độ ra Serial Monitor với độ chính xác 1 chữ số thập phân
  Serial.print(" Độ ẩm:");
  Serial.print(humi, 1);
  Serial.print("%,\n");
  Serial.print("Nhiệt đô:");
  Serial.print(tempC, 1);
  Serial.print("°C ~ ");
  Serial.print(tempF, 1);
  Serial.print("°F");
  
  if (humi > 80 && tempC > 25){
    for (int a = 0; a < 2048/8; a++){   // Servo quay 90 độ
    OneStep(true);
    delay(2);
    }
  }else{
    for (int a = 0; a < 2048/4; a++){ // Servo quay 180 độ
      OneStep(false);
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