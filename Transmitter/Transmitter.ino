#include "ESP32Servo.h"

#define SIGNAL_PIN 18

Servo servo;

void setup()
{
	Serial.begin(9600);
    servo.attach(SIGNAL_PIN);

}


void loop()
{
    Serial.print("One direction\n");
    servo.write(90);
    delay(700);
    
    Serial.print("Other direction\n");
    servo.write(170);
    delay(200);

    Serial.print("One direction\n");
    servo.write(90);
    delay(700);

    Serial.print("Other direction\n");
    servo.write(170);
    delay(200);

    Serial.print("One direction\n");
    servo.write(90);
    delay(500);
}
