
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Common I2C addresses are 0x27 and 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Hello, world!");

    lcd.setCursor(0, 1);
    lcd.print("Hello World");
}

void loop() {
    // Nothing needs to happen repeatedly
}
