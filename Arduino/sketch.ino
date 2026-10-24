
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const uint8_t IR_PIN        = 2;
const uint8_t SERVO_PIN     = 9; 
const bool    IR_ACTIVE_LOW = true; 

const int ANGLE_UP   = 0; 
const int ANGLE_DOWN = 90;

const unsigned long UNIT = 300; 
LiquidCrystal_I2C lcdIn(0x27, 16, 2);
LiquidCrystal_I2C lcdOut(0x3F, 16, 2);

Servo flag;

const char* const MORSE_LETTERS[26] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};
const char* const MORSE_DIGITS[10] = {
  "-----", ".----", "..---", "...--", "....-",
  ".....", "-....", "--...", "---..", "----."
};

const char* encodeChar(char c) {
  if (c >= 'A' && c <= 'Z') return MORSE_LETTERS[c - 'A'];
  if (c >= '0' && c <= '9') return MORSE_DIGITS[c - '0'];
  return nullptr;
}

char decodePattern(const String& p) {
  for (uint8_t i = 0; i < 26; i++) {
    if (p == MORSE_LETTERS[i]) return 'A' + i;
  }
  for (uint8_t i = 0; i < 10; i++) {
    if (p == MORSE_DIGITS[i]) return '0' + i;
  }
  return '?';
}

void printLine(LiquidCrystal_I2C& lcd, uint8_t row, const String& text) {
  lcd.setCursor(0, row);
  for (uint8_t i = 0; i < 16; i++) {
    lcd.print(i < text.length() ? text[i] : ' ');
  }
}



void setup() {}
void loop() {}