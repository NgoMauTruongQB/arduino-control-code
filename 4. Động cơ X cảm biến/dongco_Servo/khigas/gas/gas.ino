
// Khii gas
#include <Servo.h>

const int cambien = A0;

Servo servo;
int pos, val;

void setup() {
  servo.attach(9, 500, 2500);
  pinMode(cambien, INPUT);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(cambien);
  Serial.println(val);

  if(val > 700){
    int angle = 45;
    servo.write(angle);
    delay(1000);
    servo.write(0);
    delay(1000);
  }
  // else{
  //   int angle = 90;
  //   servo.write(angle);
  //   delay(1000);
  //   servo.write(0);
  //   delay(1000);
  // }
  delay(1000);
}