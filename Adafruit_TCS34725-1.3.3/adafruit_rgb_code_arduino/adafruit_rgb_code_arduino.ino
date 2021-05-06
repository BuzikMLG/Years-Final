#include<Wire.h>
#include "Adafruit_TCS34725.h"
#include<SoftwareSerial.h>
#define LEDsensor 6
#define LED_RED 7
#define LED_GREEN 8
#define LED_BLUE 9
byte flag=0;
 Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,TCS34725_GAIN_4X);
void setup(){
pinMode(LED_RED,OUTPUT);
pinMode(LED_GREEN,OUTPUT);
pinMode(LED_BLUE,OUTPUT);
pinMode(LEDsensor,OUTPUT);
digitalWrite(LEDsensor,LOW);
Serial.begin(9600);
Serial.println("ColorViewTest!");
if(tcs.begin()){
Serial.println("Foundsensor");
}
else{
Serial.println("No TCS34725 found ");
while(1);// halt!
}
}
void loop(){
word clear, red, green, blue;
long sum;
float r, g, b;
delay(500);
tcs.getRawData(&red,&green,&blue,&clear);
Serial.print("C:\t");Serial.print(clear);
Serial.print("\tR:\t");Serial.print(red);
Serial.print("\tG:\t");Serial.print(green);
Serial.print("\tB:\t");Serial.print(blue);
sum=clear;
r=red;
r/=sum;
g=green;
g/=sum;
b=blue;
b/=sum;
r=100;
g=100;
b*=100;
 Serial.print("\t");
 Serial.print((int)r);
 Serial.print(" ");
 Serial.print((int)g);
 Serial.print(" ");
 Serial.println((int)b);
 Serial.println();
if((red>green)&&(red>blue)&&(flag!=1)){
 flag=1;
 Serial.println("red");
 digitalWrite(LED_RED,HIGH);
 digitalWrite(LED_GREEN,LOW);
 digitalWrite(LED_BLUE,LOW);
 }
 else if((green>red)&&(green>blue)&&(flag!=2)){
 flag=2;
 Serial.println("green");
 digitalWrite(LED_RED,LOW);
 digitalWrite(LED_GREEN,HIGH);
 digitalWrite(LED_BLUE,LOW);
 }
 else if((blue>red)&&(blue>green)&&(flag!=3)){
 flag=3;
 Serial.println("blue");
 digitalWrite(LED_RED,LOW);
 digitalWrite(LED_GREEN,LOW);
 digitalWrite(LED_BLUE,HIGH);
 }
}
