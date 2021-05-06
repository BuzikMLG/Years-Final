
//===============================================
//                  testTouch
//===============================================
byte testTouch(){
byte i;
for(i=0; i<3 ; i++){
 if(x > 20+100*i && x<100+100*i && y>80 && y<80+80){
      
     tone(speakerPin,toneArr[i]);
     delay(350);
      noTone(speakerPin);
      return i;
     }
   }
 return 0xff; 
}//End of testTouch function
//===============================================
//         End Of File "print"        
//===============================================  

