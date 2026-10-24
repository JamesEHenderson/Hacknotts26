# 1 "C:\\Users\\TheGo\\Documents\\OtherRandomThings\\Hacknotts26\\Reciever\\Reciever.ino"
void setup()
{
 Serial0.begin(9600);
}

void loop()
{
    Serial0.print("Hello world\n");
    delay(500);
}
