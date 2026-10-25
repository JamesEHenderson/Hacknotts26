#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

byte echoPins[] = {2};
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Settings & State Variables ---
const uint8_t thresholdDist = 20;
const unsigned long dotLen = 300, letterGap = 700, wordGap = 1100, clearHold = 3000;

// --- State Tracking Variables ---
bool isPressed = false;
bool spacePrinted = true;
bool displayCleared = false;
unsigned long stateChangeTime = 0;
String currentMorse = "";
uint8_t cursorCol = 0, cursorRow = 0;


// Lookup table mapping Morse code sequences to the alphabet (A-Z)
// Not in ASCII: #, %, *, <, >, [, ], \, ^
const char* morseTable[] = {
  "-.-.--", // ! (ASCII 33)
  ".-..-.", // " (ASCII 34)
  "",       // # (ASCII 35) - NO MORSE
  "...-..-",// $ (ASCII 36)
  "",       // % (ASCII 37) - NO MORSE
  ".-...",  // & (ASCII 38)
  ".----.", // ' (ASCII 39)
  "-.--.",  // ( (ASCII 40)
  "-.--.-", // ) (ASCII 41)
  "",       // * (ASCII 42) - NO MORSE
  ".-.-.",  // + (ASCII 43)
  "--..--", // , (ASCII 44)
  "-....-", // - (ASCII 45)
  ".-.-.-", // . (ASCII 46)
  "-..-.",  // / (ASCII 47)
  "-----",  // 0 (ASCII 48)
  ".----",  // 1 (ASCII 49)
  "..---",  // 2 (ASCII 50)
  "...--",  // 3 (ASCII 51)
  "....-",  // 4 (ASCII 52)
  ".....",  // 5 (ASCII 53)
  "-....",  // 6 (ASCII 54)
  "--...",  // 7 (ASCII 55)
  "---..",  // 8 (ASCII 56)
  "----.",  // 9 (ASCII 57)
  "---...", // : (ASCII 58)
  "-.-.-.", // ; (ASCII 59)
  "",       // < (ASCII 60) - NO MORSE
  "-...-",  // = (ASCII 61)
  "",       // > (ASCII 62) - NO MORSE
  "..--..", // ? (ASCII 63)
  ".--.-.", // @ (ASCII 64)
  ".-",     // A (ASCII 65)
  "-...",   // B (ASCII 66)
  "-.-.",   // C (ASCII 67)
  "-..",    // D (ASCII 68)
  ".",      // E (ASCII 69)
  "..-.",   // F (ASCII 70)
  "--.",    // G (ASCII 71)
  "....",   // H (ASCII 72)
  "..",     // I (ASCII 73)
  ".---",   // J (ASCII 74)
  "-.-",    // K (ASCII 75)
  ".-..",   // L (ASCII 76)
  "--",     // M (ASCII 77)
  "-.",     // N (ASCII 78)
  "---",    // O (ASCII 79)
  ".--.",   // P (ASCII 80)
  "--.-",   // Q (ASCII 81)
  ".-.",    // R (ASCII 82)
  "...",    // S (ASCII 83)
  "-",      // T (ASCII 84)
  "..-",    // U (ASCII 85)
  "...-",   // V (ASCII 86)
  ".--",    // W (ASCII 87)
  "-..-",   // X (ASCII 88)
  "-.--",   // Y (ASCII 89)
  "--..",   // Z (ASCII 90)
  "",       // [ (ASCII 91) - NO MORSE
  "",       // \ (ASCII 92) - NO MORSE
  "",       // ] (ASCII 93) - NO MORSE
  "",       // ^ (ASCII 94) - NO MORSE
  "..--.-"  // _ (ASCII 95)
};

void setup() {
    HCSR04.begin(3, echoPins, 1); // triggerPin = 3, echoCount = 1
    lcd.init();
    lcd.backlight();
    lcd.blink(); // Blinking cursor shows the active input cell on the LCD
}

// Scans the lookup table and returns the matching character, or '?' if invalid
char decodeMorse(String m) {
    for (uint8_t i = 0; i < 63; i++) {
        if (morseTable[i] != "" && m == morseTable[i]) return '!' + i;
    }
    return '?';
}

// Live-updates the LCD with the current Morse translation before the letter is finalized
void previewLetter() {
    lcd.setCursor(cursorCol, cursorRow);
    lcd.print(decodeMorse(currentMorse));
}

// Handles LCD grid math: wraps to the next line or clears the screen if full
void advanceCursor() {
    if (++cursorCol >= 16) {
        cursorCol = 0;
        cursorRow = !cursorRow; // Toggle between top (0) and bottom (1) row
        if (cursorRow == 0) lcd.clear(); // Screen is full, wipe and start at top-left
    }
    lcd.setCursor(cursorCol, cursorRow);
}

void loop() {
    double dist = HCSR04.measureDistanceCm()[0];
    unsigned long now = millis();
    unsigned long elapsed = now - stateChangeTime;

    // Filter out invalid sensor readings (out of bounds or zero)
    if (dist <= 0 || dist > 600) { delay(30); return; }

    if (dist < thresholdDist) { // --- Sensor Covered ---
        if (!isPressed) { // Sensor covered start timer
            isPressed = true;
            stateChangeTime = now; 
            displayCleared = false;
        } else if (!displayCleared && elapsed > clearHold) { // Sensor covered for 3 seconds - reset screen.
            lcd.clear();
            cursorCol = cursorRow = 0;
            currentMorse = ""; 
            displayCleared = true;
        }    
    } else { // --- UNCOVERED ---
        if (isPressed) {
            isPressed = false;
            
            // Ignore tiny fluctuations (<50ms) to prevent phantom inputs
            if (!displayCleared && elapsed > 50) {
                // Append a dot or dash based on how long the sensor was covered
                currentMorse += (elapsed <= dotLen) ? "." : "-";
                previewLetter(); // Instantly show the updated translation guess
            }
            stateChangeTime = now; // Restart timer to measure the gap between presses
        } else {
            if (currentMorse.length() > 0 && elapsed > letterGap) { // Gap > 0.7 seconds - letter complete
                advanceCursor();
                currentMorse = ""; 
                spacePrinted = false; // Allow a space to be printed next if the gap continues
            } else if (currentMorse.length() == 0 && !spacePrinted && elapsed > wordGap) { // Gap is > 2 seconds - print space
                lcd.print(" ");
                advanceCursor();
                spacePrinted = true; // Prevent printing endless spaces
            }
        }
    }
    delay(30); // Short delay to allow ultrasonic echoes to dissipate 
}