// --- ADAFRUIT nrf52840 EXPRESS --- //

#define MaxSentences 30
#define LiveSentences 7
int liveSentenceIndex;

char sentences[][100]  = { 
"Is there a problem, officer?",  // 1
"You're doing great!", 
"The current time is......... NOW",
"You look good! Yes you!",
"Birmingham changed my life",
"It was better next year",
"Dont forget your pelvic floor exercises",
"Less drama, more techno",
"Arriba, abajo, al centro, pa dentro",
"Malkovich Malkovich Malkovich Malkovich Malkovich Malkovich Malkovich Malkovich",
"You only live once... broke and in poor health",
//"Advertiste you joke here! Call 1-800-CRINGE",
"ENM Ethical Non-Mahogany wood company" //13
};

int numberOfPasses = 6;
int internal_newSentence = 10000;
int interval_fps = 50;

// Use this website to calculate the color code: http://www.barth-dev.de/online/rgb565-color-picker/
uint16_t colors[] = {
  0x99F4,
  0xF81E, // pink-ish
  0x03DF, // light blue
  0x07E9, // green apple
  0xFFA0, // yellow
  0xF800, // red
  0xA81F, // purple
  0xFD60, // orange
  0x001F, // dark blue
};

int currentColor = 0;


int yposition = 12;

// ------------

#define MAX_INPUT 1000
int availableLiveSentences;

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

int16_t textX = matrix.width(),  // Current text position (X)
  textY,                         // Current text position (Y)
  textMin,                       // Text pos. (X) when scrolled off left edge
  hue = 0;
char text_line_1[MAX_INPUT];  // Buffer to hold scrolling message text
// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

int sentenceIndex = 0;
int xposition = matrix.width();
int xposition_end;


void setup(void) {

  Serial.begin(9600);

  setupBLE();

  setupMatrix();

  
  availableLiveSentences = MaxSentences - LiveSentences;
  liveSentenceIndex = availableLiveSentences;

  xposition_end = findEndXPos(sentenceIndex);
}

void loop(void) {

  loopBLE();

  displayText();

  readtext_line_1ing();
}



unsigned long prevMill_newSentence = 0;
unsigned long prevMill_fps = 0;
int passes = 0;

void displayText() {

  if(millis() - prevMill_newSentence > internal_newSentence) {
    prevMill_newSentence  = millis();

    
  }



  

  if (millis() - prevMill_fps > interval_fps) {
    prevMill_fps = millis();

    matrix.fillScreen(0);  // Fill background black

    // Draw the big scrolly text
    matrix.setTextColor(colors[currentColor]);
    matrix.setCursor(xposition, yposition);

    matrix.print(sentences[sentenceIndex]);

    //Serial.println(xposition);

    xposition = xposition - 1;
    if (xposition < xposition_end){
        xposition = matrix.width();
        passes++;
        if(passes > numberOfPasses)
        {
         passes = 0; 
         nextSentence();
        }
    } 

    matrix.show();

  }
}