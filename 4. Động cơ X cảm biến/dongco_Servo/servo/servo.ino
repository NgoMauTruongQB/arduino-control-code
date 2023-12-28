// Sieu am
#include <Servo.h>
int pos = 0;
Servo servo_9;
void setup()
{
  servo_9.attach(9, 500, 2500);
}
void loop()
{
    servo_9.write(pos);
    delay(500);
    servo_9.write(pos);
    delay(500);
}

