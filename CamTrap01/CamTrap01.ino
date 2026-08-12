#include "Arduino.h"
#include <TM1637Display.h>

// all_on pins connected to the TM1637 display
const byte CLK_PIN = 6;
const byte DIO_PIN = 5;

// Create display object of type TM1637Display:
TM1637Display cam_display = TM1637Display(CLK_PIN, DIO_PIN);

void setup() {
  cam_display.setBrightness(7);  // Configure the display brightness (0-7):
}

void loop() {
  // Clear the display (all segments off)
  cam_display.clear();
  delay(1000);

  for (int i = -100; i <= 100; i++) {
  cam_display.showNumberDec(i);
  delay(50);
  }
  delay(1000);

}
