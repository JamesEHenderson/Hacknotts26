#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

byte triggerPin = 3;
byte echoCount = 1;
byte echoPins[] = {2};

LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Morse Code Settings ---
const int thresholdDistance = 15;        // cm (Below this is "pressed", above is "released")
const unsigned long dotDuration = 300;   // ms (Less than this is a dot, more is a dash)
const unsigned long letterTimeout = 700; // ms (Time hand must be away to finish the letter)

bool isPressed = false;
unsigned long stateChangeTime = 0;
String currentMorse = "";
int cursorCol = 0;
int cursorRow = 0;

// Lookup table for A-Z
const char* morseTable[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup() {
    Serial.begin(9600);
    HCSR04.begin(triggerPin, echoPins, echoCount);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
}

// Function to match the dots/dashes to a letter
char decodeMorse(String morse) {
    for (int i = 0; i < 26; i++) {
        if (morse == morseTable[i]) {
            return (char)('A' + i); // Calculate letter based on array index
        }
    }
    return '?'; // Return a question mark if the pattern is invalid
}

void loop() {
    double* distances = HCSR04.measureDistanceCm();
    double dist = distances[0];
    unsigned long currentMillis = millis();

    // Ignore extreme/error readings from the sensor
    if (dist <= 0 || dist > 400) {
        delay(30);
        return; 
    }

    if (dist < thresholdDistance) {  // Small distance = Pressed
        if (!isPressed) {
            isPressed = true;
            stateChangeTime = currentMillis; // Record when press started
        }
    } else { // Large distance = Released
        if (isPressed) {
            isPressed = false;
            unsigned long duration = currentMillis - stateChangeTime;
            
            // CHECK TIME: Ignore tiny glitches (< 50ms)
            if (duration > 50) { 
                if (duration <= dotDuration) {
                    currentMorse += ".";
                } else {
                    currentMorse += "-";
                }
            }
            stateChangeTime = currentMillis; // Record when release started
        } else {
            // If released and enough time has passed, decode the letter
            if (currentMorse.length() > 0 && (currentMillis - stateChangeTime) > letterTimeout) {
                char decodedLetter = decodeMorse(currentMorse);
                
                // OUTPUT NOTHING BUT THE LETTER
                lcd.print(decodedLetter);
                Serial.println(dist);
                
                // Manage LCD screen wrapping
                cursorCol++;
                if (cursorCol >= 16) {
                    cursorCol = 0;
                    cursorRow = (cursorRow == 0) ? 1 : 0;
                    if (cursorRow == 0) lcd.clear(); // Clear if wrapping back to the top left
                    lcd.setCursor(cursorCol, cursorRow);
                }
                
                currentMorse = ""; // Reset for the next letter
            }
        }
    }
    
    // Short delay for sensor stability (allows sensor to process echos)
    delay(30); 
}