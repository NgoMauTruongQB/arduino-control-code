// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11);
// sensor
const int linePin = 2;
int value;
//
void setup() {
  BTSerial.begin(38400);
  Serial.begin(38400);
}

void loop() {
  value = digitalRead(linePin);
  Serial.println(value);
  if (value == 0) {
    BTSerial.write('0');
  } else {
    BTSerial.write('1');
  }
  delay(2000);
}