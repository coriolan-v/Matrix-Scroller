// Use this website to calculate the color code: http://www.barth-dev.de/online/rgb565-color-picker/
uint16_t color_line_1 = 0x99F4;
uint16_t color_line_2 = 0xE820;

#define MAX_INPUT 1000

// This code needs to be compiled for Raspberry Pi Pico
// It needs Adafruit_Protomatter installed




#include <Adafruit_Protomatter.h>

// Special nRF52840 FeatherWing pinout
uint8_t rgbPins[] = { 6, A5, A1, A0, A4, 11 };
uint8_t addrPins[] = { 10, 5, 13, 9 };
uint8_t clockPin = 12;
uint8_t latchPin = PIN_SERIAL1_RX;
uint8_t oePin = PIN_SERIAL1_TX;

// Create a 32-pixel tall, 64 pixel wide matrix with the defined pins
Adafruit_Protomatter matrix(
  64, 4, 1, rgbPins, 4, addrPins, clockPin, latchPin, oePin, false);
// Sundry globals used for animation ---------------------------------------

int16_t textX = matrix.width(),  // Current text position (X)
  textY,                         // Current text position (Y)
  textMin,                       // Text pos. (X) when scrolled off left edge
  hue = 0;
char text_line_1[MAX_INPUT];  // Buffer to hold scrolling message text
char text_line_2[MAX_INPUT];  // Buffer to hold scrolling message text

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void setupMatrix() {


  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for (;;)
      ;
  }

  // Unlike the "simple" example, we don't do any drawing in setup().
  // But we DO initialize some things we plan to animate...

  // Set up the scrolling message...
  sprintf(text_line_1, "Jason Bruges Studio artwork text here");
  sprintf(text_line_2, "Second Line of text to be printed");

  //matrix.setFont(&FreeSans9pt7b);  // Use nice bitmap font
  matrix.setTextWrap(false);  // Allow text off edge

  int16_t x1, y1;
  uint16_t w;
  uint16_t h;                                                 //
  matrix.getTextBounds(text_line_1, 0, 0, &x1, &y1, &w, &h);  // How big is it?
  textMin = -w;                                               // All text is off left edge when it reaches this point
                                                              // textY = matrix.height() / 2 - (y1 + h / 2); // Center text vertically
  textY = 100;
  // Note: when making scrolling text like this, the setTextWrap(false)
  // call is REQUIRED (to allow text to go off the edge of the matrix),
  // AND it must be BEFORE the getTextBounds() call (or else that will
  // return the bounds of "wrapped" text).

  // Set up the colors of the bouncy balls.
  // ballcolor[0] = matrix.color565(0, 20, 0); // Dark green
  // ballcolor[1] = matrix.color565(0, 0, 20); // Dark blue
  // ballcolor[2] = matrix.color565(20, 0, 0); // ark red
}


bool noFont = true;
// LOOP - RUNS REPEATEDLY AFTER SETUP --------------------------------------
unsigned long prevMill;
int interval = 25;
int pos_text_line_1 = 16;
int pos_text_line_2 = 48;

int interval_scrollSentence = 5000;
int internal_newSentence = 30000;
int interval_fps = 25;

unsigned long prevMill_scrollSentence = 0;
unsigned long prevMill_newSentence = 0;
unsigned long prevMill_fps = 0;


int xposition = 64;
void newLoopMatrix() {

  if(millis() - prevMill_scrollSentence) {
    prevMill_scrollSentence  = millis();

    xposition = 64;
  }

  

  if (millis() - prevMill_fps > interval_fps) {
    prevMill_fps = millis();

    matrix.fillScreen(0);  // Fill background black

    // Draw the big scrolly text
    matrix.setTextColor(color_line_1);
    matrix.setCursor(textX, 10);
    matrix.print(text_line_1);

    matrix.setTextColor(color_line_2);
    matrix.setCursor(textX, 20);
    matrix.print(text_line_2);

    if(xposition > 0) xposition--;

    matrix.show();

  }

    
}

void loopMatrix(void) {

  if (millis() - prevMill > interval) {
    prevMill = millis();

    // Every frame, we clear the background and draw everything anew.
    // This happens "in the background" with double buffering, that's
    // why you don't see everything flicker. It requires double the RAM,
    // so it's not practical for every situation.

    matrix.fillScreen(0);  // Fill background black

    // Draw the big scrolly text
    matrix.setTextColor(color_line_1);

    matrix.setCursor(textX, 10);


    matrix.print(text_line_1);

    matrix.setTextColor(color_line_2);

    matrix.setCursor(textX, 20);

    matrix.print(text_line_2);

    // Update text position for next frame. If text goes off the
    // left edge, reset its position to be off the right edge.
    if ((--textX) < textMin) textX = matrix.width();

    matrix.show();
  }


  //delay(10); // 20 milliseconds = ~50 frames/second

  readtext_line_1ing();
}

void readtext_line_1ing() {
  if (Serial.available() > 0)
    processIncomingByte(Serial.read());
}

void process_data(char* data) {

  if (data[0] == '2') {
    Serial.print("Updating Line 2: ");

    if (data[2] == 'x') {
      Serial.print("Updating Line 2 color: ");

      int newColor;
      char arrayColor[4];
      arrayColor[0] = data[3];
      arrayColor[1] = data[4];
      arrayColor[2] = data[5];
      arrayColor[3] = data[6];
      newColor = (int)strtol(arrayColor, NULL, 16);
      //newColor = atoi(arrayColor);
      //sscanf(arrayColor, "%i", &newColor);
      Serial.print(" / int: ");
      Serial.println(newColor);

      color_line_2 = newColor;
    } else {
      for (int i = 1; i < MAX_INPUT; i++) {
        text_line_2[i - 1] = data[i];
      }
    }

    Serial.println(text_line_2);

  } else if (data[0] == '1') {


    if (data[2] == 'x') {
      Serial.print("Updating Line 1 color: ");

      int newColor;
      char arrayColor[4];
      arrayColor[0] = data[3];
      arrayColor[1] = data[4];
      arrayColor[2] = data[5];
      arrayColor[3] = data[6];
      newColor = (int)strtol(arrayColor, NULL, 16);
      //newColor = atoi(arrayColor);
      //sscanf(arrayColor, "%i", &newColor);
      //Serial.print(" / int: "); Serial.println(newColor);

      color_line_1 = newColor;
    } else {
      // if (data[0] == '1') {

      Serial.print("Updating Line 1: ");

      for (int i = 1; i < MAX_INPUT; i++) {
        text_line_1[i - 1] = data[i];
      }
    }
    // }
    // else {
    //   for (int i = 0; i < MAX_INPUT; i++) {
    //     text_line_1[i] = data[i];
    //   }
    //}
    Serial.println(text_line_1);

  } else if (data[0] == 's') {

    Serial.print("Updating Speed: ");

    int new_speed;
    char newSpeed[3];
    newSpeed[0] = data[1];
    newSpeed[1] = data[2];
    newSpeed[2] = data[3];
    new_speed = (int)strtol(newSpeed, NULL, 16);
    Serial.println(new_speed);
    interval = constrain(new_speed, 1, 100);


  } else  // else if (data[0] == 'F') {

    // Serial.print("Updating Font: ");




    if (strcmp(data, "FontReset") == 0) {
      Serial.println("NO FONT");
      matrix.setFont(NULL);  // Use nice bitmap font
      noFont = true;
    } else {
      noFont = false;
    }
  Serial.println(data);

  matrix.show();
  // (strcmp(data, "FreeMono9pt7b.h") == 0) ? "Good day." : "Good evening.";

  // if (strcmp(data, "FreeMono9pt7b.h") == 0) {
  //   Serial.println("Found FreeMono9pt7b ");
  // }

  // }

}  // end of process_data


void processIncomingByte(const byte inByte) {
  static char input_line[MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte) {

    case '\n':                    // end of text
      input_line[input_pos] = 0;  // terminating null byte

      // terminator reached! process input_line here ...
      process_data(input_line);

      // reset buffer for next time
      input_pos = 0;
      break;

    case '\r':  // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line[input_pos++] = inByte;
      break;

  }  // end of switch

}  // end of processIncomingByte
