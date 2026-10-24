
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const uint8_t IR_PIN        = 2;
const uint8_t SERVO_PIN     = 9; 
const bool    IR_ACTIVE_LOW = true; 

const int ANGLE_UP   = 0; 
const int ANGLE_DOWN = 90;

const unsigned long UNIT = 300; 


// chaarctyer limit = 32 (temp)//
const uint8_t MAX_MSG = 32;

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

// sender //
String toSend = "";
String toReceieve = "";
enum SendState {S_IDLE , S_MARK , S_GAP};
SendState sState = S_IDLE;

const char* cutPattrn = nullptr;
uint8_t elemIdx = 0;

unsigned long sTimer = 0 ;
unsigned long sDuration = 0 ;

//receiever//

bool blocked = false;
bool RawPrev = false;

unsigned long lastChange = 0;
unsigned long lastEdge = 0;

String rxPattern = "";
String Recieved = "";
bool spaceAdded = true ;

void showOutput() {
  String msg = received.length() > MAX_MSG ? received.substring(received.length() - MAX_MSG) : received;
  printLine(lcdOut, 0, msg.substring(0, 16));
  printLine(lcdOut, 1, msg.length() > 16 ? msg.substring(16) : "");
}

void addReceived(char c) {
  received += c;
  if (received.length() > MAX_MSG) received.remove(0, received.length() - MAX_MSG);
  Serial.print(c);
  showOutput();
}

void setup() {}
void loop() {}