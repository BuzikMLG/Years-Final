#include "TFT9341.h"
#include "Defines.h"
#include "touch.h"
#include "Button.h"
#include <Servo.h>
#include "MagStripe.h"


char** arrayOfPasswords;

#define ADMIN_CODE ";0710000000000000003234270620211?7" // this is a premade code that is known and will act as an admin code
#define trigPin 7
#define echoPin 6
#define CARD_AMOUNT 5 


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
bool checkPassword(char* password);


bool CONFIGURATION_MODE = true;//this is a configuration sequence - if set true will ask for 5 passwords if false will continue regularly
int card_number = CARD_AMOUNT;//amount of possible cards


bool buttonState= false;
bool modeChosen = false;
bool ControlType = false;

void readMagCard();
void SuffDistance();
void AutoGate();
void drawGateOpen();
void drawGateClose();

int pos = 0;
bool gateStatus = false;
bool firstEnter = true;
bool SuffDistance_state = false;

static const byte DATA_BUFFER_LEN = 108;
static char data[DATA_BUFFER_LEN];
MagStripe card;

Button DEBUG(100,100,150,60,BLACK,WHITE,"Configure");


Button Open(10,100,150,60,GREEN,BLACK,"OPEN");
Button Close(10,170,150,60,RED,BLACK,"CLOSE");
Button Auto(8,100,150,60,BLUE,BLACK,"AUTO");
Button Manual(163,100,150,60,YELLOW,BLACK,"MANUAL");
Button Back(10,10,80,30,GRAY,WHITE,"BACK");
Button EnterState_nopress(10,50,290,160,GREEN,WHITE,"GATE OPEN");
Button CloseState_nopress(10,50,290,160,RED,WHITE,"GATE CLOSED");


/*
 * This is the setup function
 * INPUT:NONE
 * OUTPUT:NONE
 * The function sets the data and configures the basic parts
 * The function creates the first buttons that are presented to the user
 */
void setup()
{  
   Serial.begin(9600);
   pinMode(trigPin,OUTPUT);
   pinMode(13,OUTPUT);
   pinMode(echoPin,INPUT);//here all the part of the project are being initialized
   card.begin(2);
   lcd.begin();
   lcd_touch.begin();
   lcd.clrscr(WHITE);
   if(CONFIGURATION_MODE==true)
   {
    card_number = CARD_AMOUNT;
    DEBUG.drawButton(2);
    digitalWrite(13,HIGH);
    delay(1500);
    digitalWrite(13,LOW);

   }
   else
   {
    uploadToLCD(20,50,BLACK,WHITE,2,"Choose a Control Setting");//first display image is shown
    Auto.drawButton(3);
    Manual.drawButton(3);
   }
   Gate.attach(5);
   CloseGate();
}


/*-
 * This is a function that prints the message with all the configuration that it recieves
 * Input:the configurations as listed
 * Output:None
 * Physical Output:The message will be shown on the lcd screen with the correct configuration.
 * 
 */
void uploadToLCD(size_t x_location, size_t y_location, size_t font_color, size_t background_color, size_t font_size, char* data)//this is an auxiliary function which purpose is to ease the showing of a text on the screen
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
void printToLCD(char* message)//helper function just to print long data parts
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


/*
 * This is the loop function
 * INPUT:NONE
 * OUTPUT:NONE
 * The Functions controls the flow of the code and creates the user interface
 * The function first checks the debug mode for an option to enter 5 card that itll recognize
 * Then after/if entered it gets to the mode selection AUTO/MANUAL
 * After the mode selection it lets the user perform actions accordingly to the mode he chose.
 */
void loop()
{ 

  while(!DEBUG.getButtonState() && CONFIGURATION_MODE)
  {
    char curr_card[108];
    readMagCard();
    if(strlen(data)>0){
      Serial.println("Entered");
      Serial.println(data);
      strncpy(curr_card,data,strlen(data));
      digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
      Serial.println(curr_card);
      arrayOfPasswords[5-card_number] = curr_card;
      Serial.println("Doesnt Crash");
      //maybe turn yellow led on when ever card passed !
      card_number--;
      Serial.println(card_number);
      strcpy(data,"");//to clear data
    }
    
    if(DEBUG.getButtonState() || card_number==1)//end of config
    {
      lcd.clrscr(WHITE);
      CONFIGURATION_MODE = false;
      Auto.drawButton(3);//drawign buttons after conf
      Manual.drawButton(3);
    }
  }
  if(ControlType && modeChosen)//this is for the MANUAL screen
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
         CloseGate();
         lcd.clrscr(WHITE);
         uploadToLCD(20,50,BLACK,WHITE,2,"Choose a Control Setting");
         Auto.drawButton(3);
         Manual.drawButton(3);
     }
   }
   else if (!ControlType && modeChosen)//this is for the AUTO screen ||| 0 is auto and 1 is manual
   {
      Back.updateButtonState();
      readMagCard();
      AutoGate();
      if(Back.getButtonState())
      {
          modeChosen=false;
          CloseGate();
          lcd.clrscr(WHITE);
          uploadToLCD(20,50,BLACK,WHITE,2,"Choose a Control Setting");
          Auto.drawButton(3);
          Manual.drawButton(3);
          firstEnter=true;
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
   

void printLoadScreen(bool state)//this prints the load status of the gate when it is beign opened and closed
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
    ControlType=true;// manual is "1" (true) by defualt
}



void AutoPressed()
{
     modeChosen=true;
     ControlType=false;// auto is "0" (false) by defualt
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
  gateStatus=true;
}

void CloseGate()
{  
  for(pos = Gate.read();pos<=75;pos++)
  {
    Gate.write(pos);
    delay(15);
  }
  gateStatus=false;
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


void SuffDistance()
{
  float duration=0 , distance=0;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = duration*0.017;
  Serial.print("Distance is: ");
  Serial.println(distance);
  if(distance<30.0)//this is the minimal distance to close the gate after car pass.
  {
    SuffDistance_state = true;
  }
  else
  {
    SuffDistance_state = false;
  }
  delay(100);
}

void drawGateOpen()
{
  
}

void DrawGateClose()
{
  
}

void AutoGate()
{
  if(firstEnter)
  {
      CloseState_nopress.drawButton(3);
      uploadToLCD(60,130,WHITE,RED,2,"Swipe your Card");
      firstEnter = false;
  }
  SuffDistance();
  if(!gateStatus && strcmp(data,ADMIN_CODE)==0)
  {
    OpenGate();
    EnterState_nopress.drawButton(3);
    uploadToLCD(65,130,WHITE,GREEN,2,"Please Enter...");
    Serial.println(data);
    strcpy(data,"");
  }
  else
  {
      if(SuffDistance_state && gateStatus)
      {
        CloseState_nopress.drawButton(3);
        uploadToLCD(60,130,WHITE,RED,2,"Swipe your Card");
        CloseGate();
      }
  }
}

bool checkPassword(char* password)
{
  for(int i=0;i<CARD_AMOUNT;i++)
  {
    if(strcmp(password,arrayOfPasswords[i])==0)
      return true;
  }
  return false;
}

