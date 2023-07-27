#include <SoftwareSerial.h>

String allSentences[100] = {
  "Is there a problem officer?",
  "You're doing great!",
  "Allez on y va"
};

//const char* array[][1] = {{"AA"},{"BBBB"},{"CCCC CCCC"}};


void setup() {
  Serial.begin(9600);

 // delay(4000);

 // Serial.print("array1 size": ); Serial.println(sizeof([][1]));
  //Serial.print("array2 size": ); Serial.println(array[][]);

  // Serial.println(array[0][1]);
  //  Serial.println(array[1][1]);
  //   Serial.println(array[2][1]);
  // put your setup code here, to run once:
  setupBLE();
  //setupMatrix();
}

void loop() {
  // put your main code here, to run repeatedly:
  loopBLE();
  //newLoopMatrix();
}
