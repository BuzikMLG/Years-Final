#include <SPI.h>
#include "TFT9341.h"
#include <SD.h>

#define SD_CS 4


void setup(void) {
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");

  lcd.begin();
  lcd.clrscr();
  
  bmpDraw("photo1.bmp", 0, 40);
}

void loop() {
}


