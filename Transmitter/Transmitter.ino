#include "ESP32Servo.h"
#include "BluetoothSerial.h"

#define SIGNAL_PIN 18

BluetoothSerial BT;
Servo servo;

String bluetoothInput(){

    while(BT.available() == 0);

    String msg = BT.readStringUntil('\n');
    msg.trim();
    return msg;
}

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


void setup(){
	Serial.begin(9600);
    BT.begin("Esp32-BT");
    servo.attach(SIGNAL_PIN);
}

// String serialInput(){
//     while(Serial.available() == 0);//Block until any input

//     String msg = Serial.readStringUntil('\n');
//     msg.trim();
//     return msg;
// }

void flagDown(){
    servo.write(0);
}

void flagUp(){
    servo.write(90);
}

void resetLCD(){
    flagDown();
    delay(3200);
    flagUp();
}

void loop()
{
    String txt = "";
    int i = 0, j = 0, morse_index = 0;
    char c = '\0';

    const int CHAR_DELAY = 700; //The delay in ms between each symbol
    const int WORD_DELAY = 1100; //The delay in ms to denote a space
    const int DOT_DELAY = 300; //The delay in ms to denote a dot
    const int DASH_DELAY = 600; //The delay in ms to denote a dash

    flagUp();

    txt = bluetoothInput();

    String morse_code = "";

    for(c=txt[i] ; c != '\0'; i++,  c = txt[i]){
        c = toupper(c);

        if(c == '#'){//Resets LCD display
            resetLCD();
            break;
        } else if(c == ' '){
            flagUp();
            delay(WORD_DELAY);
            flagDown();
            continue;
        }
        
        //Lookup each char in morse code    
        morse_index = c - '!';
        morse_code = morseTable[morse_index];

        //Move servo according to morse code symbol
        for(j = 0; morse_code[j] != '\0'; j++){
            flagDown();
            if(morse_code[j] == '.'){
                delay(DOT_DELAY);
    
            } else if(morse_code[j] == '-'){
                delay(DASH_DELAY);

            }

            flagUp();
            delay(200);

        }
        delay(CHAR_DELAY);
    }
}
