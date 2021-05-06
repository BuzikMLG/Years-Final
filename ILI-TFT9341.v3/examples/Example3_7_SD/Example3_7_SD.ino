//===============================================
//                   Example3_7
//===============================================
#include <SPI.h>
#include "TFT9341.h"
#include <SD.h>
#define SD_CS 4
void setup() {
  lcd.begin();
  lcd.clrscr();
  lcd.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) 
         lcd.println("failed!");
   else
         lcd.println("OK!");
     delay(1000);
     lcd.clrscr();
    
        bmpDraw("logo1.bmp", 0, 0);

}//End of setup function 


void loop() {
    }//End of loop function 

//===============================================
//         End Of File "Example3_7"        
//===============================================

