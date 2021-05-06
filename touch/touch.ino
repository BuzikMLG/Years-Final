
//===============================================
#include "spiUnit.h"
#include "TFT9341.h"
#include "touch.h"
#include "Button.h"

#include <string.h>


void printToLcd(point pos, size_t fg, size_t bg, size_t fs, char * msg){
	lcd.gotoxy(pos.x,pos.y);
	lcd.setColor(fg, bg);
	lcd.setFont(fs);
	lcd.print(msg);
}

void button_screen(char * code) {
	lcd.clrscr(BLACK);

	size_t startx = 0;
	size_t starty = 55;
	Button btn_list[4][3];
	char msg[12][4][2] =  {{"1", "2", "3"}, {"4", "5", "6"}, {"7", "8", "9"}, {"*", "0", "#"}};
	Button btn;

	printToLcd(point(20, 20), WHITE, BLACK, 2, "Enter your PIN code: ");

	for (int i = 0; i < 4; i ++){
  	startx = 20; 
  	for (int j = 0; j < 3; j++){
    	btn = Button(point(startx, starty), msg[i][j]);
    	btn.draw();
    	btn_list[i][j] = btn;
    	startx += 55;
  	}
	  starty += 45;
  }
  button_screen_touch(btn_list, code);
  
}



void button_screen_touch(Button btn_list[4][3], char * code){
	static byte pos = 0;
	printToLcd(point(190, 60), WHITE, BLACK, 3, code);
	while (pos < 4) {
  	lcdtouch.readxy();
  	size_t x = 320 - lcdtouch.readx();
  	size_t y = 240 - lcdtouch.ready();
  	delay(120); 
  	for (byte i = 0; i < 4; i++){
			for (byte j = 0; j < 3; j++){
					if (btn_list[i][j].pressed(x, y)) {
  					code[pos] = btn_list[i][j].getMsg()[0];
  					pos++;
  					printToLcd(point(190, 60), WHITE, BLACK, 3, code);
				}
			}
		} 
	}
}


size_t option_screen(size_t balance) {
	lcd.clrscr(BLACK);
	char msg[50];
	sprintf(msg, "withdraw? Balance: %d", balance); 
	printToLcd(point(0, 0), WHITE, BLACK, 2, "How much would like to");
	printToLcd(point(0, 20), WHITE, BLACK, 2, msg);
}


void setup() {	
	Serial.begin(9600);
	lcd.begin();
	lcdtouch.begin();
	lcd.clrscr (BLACK);
	char code[5] = "____";
	option_screen(1000);
  /*
	button_screen(code);
  Serial.print(code);
  */
}


void loop() {
}
