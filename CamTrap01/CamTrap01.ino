#include "Arduino.h"
#include <TM1637Display.h>

// all_on pins connected to the TM1637 display
const byte CLK_PIN = 6;
const byte DIO_PIN = 5;

const byte CLK_ROT = 2;
const byte DT_ROT = 3;
const byte SW_ROT = 4;

int counter = 0;    // keep a running tally of steps...
int currentStateCLK;
int lastStateCLK;

bool buttonPressed = false;

// Create display object of type TM1637Display:
TM1637Display cam_display = TM1637Display(CLK_PIN, DIO_PIN);

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(CLK_ROT,INPUT);
  pinMode(DT_ROT,INPUT);
  pinMode(SW_ROT,INPUT_PULLUP);

  // Read the initial state of A (CLK)
  lastStateCLK = digitalRead(CLK_ROT);

  cam_display.clear();
  delay(1000);
  cam_display.setBrightness(7);  // Configure the display brightness (0-7):

  attachInterrupt(digitalPinToInterrupt(CLK_ROT), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT_ROT), updateEncoder, CHANGE);
}

void loop() {
  // Clear the display (all segments off)
  // cam_display.clear();
  // delay(1000);

  cam_display.showNumberDec(counter);
  delay(50);

  if (digitalRead(SW_ROT) == LOW){
    Serial.println("Encoder Button Down");
    buttonPressed = true;
    delay(200);
  }
  
  if (buttonPressed && digitalRead(SW_ROT) == HIGH){
    Serial.println("Encoder Button Released");
    buttonPressed = false;
  }
}

void updateEncoder(){
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK_ROT);
 
  // If last and current state of CLK are different, then a pulse occurred;
  // React to only 0->1 state change to avoid double counting
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
 
    // If the DT state is different than the CLK state then
    // the encoder is rotating CW so INCREASE counter by 1
    // my encoder is working opposite of how it was described in the tutorial.  
    // I changed the '!=' to '==' so the code will correspond with the directions
    // I found I can either change the logic here, or swap the pins as well
    if (digitalRead(DT_ROT) == currentStateCLK) {
      //counter ++;
      if (counter >= 5975){
        counter = 6000;
      } else {
        counter = counter + 25;
      }
     
    } else {
      // Encoder is rotating CCW so DECREASE counter by 1
      //counter --;
      if (counter <= 25) {
        counter = 0;
      } else {
        counter = counter - 25;
      }
    }
   }
 
  // Remember last CLK state to use on next interrupt...
  lastStateCLK = currentStateCLK;
}