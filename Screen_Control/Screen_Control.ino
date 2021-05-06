#include "TFT9341.h"
#include "Defines.h"
#include "touch.h"
#include "Button.h"
#include <Servo.h>
#include "MagStripe.h"

Servo Gate;
void uploadToLCD(size_t x_location, size_t y_location, size_t font_color, size_t background_color, size_t font_size, char* data);
void OpenGate();
void CloseGate();
void printToLCD(char* message);
void ManualPressed();
void AutoPressed();
void ClosePressed();
void OpenPressed();
void printLoadScreen(bool state);
bool buttonState= false;
bool modeChosen = false;
bool ControlType = false;
void readMagCard();
int pos = 0;
static const byte DATA_BUFFER_LEN = 108;
static char data[DATA_BUFFER_LEN];
MagStripe card;


Button Open(10,100,150,60,GREEN,BLACK,"OPEN");
Button Close(10,170,150,60,RED,BLACK,"CLOSE");
Button Auto(8,100,150,60,BLUE,BLACK,"AUTO");
Button Manual(163,100,150,60,YELLOW,BLACK,"MANUAL");
Button Back(10,10,80,30,GRAY,WHITE,"BACK");

void setup()
{  
   Serial.begin(9600);
   card.begin(2);
   lcd.begin();
   lcd_touch.begin();
   lcd.clrscr(WHITE);
   uploadToLCD(20,50,BLACK,WHITE,2,"Choose a Control Setting");
   Auto.drawButton(3);
   Manual.drawButton(3);
   //Gate.attach(3);
   CloseGate();
}


/*-
 * This is a function that prints the message with all the configuration that it recieves
 * Input:the configurations as listed
 * Output:None
 * Physical Output:The message will be shown on the lcd screen with the correct configuration.
 * 
 */
void uploadToLCD(size_t x_location, size_t y_location, size_t font_color, size_t background_color, size_t font_size, char* data)
{
  lcd.gotoxy(x_location,y_location);
  lcd.setColor(font_color,background_color);
  lcd.setFont(font_size);
  lcd.print(data);
}


/*
 * This function is a smart function that will print a long string of data with new lines.
 * Input:The message to print
 * Output:None
 * Physical Output:Message will be shown of the lcd screen.
 * 
 */
void printToLCD(char* message)
{
  char * split = strtok(message,'\n');
  char * rest = message;
  int y_start_location = -5;
  while ((split = strtok_r(rest, "\n", &rest))) 
  {
    y_start_location+=20;
    uploadToLCD(10,y_start_location,BLACK,WHITE,2,split);
  }
}


void loop()
{  
  if(ControlType && modeChosen)//this is for the manual screen
  {
     Open.updateButtonState();
     Close.updateButtonState();
     Back.updateButtonState();
     if(Open.getButtonState() && buttonState!=true)
     {
        OpenPressed();
     }
     if(Close.getButtonState() && buttonState!=false)
     {       
        ClosePressed ();
     }     
     if(Back.getButtonState())
     {       
         buttonState=false;
         modeChosen=false;
         lcd.clrscr(WHITE);
         uploadToLCD(20,50,BLACK,WHITE,2,"Choose a Control Setting");
         Auto.drawButton(3);
         Manual.drawButton(3);
     }
   }
   else if (!ControlType && modeChosen)//this is for the auto screen
   {
      Back.updateButtonState();
      readMagCard();
      if(Back.getButtonState())
      {
          modeChosen=false;
          lcd.clrscr(WHITE);
          uploadToLCD(20,50,BLACK,WHITE,2,"Choose a Control Setting");
          Auto.drawButton(3);
          Manual.drawButton(3);
      }
   }
   else if (modeChosen==false)//this is to show the type of control screen
   {
     Manual.updateButtonState();
     Auto.updateButtonState();
     if(Auto.getButtonState())
     {
        lcd.clrscr(WHITE);
        AutoPressed();

     }
     else if(Manual.getButtonState()) 
     {
        lcd.clrscr(WHITE);
        ManualPressed();
     }
   }
}
   

void printLoadScreen(bool state)
{
  if(state)
  {
      OpenGate();
      lcd.drawRect(170,100,145,60);
      for(size_t i=172;i<313;i++)
      {
        lcd.drawLine(i,102,i,158,GREEN);
        delay(2);
      }
  }
  else
  {
      CloseGate();
      lcd.drawRect(170,170,145,60);
      for(size_t i=172;i<313;i++)
      {
        lcd.drawLine(i,172,i,228,GREEN);
        delay(2);
      }
  }
}


void ManualPressed()
{
    lcd.clrscr(WHITE);
    uploadToLCD(30,50,BLACK,RED,2,"Gate State -> [Closed]");
    Open.drawButton(3);
    Close.drawButton(3);
    Back.drawButton(2);
    modeChosen=true;
    ControlType=true;
}



void AutoPressed()
{
     modeChosen=true;
     ControlType=false;
     Back.drawButton(2);
}


void OpenPressed()
{
     buttonState=true;
     printLoadScreen(true);
     lcd.clrscr(WHITE);
     uploadToLCD(30,50,BLACK,GREEN,2,"Gate State -> [Opened]");
     Close.setPressed(false);
     Back.drawButton(2);
     Open.drawButton(3);
     Close.drawButton(3);
}


void ClosePressed()
{
     buttonState=false;
     printLoadScreen(false);
     lcd.clrscr(WHITE);
     uploadToLCD(30,50,BLACK,RED,2,"Gate State -> [Closed]");
     Open.setPressed(false);
     Back.drawButton(2);
     Open.drawButton(3);
     Close.drawButton(3);
}



void OpenGate()
{
  for(pos=Gate.read();pos>=12;pos--)
  {
    Gate.write(pos);
    delay(15);
  }
}

void CloseGate()
{  
  for(pos = Gate.read();pos<=75;pos++)
  {
    Gate.write(pos);
    delay(15);
  }

}

/*
 * This Function reads from the magnetic card reader, and updates the data buffer
 * Input:None
 * Output:None
 * Physical Output:None
 * 
 */
void readMagCard()
{
  short chars;
  if (card.available()) {
      chars = card.read(data, DATA_BUFFER_LEN);
      Serial.println(data);  
  }
}
