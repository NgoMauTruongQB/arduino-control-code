/*
 * Module L293D:
 *                                             ┏━━━━━━━━━╰╯━━━━━━━━━┑
 * to PWM pin (9)  <-- Enable 1,2            1 ┃                    ┃   9                 Vcc 1 --> to pin 5V
 * to GPIO pin (8) <-- Input 1               2 ┃                    ┃  10               Input 4
 * to motor        <-- Output 1              3 ┃                    ┃  11              Output 4
 * to GND          <-- HEAT SINK AND GND     4 ┃                    ┃  12     HEAT SINK AND GND
 *                     HEAT SINK AND GND     5 ┃                    ┃  13     HEAT SINK AND GND
 * to motor        <-- Output 2              6 ┃                    ┃  14              Output 3
 * to GPIO pin (7) <-- Input 2               7 ┃                    ┃  15               Input 3
 * to pin 5V       <-- Vcc 2                 8 ┃                    ┃  16            Enable 3,4
 *                                             ┗━━━━━━━━━━━━━━━━━━━━┙
 */
const int en12_pin = 9;
const int dir_1a_pin = 8;
const int dir_2a_pin = 7;

const int cambien = A0;

int pos, val;

void dc_motor_thuan(int output_1 = HIGH, int output_2 = LOW, int speed = 255, int delay_ms = 1000);
void dc_motor_nghich(int output_1 = LOW, int output_2 = HIGH, int speed = 255, int delay_ms = 1000);

void setup() {
  pinMode(en12_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);

  
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(cambien);
  Serial.println(val);

  if(val > 700){
    dc_motor_thuan(LOW, HIGH);
    //brake
    dc_motor_thuan(LOW, LOW, 0);
  }
  else{
    dc_motor_nghich(LOW, HIGH);
    //brake
    dc_motor_nghich(LOW, LOW, 0);
  }
  delay(1000);
}



void dc_motor_thuan(int output_1, int output_2, int speed, int delay_ms) {
  digitalWrite(dir_1a_pin, output_1);
  digitalWrite(dir_2a_pin, output_2);
  analogWrite(en12_pin, speed);
  delay(delay_ms);
}

void dc_motor_nghich(int output_1, int output_2, int speed, int delay_ms) {
  digitalWrite(dir_1a_pin, output_2); // Đảo ngược giá trị của output_1 và output_2
  digitalWrite(dir_2a_pin, output_1);
  analogWrite(en12_pin, speed);
  delay(delay_ms);
}

void dc_motor_cham(){
  analogWrite(en12_pin, 255);
  delay(200);
  analogWrite(en12_pin, 150);
  delay(200);
  analogWrite(en12_pin, 70);
  delay(200);
  analogWrite(en12_pin, 0);
  delay(200);
}

void dc_motor_nhanh(){
  analogWrite(en12_pin, 0);
  delay(200);
  analogWrite(en12_pin, 70);
  delay(200);
  analogWrite(en12_pin, 150);
  delay(200);
  analogWrite(en12_pin, 255);
  delay(200);
}