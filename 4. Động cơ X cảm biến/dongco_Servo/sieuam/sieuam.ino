// Sieu am
#include <Servo.h>

// digital
int trig = 3;
int echo = 5;
int pos = 0;
Servo servo;
float distance;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servo.attach(9, 500, 2500);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  int duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2.0;

  Serial.println(distance);

  if(distance < 5) {
    int angle = 90;
    servo.write(angle);
    delay(1000);
    servo.write(0);
    delay(1000);
  } else {
    int angle = 45;
    servo.write(angle);
    delay(1000);
    servo.write(0);
    delay(1000);
  }
  delay(1000);
}

