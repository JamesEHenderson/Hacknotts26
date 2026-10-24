
#include <HCSR04.h>

byte triggerPin = 3;
byte echoCount = 1;
byte echoPins[] = {2};

void setup() {
  Serial.begin(9600);
  HCSR04.begin(triggerPin, echoPins, echoCount);
}

void loop() {
  double* distances = HCSR04.measureDistanceCm();

  Serial.print("Distance: ");
  Serial.print(distances[0]);
  Serial.println(" cm");

  delay(500);
}
