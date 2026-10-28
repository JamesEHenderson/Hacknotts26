# Hacknotts26 - Morse Code UltraSonic Transmitter

For Hacknotts26, a 25-Hour Hackathon, we built a Bluetooth text to morse code converter on an ESP32 microcontroller, which sends the information to an Arduino Uno to decode and display on an LCD screen.

[Watch the Demo Video!](https://www.youtube.com/watch?v=JobzFIudcuk)

<img src="images/WhatsApp Image 2026-10-25 at 11.34.10.jpeg" width="30%" hspace=2 alt="Image"><img src="images/WhatsApp Image 2026-10-25 at 11.34.11.jpeg" width="30%" hspace=2 alt="Image"><img src="images/WhatsApp Image 2026-10-25 at 11.34.18.jpeg" width="30%" hspace=2 alt="Image">


## Inspiration
We were originally inspired by pager technology in the early 2000s, this then progressed into a different project after we were trying to brainstorm ways to get them to communicate with each other (we decided on morse code) then quickly evolved into our finished project.
## What it does
Our project has 2 parts, a Transmitter(ESP32) and a Receiver(Arduino Uno), the transmitter takes in a string message(allowing for numerals, symbols and letters) via Bluetooth connection with a phone and then encodes it into morse code, the microcontroller then moves a motor with an extended arm in specific patterns to convey either a dot, dash, space or screen clear. We chose the ESP32 for its wireless capacities, allowing us to interface with the system using Bluetooth. The Receiver then interprets the morse code from the motor using an ultrasonic sensor, we programmed it to make sense of the morse code and decode it into ASCII text to be displayed on our LCD screen which will cycle between characters as each dot/dash gets us closer to the correct representation.
## How we built it
The Project is made up of many components, including: An Espressif ESP32 Dev Board, an Arduino Uno, a Breadboard, an Ultrasonic Sensor, a Servo Motor, and an LCD Display. We started by splitting into pairs with one pair working on the Receiver and one pair working on the Transmitter. Working on the receiver required programming the detection of morse code via the Ultrasonic Sensor, the decoding of that morse code and then displaying the resulting text (including the character cycling animation). The Transmitter involved setting up the Bluetooth Connection to the Input Device, programming to receive data and encode ASCII text into More code and finally controlling the servo motor to go down and up at specifically timed intervals according to the Receiver's specifications.
## Challenges we ran into
Our first challenge was in Planning, due to the nature of the competition we were unsure what devices we would initially have access to. We knew we wanted wireless capabilities so our first thought was a Raspberry Pi - we attempted to use that as our Receiver, with plans to develop a Web Interface, but unfortunately we were unable to configure the Pi as we didn't have the keyboard and mouse necessary for a normal setup and were unable to access the Pi via a Headless setup due to network constraints, this led us to the ESP32 instead, it was lightweight and had the perfect wireless capabilities to ensure some of our intended vision. 

Another challenge was the motor. Our first problem was pin layout, We had initially decided to use a Stepper Motor after initial research showed us it would allow more precision but we were unable to configure it with the ESP32 Board we had access to in a reasonable time as we had no access to our specific boards documentation and had to intuit much of the Microcontrollers pin layout as well as relevant libraries through a combination of trial and error and extensive research, after this started becoming a larger burden than necessary we decided that it would make more sense to use a servo motor instead. This involved a full rewrite of code and different wiring, but the result performed much faster - resulting in a speed increase that ultimately meant faster transmission of text. Likely our most common problem was motor jittering causing the sensor to receive false inputs. After 2 hours of trial and error we discovered the problem to be the result of a dying battery and out-of-sync timings between the receiver and transmitter. This took us several iterations of code and required us to look for spare components as we underwent troubleshooting. 

Our final challenge was maintaining accuracy, we found that as we attempted to increase the speed of transmission and the distance over which it occurred we often ended up with the wrong characters, we fixed this by fine-tuning the margins for error in the Receiver's detecting thresholds and making sure they lined up accurately with the Transmitter's motor delays, resulting in  lot of code optimisations.
## Accomplishments that we're proud of
Since this is our first hardware project we are really quite proud of being able to actually have our program run independently of any computers, both devices - once programmed - require only power and text input. We are proud of the most difficult parts of the programming such as the morse code decoding and encoding and the motor movement. But most of all we are proud of the fact that, despite all the problems we encountered in this 25 hour Hackathon, we endured and made a working and impressive Project!
## What we learned
We learned a lot about low-level programming with Arduino and microcontroller technology using a version of C++ intended for Arduino's. We also learned the satisfaction of our code running on hardware over regular software and seeing actual, physical things happen without a computer connected to either of them.
## What's next for the Morse Code UltraSonic Transmitter
We have plans to allow the project two way communication by merging both the transmitter and receiver code. We would also like to add a 3rd part to the communication, where the receiver passes on the decoded message to a different piece of technology such as a TV screen or a projector.

## Built With
`Arduino` `Circuits` `ESP32` `Microcontroller` `LCD` `Ultrasonic Sensor` `Servo Motor`
`C++` `Arduino IDE`
