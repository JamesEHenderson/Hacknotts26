#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

byte echoPins[] = {2};
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Settings & State Variables ---
const uint8_t thresholdDist = 15; 
const unsigned long dotLen = 300, letterGap = 700, wordGap = 2000, clearHold = 3000;

bool isPressed = false, spacePrinted = true, displayCleared = false;
unsigned long stateChangeTime = 0;
String currentMorse = "";
uint8_t cursorCol = 0, cursorRow = 0;


const char* morseTable[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup() {
    HCSR04.begin(3, echoPins, 1);
    lcd.init();
    lcd.backlight();
    lcd.blink();                          // optional: shows the pending cell
}

char decodeMorse(String m) {
    for (uint8_t i = 0; i < 26; i++) {
        if (m == morseTable[i]) return 'A' + i;
    }
    return '?';
}

// Step 1: new helper (must be below decodeMorse)
void previewLetter() {
    lcd.setCursor(cursorCol, cursorRow);
    lcd.print(decodeMorse(currentMorse));
}

void advanceCursor() {
    if (++cursorCol >= 16) {
        cursorCol = 0;
        cursorRow = !cursorRow;
        if (cursorRow == 0) lcd.clear();
    }
    lcd.setCursor(cursorCol, cursorRow);
}

void loop() {
    double dist = HCSR04.measureDistanceCm()[0];
    unsigned long now = millis();
    unsigned long elapsed = now - stateChangeTime;

    if (dist <= 0 || dist > 400) { delay(30); return; }

    if (dist < thresholdDist) { // --- COVERED ---
        if (!isPressed) {
            isPressed = true;
            stateChangeTime = now; 
            displayCleared = false;
        } else if (!displayCleared && elapsed > clearHold) {
            lcd.clear();
            cursorCol = cursorRow = 0;
            currentMorse = ""; 
            displayCleared = true;
        }
        
    } else { // --- UNCOVERED ---
        if (isPressed) {
            isPressed = false;
            if (!displayCleared && elapsed > 50) {
                currentMorse += (elapsed <= dotLen) ? "." : "-";
                previewLetter();          // Step 2: show the letter so far
            }
            stateChangeTime = now;
        } else {
            if (currentMorse.length() > 0 && elapsed > letterGap) {
                advanceCursor();          // Step 3: commit (no lcd.print here anymore)
                currentMorse = ""; 
                spacePrinted = false;
            } else if (currentMorse.length() == 0 && !spacePrinted && elapsed > wordGap) {
                lcd.print(" ");
                advanceCursor();
                spacePrinted = true;
            }
        }
    }
    delay(30); 
}