//===============================================
//                   Example3_11
//===============================================
#include <SPI.h>
#include "TFT9341.h"
#include <SD.h>
#include "touch.h"
#include "Wire.h"
#define Tout 5 
byte hour=12,minute=59 ,second=57;

#define speakerPin 6
unsigned int x,y;
int toneArr[3]={523, 698, 987};
#define SD_CS 4
void setup() {
  
  Wire.begin();
  setupTime();
  setupTout();
  pinMode(Tout,INPUT_PULLUP);
  lcd.begin();
  lcdtouch.begin();
  lcd.clrscr();
  lcd.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) 
         lcd.println("failed!");
   else{
       lcd.println("OK!");
       delay(1000);
       lcd.clrscr();
         // bmpDraw("logo.bmp",  0,  0);
   }
}//End of setup function 


void loop() {
  byte sel;
     printIcon();
     while(digitalRead(2));
     lcdtouch.readxy();
     x=lcdtouch.readx();
     y=lcdtouch.ready();
     lcd.print(digitalRead(2));
     sel=testTouch();
    
    switch(sel){
     case 0:  
              bmpDraw("PIC1.bmp",  0,  0);
              break;
     case 1:  bmpDraw("PIC1.bmp",  0,  0);
              break;
     case 2:  lcd.clrscr();             
              rtc();
               break;
   }
   
    }//End of loop function 

//===============================================
//         End Of File "Example3_8"        
//===============================================

