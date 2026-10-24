#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <HCSR04.h>

// ultrasonic sensor //
const uint8_t TRIG_PIN = 3;
const uint8_t ECHO_PIN = 2;
const float BLOCK_CM = 10.0;

// stepper motor (28BYJ-48 + ULN2003) //
const int STEPS_PER_REV = 2048;
const int SWING_STEPS = 512;
const int MOTOR_RPM = 12;
const unsigned long MOVE_MS = 60000UL / MOTOR_RPM * SWING_STEPS / STEPS_PER_REV;

// morse speed (dot = 1 unit, dash = 3) //
const unsigned long UNIT = 300; 


// chaarctyer limit = 32 (temp)//
const uint8_t MAX_MSG = 32;

LiquidCrystal_I2C lcdIn(0x27, 16, 2);
LiquidCrystal_I2C lcdOut(0x3F, 16, 2);

Stepper flag(STEPS_PER_REV, 8, 10, 9, 11);

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

// read distance in cm (-1 = nothing there) //
float readDistanceCm() {
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);
unsigned long us = pulseIn(ECHO_PIN, HIGH, 6000);
if (us == 0) return -1;
return us / 58.0;
}

// is the flag in front of the sensor (pings every 50ms) //
bool flagSeen() {
static unsigned long lastPing = 0;
static bool last = false;
if (millis() - lastPing < 50) return last;
lastPing = millis();
float d = readDistanceCm();
last = (d > 0 && d < BLOCK_CM);
return last;
}

// sender //
String toSend = "";
String serialLine = "";
enum SendState {S_IDLE , S_MARK , S_GAP};
SendState sState = S_IDLE;

const char* curPattern = nullptr;
uint8_t elemIdx = 0;

unsigned long sTimer = 0 ;
unsigned long sDuration = 0 ;

// move the flag 90 degrees //
void flagDown() {
flag.step(SWING_STEPS);
}

void flagUp() {
flag.step(-SWING_STEPS);
}

// show whats left to send //
void showInput() {
printLine(lcdIn, 0, toSend.substring(0, 16));
printLine(lcdIn, 1, toSend.length() > 16 ? toSend.substring(16) : "");
}

// start a dot or dash //
void startElement() {
flagDown();
sState = S_MARK;
sDuration = (curPattern[elemIdx] == '.') ? UNIT : 3 * UNIT;
sTimer = millis();
}

// start the next letter (or a space) //
void startNextChar() {
char c = toSend[0];

if (c == ' ') {
toSend.remove(0, 1);
curPattern = nullptr;
showInput();
sState = S_GAP;
sDuration = 4 * UNIT;
sTimer = millis();
return;
  }

const char* p = encodeChar(c);
if (!p) {
toSend.remove(0, 1);
showInput();
return;
  }

curPattern = p;
elemIdx = 0;
startElement();
}

// timing for dots, dashes and gaps //
void updateSender() {
if (sState != S_IDLE && millis() - sTimer < sDuration) return;

switch (sState) {
case S_IDLE:
if (toSend.length() > 0) startNextChar();
break;

case S_MARK:
flagUp();
elemIdx++;
sState = S_GAP;
sTimer = millis();
if (curPattern[elemIdx] != '\0') {
sDuration = UNIT;
      } else {
sDuration = 3 * UNIT;
toSend.remove(0, 1);
curPattern = nullptr;
showInput();
      }
break;

case S_GAP:
if (curPattern) startElement();
else sState = S_IDLE;
break;
  }
}

// read typing from the laptop //
void readKeyboard() {
while (Serial.available()) {
char c = Serial.read();
if (c == '\n' || c == '\r') {
if (serialLine.length() > 0) {
serialLine.toUpperCase();
if (toSend.length() + serialLine.length() + 1 > MAX_MSG) {
Serial.println(F("Too long! Max 32 characters."));
serialLine = "";
return;
        }
if (toSend.length() > 0) toSend += ' ';
toSend += serialLine;
Serial.print(F("Sending: "));
Serial.println(serialLine);
serialLine = "";
showInput();
      }
    } else if (serialLine.length() < MAX_MSG) {
serialLine += c;
    }
  }
}

//receiever//

bool blocked = false;
bool rawPrev = false;

unsigned long lastChange = 0;
unsigned long edgeTime = 0;

String rxPattern = "";
String received = "";
bool spaceAdded = true ;

// show received message //
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

// take off the time the motor spent moving //
unsigned long minusMove(unsigned long t) {
return t > MOVE_MS ? t - MOVE_MS : 0;
}

// watch the sensor and decode letters //
void updateReceiver() {
unsigned long now = millis();
bool raw = flagSeen();

if (raw != rawPrev) {
rawPrev = raw;
lastChange = now;
  }

if (now - lastChange > 20 && raw != blocked) {
unsigned long duration = minusMove(now - edgeTime);
blocked = raw;
edgeTime = now;
if (!blocked) {
rxPattern += (duration < 2 * UNIT) ? '.' : '-';
if (rxPattern.length() > 6) rxPattern = "??????";
    }
return;
  }

if (!blocked) {
unsigned long gap = minusMove(now - edgeTime);
if (rxPattern.length() > 0 && gap > 2 * UNIT) {
char c = decodePattern(rxPattern);
rxPattern = "";
spaceAdded = false;
addReceived(c);
    }
if (!spaceAdded && gap > 5 * UNIT) {
spaceAdded = true;
addReceived(' ');
    }
  }
}

// start up //
void setup() {
Serial.begin(9600);
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
flag.setSpeed(MOTOR_RPM);

lcdIn.init();
lcdIn.backlight();
lcdOut.init();
lcdOut.backlight();

printLine(lcdIn, 0, "HACKNOTTS26");
printLine(lcdIn, 1, "Type a message");
printLine(lcdOut, 0, "");
printLine(lcdOut, 1, "");

Serial.println(F("Hacknotts26 Morse Transmitter ready. Max 32 characters."));
}

void loop() {
readKeyboard();
updateSender();
updateReceiver();
}