void rtc(){
   delay(500);
    lcd.print(digitalRead(2));
  do{
    readTime();
     while(digitalRead(Tout)==0);
     lcd.gotoxy(38,110);
     lcd.setFont(5);
     lcd.print(hour/10,DEC);
     lcd.print(hour%10,DEC);
     lcd.print(":");
     lcd.print(minute/10,DEC);
     lcd.print(minute%10,DEC);
     lcd.print(":");
     lcd.print(second/10,DEC); 
     lcd.print(second%10,DEC);
      delay (700);
  }while(digitalRead(2)==0);

}

