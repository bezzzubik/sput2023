#include <ServoTimer2.h>
#define ZEROS 60

int servoPin = 3;
ServoTimer2 Servo2;


void setupServo()
{
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  Servo2.write(10);
}


void servoGo()
{
  for (int i = 0; i < 5; i++)
  {
    Servo2.write(60);
    delay(100);
  }
}
