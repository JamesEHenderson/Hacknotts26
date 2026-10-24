
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const uint8_t IR_PIN        = 2;
const uint8_t SERVO_PIN     = 9; 
const bool    IR_ACTIVE_LOW = true; 

const int ANGLE_UP   = ; 
const int ANGLE_DOWN = ;

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
