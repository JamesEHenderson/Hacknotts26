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
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup()
{
	Serial.begin(9600);
    servo.attach(SIGNAL_PIN);
    BT.begin("Esp32-BT");
    Serial.print("Enter some text! :\n");
}


String serialInput(){
    
    while(Serial.available() == 0);//Block until any input

    String msg = Serial.readStringUntil('\n');
    msg.trim();
    return msg;
}

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
   

    int i = 0, j = 0;
    
    char c = '\0';
    int morse_index = 0;

    const int CHAR_DELAY = 700; //The delay in ms between each symbol
    const int WORD_DELAY = 1100; //The delay in ms to denote a space
    const int DOT_DELAY = 300; //The delay in ms to denote a dot
    const int DASH_DELAY = 600; //The delay in ms to denote a dash


    flagUp();

    txt = bluetoothInput();
    

    String morse_code = "";


    for(c=txt[i] ; c != '\0'; i++,  c = txt[i]){
        c = tolower(c);

        if(c == '0'){
            Serial.printf("Clear pressed\n");
            resetLCD();
            break;
        }
        
        //Lookup each char in morse code    
        morse_index = c - 'a';
        morse_code = morseTable[morse_index];

        
        
        //Move servo according to morse code symbol
        Serial.printf("Char:'%c', combo:", c);
        for(j = 0; morse_code[j] != '\0'; j++){
            
            flagDown();
            if(morse_code[j] == '.'){
                Serial.printf("dot, ");
                delay(DOT_DELAY);
    
            }else if(morse_code[j] == '-'){
                Serial.printf("dash, ");
                delay(DASH_DELAY);

            }

            flagUp();
            delay(200);

        }
        Serial.printf("\n");
        delay(CHAR_DELAY);
        

       
    }
    

    // Serial.print("One direction\n");
    // servo.write(90);
    // delay(700);
    // servo.write(180);
    // delay(700);
    
    
}
