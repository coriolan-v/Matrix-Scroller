#include <SoftwareSerial.h>

#define rxPin 20
#define txPin 21

// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(9600);
}

void loop() {
    if (mySerial.available() > 0) {
        mySerial.read();
    }
}