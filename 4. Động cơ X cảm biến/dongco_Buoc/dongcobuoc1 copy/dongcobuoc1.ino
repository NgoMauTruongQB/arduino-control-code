// #define STEPPER_PIN_1 5
// #define STEPPER_PIN_2 9
// #define STEPPER_PIN_3 10
// #define STEPPER_PIN_4 11
// #define  cambien  A0

// int val;
// int step_number = 0;
// void setup() {
//   // put your setup code here, to run once:
//   pinMode(STEPPER_PIN_1, OUTPUT);
//   pinMode(STEPPER_PIN_2, OUTPUT);
//   pinMode(STEPPER_PIN_3, OUTPUT);
//   pinMode(STEPPER_PIN_4, OUTPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:

//    val = analogRead(cambien);

//   XoayXuoiNguoc(val);
//   Serial.print("Giá trị cảm biến: ");
//   Serial.println(val);
//   // NhanhDan();
// }

// void XoayXuoiNguoc(int val){
//   if (val > 600){
//     for (int a = 0; a < 2048/8; a++){
//     OneStep(true);
//     delay(2);
//     }
//   }else{
//     for (int a = 0; a < 2048/4; a++){
//       OneStep(false);
//       delay(2);
//     }
//   }
//       delay(500);

// }

// void NhanhDan(){
//   for(int i = 0; i <= 2048; i++){
//     OneStep(true);
//     delay(map(2048 - i, 0, 2048, 2, 10));
//   }
// }

// void OneStep(bool dir){
//   if(dir){
//     switch (step_number) {
//       case 0:
//         digitalWrite(STEPPER_PIN_1, HIGH);
//         digitalWrite(STEPPER_PIN_2, LOW);
//         digitalWrite(STEPPER_PIN_3, LOW);
//         digitalWrite(STEPPER_PIN_4, LOW);
//         break;
//       case 1:
//         digitalWrite(STEPPER_PIN_1, LOW);
//         digitalWrite(STEPPER_PIN_2, HIGH);
//         digitalWrite(STEPPER_PIN_3, LOW);
//         digitalWrite(STEPPER_PIN_4, LOW);
//         break;
//       case 2:
//         digitalWrite(STEPPER_PIN_1, LOW);
//         digitalWrite(STEPPER_PIN_2, LOW);
//         digitalWrite(STEPPER_PIN_3, HIGH);
//         digitalWrite(STEPPER_PIN_4, LOW);
//         break;
//       case 3:
//         digitalWrite(STEPPER_PIN_1, LOW);
//         digitalWrite(STEPPER_PIN_2, LOW);
//         digitalWrite(STEPPER_PIN_3, LOW);
//         digitalWrite(STEPPER_PIN_4, HIGH);
//         break;
//     }
//   }
//   else{
//     switch(step_number){
//       case 0:
//         digitalWrite(STEPPER_PIN_1, LOW);
//         digitalWrite(STEPPER_PIN_2, LOW);
//         digitalWrite(STEPPER_PIN_3, LOW);
//         digitalWrite(STEPPER_PIN_4, HIGH);
//         break;
//       case 1:
//         digitalWrite(STEPPER_PIN_1, LOW);
//         digitalWrite(STEPPER_PIN_2, LOW);
//         digitalWrite(STEPPER_PIN_3, HIGH);
//         digitalWrite(STEPPER_PIN_4, LOW);
//         break;
//       case 2:
//         digitalWrite(STEPPER_PIN_1, LOW);
//         digitalWrite(STEPPER_PIN_2, HIGH);
//         digitalWrite(STEPPER_PIN_3, LOW);
//         digitalWrite(STEPPER_PIN_4, LOW);
//         break;
//       case 3:
//         digitalWrite(STEPPER_PIN_1, HIGH);
//         digitalWrite(STEPPER_PIN_2, LOW);
//         digitalWrite(STEPPER_PIN_3, LOW);
//         digitalWrite(STEPPER_PIN_4, LOW);
//         break;
//     }
//   }
//   step_number++;
//   if(step_number > 3){
//     step_number = 0;
//   }
// }


#include <IRremote.h>
#define STEPPER_PIN_1 5
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

int val;
int step_number = 0;
IRrecv irrecv(11); // Chân số 11 trên Arduino
decode_results results; 

void setup() {
  Serial.begin(9600);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  irrecv.enableIRIn(); // Bắt đầu nhận tín hiệu hồng ngoại
}

void loop() {
  // 360do = 1024
  if (irrecv.decode(&results)) { // Nếu nhận được tín hiệu hồng ngoại thì run code
    if(results.value == 256){ // Nếu tín hiệu = 256 thì servo quay 90 độ
      for (int a = 0; a < 2048/8; a++){
        OneStep(true);
        delay(2);
        }
    }else if (results.value == 257){ // nếu không thì quay 180 độ
      for (int a = 0; a < 2048/4; a++){
        OneStep(false);
        delay(2);
      }
    }
  Serial.print("Received IR code: ");
  Serial.println(results.value);
  delay(100);
  irrecv.resume(); // Tiếp tục nhận tín hiệu hồng ngoại 
  }
  delay(300);
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