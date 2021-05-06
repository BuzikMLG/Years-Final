//===============================================
//                   Example3_8
//===============================================
#include <SPI.h>
#include "TFT9341.h"
#include <SD.h>
#define SD_CS 4
void setup() {
  lcd.begin();
  //lcd.clrscr();
  lcd.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) 
         lcd.println("failed!");
   else{
         lcd.println("OK!");
     delay(1000);
     lcd.clrscr();
     for(byte j=0;j<3;j++)
     for(byte i=0;i<3;i++)
        bmpDraw("PIC1.bmp", i*100, j*80);
     }
}//End of setup function 


void loop() {
    }//End of loop function 

//===============================================
//         End Of File "Example3_8"        
//===============================================

