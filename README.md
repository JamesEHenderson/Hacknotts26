# Hacknotts26 - Morse Code Ultrasonic Transmitter

For Hacknotts26, a 24-Hour Hackathon, we built a Bluetooth text to morse code converter on an ESP32 microcontroller sending the information to an arduino uni to then decode and display on an LCD screen.

[![Watch the Demo Video!](https://www.youtube.com/watch?v=JobzFIudcuk)](https://www.youtube.com/watch?v=JobzFIudcuk)


## Inspiration
We were originally inspired by pager technology in the early 2000s, this then progressed into a different project after we were trying to brainstorm ways to get them to communicate with each other (we decided on morse code) then quickly evolved into our finished project.
## What it does
Our project has 2 parts, a transmitter and a receiver, the transmitter takes in a string message via bluetooth and the encodes it into morse code, this then processes that morse code into moving a motor with an extended arm with specific patterns to convey a dot/dash/space/screen clear. This was programmed onto an ESP32 microcontroller we chose for its wireless capacities. The receiver takes in the morse code from the motor using an ultrasonic sensor, we programmed it to make sense of the morse code and decode it into text which will be displayed using a cycling animation for each character played after each dot/dash.
## How we built it
We build it using an ESP32 microcontroller, an Arduino uno, a breadboard, an ultrasonic sensor, and a servo motor. We started by splitting into pairs with one working on the receiver and one working on the transmitter, working on the receiver involved programming the decoding of morse code, the sensing of morse code and then displaying that code (including an animation). The transmitter involved programming the encoding to morse code from a bluetooth message, then also programming a motor to go down and up according to the receiver specifications.
## Challenges we ran into
Our first challenge involved a raspberry pi, we attempted to use that as our receiver, although this did not end up working due to network constraints and led to us using the ESP32 instead. Our next challenge was the motor (which had quite a few). The first time was when we decided after being nearly finished with the stepper motor code that it would make more sense for us to use a servo motor instead. This involved a full rewrite and some different wiring. We then ran into a problem of the motor jittering causing the sensor to receive false inputs. After 2 hours of trial and error we discovered the problem to be a dying battery used as the power source for the motor. This took us 2 different iterations of code and asking multiple people to see if they had a spare motor until it finally worked. Our final challenge was maintaining accuracy, we found that with words with fast changing dots/dashes often ended up with wrong letters/symbols/numbers. This was fixed by a lot of code optimisations and moving the sensor/motor around.
## Accomplishments that we're proud of
Since this is our first hardware project we are quite proud of being able to actually have our program run not on our own computers. We are proud of the most difficult parts of the programming such as the morse code decoding and encoding and the motor movement.
## What we learned
We learned a lot about programming Arduino and microcontroller technology using the small version of C++ that is used in Arduino IDE. We also learned about the satisfaction of code running on hardware over regular software with seeing actual physical things happen without a computer connected to either of them.
## What's next for Morse Code UltraSonic Transmitter
We have plans to make it a two way communication with applying both the transmitter and receiver code to both of them. We also would like to add a 3rd part of the communication, where the receiver passes on the decoded message to a different piece of technology such as a TV screen or a projector.

## Built With
`Arduino` `Circuits` `ESP312` `Microcontroller` `LCD` `Ultrasonic Sensor` `Servo Motor`
`C++` `Ardino IDE`
