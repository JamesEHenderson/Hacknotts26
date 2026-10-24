#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

byte triggerPin = 3;
byte echoCount = 1;
byte echoPins[] = {2};

// Common I2C addresses are 0x27 and 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);
    HCSR04.begin(triggerPin, echoPins, echoCount);

    lcd.init();
    lcd.backlight();
}

void loop() {
    // The HCSR04 library returns a pointer to an array of doubles
    double* distances = HCSR04.measureDistanceCm();

    // Clear the display before writing new data
    lcd.clear();
    
    // Set cursor to the first column, first row
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    
    // Set cursor to the first column, second row
    lcd.setCursor(0, 1);
    lcd.print(distances[0]);
    lcd.print(" cm");

    delay(500);
}