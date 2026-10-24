#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

byte echoPins[] = {2};
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Settings & State Variables ---
const int thresholdDist = 15; 
const unsigned long dotLen = 300, letterGap = 700, wordGap = 2000, clearHold = 3000;

bool isPressed = false, spacePrinted = true, displayCleared = false;
unsigned long stateChangeTime = 0;
String currentMorse = "";
int cursorCol = 0, cursorRow = 0;

const char* morseTable[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup() {
    HCSR04.begin(3, echoPins, 1); // triggerPin = 3, echoCount = 1
    lcd.init();
    lcd.backlight();
}

char decodeMorse(String m) {
    for (int i = 0; i < 26; i++) {
        if (m == morseTable[i]) return 'A' + i;
    }
    return '?';
}

void advanceCursor() {
    if (++cursorCol >= 16) {
        cursorCol = 0;
        cursorRow = !cursorRow;         // Toggle between row 0 and 1
        if (cursorRow == 0) lcd.clear(); // Clear when returning to top-left
    }
    lcd.setCursor(cursorCol, cursorRow);
}

void loop() {
    double dist = HCSR04.measureDistanceCm()[0];
    unsigned long now = millis();
    unsigned long elapsed = now - stateChangeTime;

    if (dist <= 0 || dist > 400) { delay(30); return; } // Ignore errors

    if (dist < thresholdDist) { // --- COVERED ---
        if (!isPressed) {
            isPressed = true;
            stateChangeTime = now; 
            displayCleared = false;
        } else if (!displayCleared && elapsed > clearHold) { // Long hold to clear
            lcd.clear();
            cursorCol = cursorRow = 0;
            currentMorse = ""; 
            displayCleared = true;
        }
    } else { // --- UNCOVERED ---
        if (isPressed) {
            isPressed = false;
            // Record dot or dash if not clearing the screen (ignore <50ms glitches)
            if (!displayCleared && elapsed > 50) {
                currentMorse += (elapsed <= dotLen) ? "." : "-";
            }
            stateChangeTime = now;
        } else {
            // Check timeouts for letters and spaces
            if (currentMorse.length() > 0 && elapsed > letterGap) {
                lcd.print(decodeMorse(currentMorse));
                advanceCursor();
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