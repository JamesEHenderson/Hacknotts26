#include <Arduino.h>
#line 1 "C:\\Users\\TheGo\\Documents\\OtherRandomThings\\Hacknotts26\\Reciever\\Reciever.ino"
#line 1 "C:\\Users\\TheGo\\Documents\\OtherRandomThings\\Hacknotts26\\Reciever\\Reciever.ino"
void setup();
#line 6 "C:\\Users\\TheGo\\Documents\\OtherRandomThings\\Hacknotts26\\Reciever\\Reciever.ino"
void loop();
#line 1 "C:\\Users\\TheGo\\Documents\\OtherRandomThings\\Hacknotts26\\Reciever\\Reciever.ino"
void setup()
{
	Serial.begin(9600);
}

void loop()
{
    Serial.print("Hello world\n");
    delay(500);	
}

