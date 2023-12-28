// Anh sang
#include <Servo.h>
// analogRead
int LIGHT = 0;

Servo servo;
int val;

void setup() {
  servo.attach(9, 500, 2500);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(LIGHT);
  if(val < 700){
    int angle = 90;
    servo.write(angle);
    delay(1000);
    servo.write(0);
    delay(1000);
  }
  else{
    int angle = 45;
    servo.write(angle);
    delay(1000);
    servo.write(0);
    delay(1000);
  }
  delay(1000);

  Serial.println(val);
}
