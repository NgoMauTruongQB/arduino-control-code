// Nhiet do do am
#include <Servo.h>
#include "DHT.h" // Sử dụng thư viên DHT
#define DHT11Pin 4 // Chọn chân kết nối với DHT11 là chân số 4
#define DHTType DHT11 // Chọn loại cảm biến DHT11

DHT HT(DHT11Pin, DHTType);

Servo servo_9;


float tmprVal; // Biến để lưu giá trị độ ẩm
float tempC; // Biến để lưu giá trị nhiệt độ theo độ C
float tempF; // Biến để lưu giá trị nhiệt độ theo độ F

int pos;
void setup() {
  servo_9.attach(9, 500, 2500);
  Serial.begin(9600);
  HT.begin();
}

void loop() {

  tmprVal= HT.readHumidity(); 
  tempC = HT.readTemperature(); 
  tempF = HT.readTemperature(true);
  Serial.print(" doam:");
  Serial.print(tmprVal, 1);
  Serial.print("%, ");
  Serial.print("nhiet do :");
  Serial.print(tempC, 1);
  Serial.print("°C ~ ");
  Serial.print(tempF, 1);
  Serial.print("°F,\n");
  
  if(tmprVal > 85 && tempC > 25){
    pos = 180;
      servo_9.write(pos);
      delay(1000);
      pos = 0;
      servo_9.write(pos);
      delay(1000);
  }
  else{
    pos = 45;
      servo_9.write(pos);
      delay(1000);
      pos = 0;
      servo_9.write(pos);
      delay(1000);
  }
  delay(1000);
}
