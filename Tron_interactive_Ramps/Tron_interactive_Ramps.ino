//Eric Lyons 2015
//Note to self:  Play more pinball!
//Interfaced for pinduino shield v0.2
//Uses pinduino library


#include <pinduino.h>
int N_LEDS = 150;
int aLEDNum1 = 120;
int aLEDNum2 = 120;
int aLEDNum3 = 1;

pinduino pd (aLEDNum1, aLEDNum2, aLEDNum3);


int bg_chase_on = 1;
int disc_motor = 0; // flag for sensing disc motor
int disc_motor_wait = 10000;//time to wait with no disc motor activity to reset disc_motor_flag
int zuse = 0; // flag for sensing zuse
int zuse_wait = 500;//time to wait with no zuse activity to reset zuse flag
unsigned long timeLastEvent = 0; // time last event was last triggered
int startChaseWaitTime = 30000; //Amount of time to wait before chase lights start up again 1000 == 1 second
int bgTime = 50; // time till lights are set to background color
String chaseColor = "red"; // var to hold bg chase color

void setup() {
  Serial.begin(115200);
  pd.adrLED1()->clear();
  pd.adrLED2()->clear();
  pd.adrLED3()->clear();
  pd.pinState()->reset();
}

void loop(){
//  if (bg_chase_on){backgroundChase();}
  for (int i=0; i<1000; i=i+1) {     
    pd.pinState()->update();
  }
//   Print the pin states out to serial 
  pd.pinState()->print();
  checkPinStates();
  if (millis()-timeLastEvent > startChaseWaitTime) {bg_chase_on=1;}
  if (millis()-timeLastEvent > bgTime) {
    pd.adrLED1()->clear();
    pd.adrLED2()->clear();
  }
  if (!pd.pinState()->J6(6) && millis()-timeLastEvent > disc_motor_wait) { disc_motor=0;}
  if ( millis()-timeLastEvent > zuse_wait) { zuse=0;}

}

void checkPinStates(){
  int trigger =0;
  if ( pd.pinState()->J6(1) && pd.pinState()->J7(4) && pd.pinState()->J7(8) && pd.pinState()->J7(9) && 
      !pd.pinState()->J6(2) && !pd.pinState()->J6(3) && !pd.pinState()->J6(7) && !pd.pinState()->J6(8) ) { // light cycle
    pd.adrLED2()->chase2Color("yellow", "yellow", 25, 0, 1);
    pd.adrLED1()->chase2Color("blue", "blue", 25, 0, 1);
    trigger =1;    
  }

  if ( !zuse && pd.pinState()->J6(4) && ! pd.pinState()->J7(4) && ! pd.pinState()->J7(8) && ! pd.pinState()->J7(9) ) { // zuse
    pd.adrLED1()->clear();
    pd.adrLED2()->clear();
    delay(30);
    for (int i=0; i<3; i=i+1) {
      pd.adrLED1()->color("blue", 255);
      pd.adrLED2()->color("blue", 255);
      delay(30);
      pd.adrLED1()->clear();
      pd.adrLED2()->clear();
      delay(30);
    }  
    trigger =1;
    zuse = 1;
   }
   
   if ( pd.pinState()->J6(2) && !pd.pinState()->J6(3) ){//quora
//    pd.adrLED1()->color("red", 255);
//    Serial.print( pd.pinState()->J6(2));
//    Serial.print (" ");
//    Serial.println( pd.pinState()->J6(3));
    pd.adrLED1()->color("green", 1);    
    pd.adrLED2()->color("green", 1);
    pd.fadeInAllAdr(100);    
    delay(300);
    pd.fadeOutAllAdr(100);
    trigger = 1; 
   }

   if (pd.pinState()->J6(2) && pd.pinState()->J6(3) && pd.pinState()->J6(7) && pd.pinState()->J6(8) 
     && ! pd.pinState()->J7(4) && ! pd.pinState()->J7(8) && ! pd.pinState()->J7(9) ) {// disc open and hit
    for (int i=0; i<2; i=i+1) {     
      pd.adrLED1()->clear();
      pd.adrLED2()->clear();
      delay(50);
      pd.adrLED1()->color("blue",255);
      pd.adrLED2()->color("red",255);
      delay(50);
      pd.adrLED1()->clear();
      pd.adrLED2()->clear();
      delay(50);
      pd.adrLED1()->color("red",255);
      pd.adrLED2()->color("blue",255);
      delay(50);
      trigger = 1;
    }    
  } 
  
  if ( pd.pinState()->J6(5) ){ // recognizer
    pd.adrLED1()->color("red", 255);
    pd.adrLED2()->color("red", 255);
    pd.fadeOutAllAdr(100);
    trigger = 1; 
  }
  if ( pd.pinState()->J6(6) && !disc_motor){ // Disc motor
    //want this to run when the disc starts up
    pd.adrLED1()->chase2Color("red", "red", 10, 10, 1);
    pd.adrLED1()->chase2Color("red", "red", 10, 10, -1);
    pd.adrLED2()->chase2Color("blue", "blue", 10, 10, 1);
    pd.adrLED2()->chase2Color("blue", "blue", 10, 10, -1);
    pd.adrLED1()->color("red", 1);
    pd.adrLED2()->color("blue", 1);
    pd.fadeInAllAdr(255);
    delay (300);
    pd.fadeOutAllAdr(255);
    disc_motor=1;
    trigger = 1; 
  }
//  if ( pd.pinState()->J6(6) ) { pd.pinState()->resetJ6(6);}
  if ( pd.pinState()->J6(7) ){ //RED disc left (flashers under disc)
  }
  if ( pd.pinState()->J6(8) ){ //RED disc right (flashers under disc)
  }
  
//trigger is to take care of any cleanup after a sequence has been triggered.
  if (trigger) {
   pd.pinState()->reset();
   trigger =0;
   bg_chase_on = 0;
   timeLastEvent = millis();
  }
}



void backgroundChase() {
  }

