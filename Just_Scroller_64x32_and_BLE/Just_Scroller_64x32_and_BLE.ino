// Use this website to calculate the color code: http://www.barth-dev.de/online/rgb565-color-picker/
String sentence1 = "Is there a problem, officer?";
String sentence2 = "You're doing great!";
String sentence3 = "Don't worry about it";
String sentence4 = "The current time is......... NOW";
String sentence5 = "You look good!";
String sentence6 = "La, tout n'est qu'ordre et beaute, Luxe, calme et volupte.";
String sentence7 = "¡Arriba, abajo, al centro, pa' dentro!";
String sentence8 = "";
String sentence9 = "";
String sentence10 = "";

#define MaxSentences 20
#define LiveSentences 7
int liveSentenceIndex;

char sentences[MaxSentences][100]  = { 
"Is there a problem, officer?",  // 1
"You're doing great!", 
"The current time is......... NOW",
"You look good! Yes you!",
"Birmingham changed my life",
"It was better next year",
"Gaffer tape fixes everything",
"Less drama, more techno",
"¡Arriba, abajo, al centro, pa' dentro!",
"Malkovich Malkovich Malkovich Malkovich Malkovich Malkovich Malkovich Malkovich",
"You only live once... broke and in poor health",
"Advertiste you joke here! Call 1-800-CRINGE",
"ENM Ethical Non Mahogany wood company" //13
};


uint16_t color_line_1 = 0x99F4;

#define MAX_INPUT 1000

#include <Adafruit_Protomatter.h>

 uint8_t rgbPins[]  = {6, A5, A1, A0, A4, 11};
  uint8_t addrPins[] = {10, 5, 13, 9};
  uint8_t clockPin   = 12;
  uint8_t latchPin   = PIN_SERIAL1_RX;
  uint8_t oePin      = PIN_SERIAL1_TX;

Adafruit_Protomatter matrix(
  64,          // Matrix width in pixels
  4,           // Bit depth -- 6 here provides maximum color options
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);       // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!

// Sundry globals used for animation ---------------------------------------

int16_t textX = matrix.width(),  // Current text position (X)
  textY,                         // Current text position (Y)
  textMin,                       // Text pos. (X) when scrolled off left edge
  hue = 0;
char text_line_1[MAX_INPUT];  // Buffer to hold scrolling message text
// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

int sentenceIndex = 0;
int xposition = matrix.width();
int xposition_end;
int yposition = 18;

void setup(void) {

  Serial.begin(9600);

  liveSentenceIndex = MaxSentences - LiveSentences;

  xposition_end = findEndXPos(sentenceIndex);

  setupBLE();

  setupMatrix();
}

void loop(void) {

  loopBLE();

  newLoopMatrix();

  readtext_line_1ing();
}


int internal_newSentence = 30000;
int interval_fps = 50;
unsigned long prevMill_newSentence = 0;
unsigned long prevMill_fps = 0;

void newLoopMatrix() {

  if(millis() - prevMill_newSentence > internal_newSentence) {
    prevMill_newSentence  = millis();

    sentenceIndex++;
    
    if(sentenceIndex >= MaxSentences || sentences[sentenceIndex][0] == 0){
        sentenceIndex = 0;
    } 
    
    Serial.print("Timer new sentence: ");Serial.println(sentenceIndex);

    xposition_end = findEndXPos(sentenceIndex);

    xposition = matrix.width();
  }



  

  if (millis() - prevMill_fps > interval_fps) {
    prevMill_fps = millis();

    matrix.fillScreen(0);  // Fill background black

    // Draw the big scrolly text
    matrix.setTextColor(color_line_1);
    matrix.setCursor(xposition, yposition);

    matrix.print(sentences[sentenceIndex]);

    Serial.println(xposition);

    if ((--xposition) < xposition_end) xposition = matrix.width();

    matrix.show();

  }
}