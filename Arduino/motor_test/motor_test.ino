#include <Stepper.h>

// motor test - just the stepper, no screens or sensor //
// type a letter in the Serial Monitor and the flag taps it in morse //

// stepper motor (28BYJ-48 + ULN2003) //
const int STEPS_PER_REV = 2048;
const int SWING_STEPS = 512;
const int MOTOR_RPM = 12;

// morse speed (dot = 1 unit, dash = 3) //
const unsigned long UNIT = 300;

Stepper flag(STEPS_PER_REV, 8, 10, 9, 11);

// morse tables //
const char* const MORSE_LETTERS[26] = {
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
"-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
"..-", "...-", ".--", "-..-", "-.--", "--.."
};
const char* const MORSE_DIGITS[10] = {
"-----", ".----", "..---", "...--", "....-",
".....", "-....", "--...", "---..", "----."
};

// letter -> morse //
const char* encodeChar(char c) {
if (c >= 'A' && c <= 'Z') return MORSE_LETTERS[c - 'A'];
if (c >= '0' && c <= '9') return MORSE_DIGITS[c - '0'];
return nullptr;
}

// move the flag 90 degrees //
void flagDown() {
flag.step(SWING_STEPS);
}

void flagUp() {
flag.step(-SWING_STEPS);
}

// tap out one letter //
void sendLetter(char c) {
const char* p = encodeChar(c);
if (!p) return;

Serial.print(c);
Serial.print(" = ");
Serial.println(p);

for (uint8_t i = 0; p[i] != '\0'; i++) {
flagDown();
delay(p[i] == '.' ? UNIT : 3 * UNIT);
flagUp();
delay(UNIT);
  }
delay(2 * UNIT);
}

// start up //
void setup() {
Serial.begin(9600);
flag.setSpeed(MOTOR_RPM);
Serial.println(F("Motor test ready. Type a letter or word and press Enter."));
Serial.println(F("Type ! to just swing once."));
}

// runs forever //
void loop() {
if (Serial.available()) {
char c = toupper(Serial.read());
if (c == '!') {
Serial.println(F("Swing down..."));
flagDown();
delay(1000);
Serial.println(F("Swing up..."));
flagUp();
    } else if (c == ' ') {
delay(4 * UNIT);
    } else {
sendLetter(c);
    }
  }
}