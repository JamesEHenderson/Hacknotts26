#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const uint8_t TRIG_PIN = 3;
const uint8_t ECHO_PIN = 2;
const float BLOCK_CM = 10.0;

const int STEPS_PER_REV = 2048;
const int SWING_STEPS = 512;
const int MOTOR_RPM = 12;
const unsigned long MOVE_MS = 60000UL / MOTOR_RPM * SWING_STEPS / STEPS_PER_REV;

const unsigned long UNIT = 300; 

const uint8_t MAX_MSG = 32;

LiquidCrystal_I2C lcdOut(0x27, 16, 2);

const char* const MORSE_LETTERS[26] = {
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
"-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
"..-", "...-", ".--", "-..-", "-.--", "--.."
};
const char* const MORSE_DIGITS[10] = {
"-----", ".----", "..---", "...--", "....-",
".....", "-....", "--...", "---..", "----."
};

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

bool flagSeen() {
static unsigned long lastPing = 0;
static bool last = false;
if (millis() - lastPing < 50) return last;
lastPing = millis();
float d = readDistanceCm();
last = (d > 0 && d < BLOCK_CM);
return last;
}

bool blocked = false;
bool rawPrev = false;

unsigned long lastChange = 0;
unsigned long edgeTime = 0;

String rxPattern = "";
String received = "";
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

unsigned long minusMove(unsigned long t) {
return t > MOVE_MS ? t - MOVE_MS : 0;
}

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

void setup() {
Serial.begin(9600);
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

lcdOut.init();
lcdOut.backlight();
printLine(lcdOut, 0, "HACKNOTTS26");
printLine(lcdOut, 1, "Waiting...");

Serial.println(F("Receiver ready."));
}

void loop() {
updateReceiver();
}