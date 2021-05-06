#include <Servo.h>
#define trigPin 7
#define echoPin 6
float duration , distance;

Servo servo;

void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
  servo.attach(8);
}

void loop() {
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);
  distance = duration*0.017;
  Serial.print("Distance: ");
  Serial.println(distance);
  if(distance>60)
  {
    for(int i=servo.read();i<90;i++)
    {
      servo.write(i);
      delay(15);
    }
  }
  else
  {
    for(int i=servo.read();i>0;i--)
    {
      servo.write(i);
      delay(15);
    }
  }
  delay(100);
}
