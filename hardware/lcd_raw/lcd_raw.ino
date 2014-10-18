#include "Wire.h"
#include "LiquidCrystal.h"

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidCrystal lcd(0);

void setup() {
  // set up the LCD's number of rows and columns: 
   lcd.begin(16, 2);

  lcd.setBacklight(HIGH);
  lcd.print("hello, world 0 !");
  lcd.setCursor(0, 1);
  lcd.print("hello, world 1 !");

}

void loop() {
 
}
