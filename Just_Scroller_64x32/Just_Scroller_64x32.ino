// Use this website to calculate the color code: http://www.barth-dev.de/online/rgb565-color-picker/
uint16_t color_line_1 = 0x99F4;
uint16_t color_line_2 = 0xE820;

#define MAX_INPUT 1000

#include <Adafruit_Protomatter.h>

 uint8_t rgbPins[]  = {6, A5, A1, A0, A4, 11};
  uint8_t addrPins[] = {10, 5, 13, 9};
  uint8_t clockPin   = 12;
  uint8_t latchPin   = PIN_SERIAL1_RX;
  uint8_t oePin      = PIN_SERIAL1_TX;

/* ----------------------------------------------------------------------
Matrix initialization is explained EXTENSIVELY in "simple" example sketch!
It's very similar here, but we're passing "true" for the last argument,
enabling double-buffering -- this permits smooth animation by having us
draw in a second "off screen" buffer while the other is being shown.
------------------------------------------------------------------------- */

Adafruit_Protomatter matrix(
  64,          // Matrix width in pixels
  6,           // Bit depth -- 6 here provides maximum color options
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
char text_line_2[MAX_INPUT];  // Buffer to hold scrolling message text

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void setup(void) {
  Serial.begin(9600);

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
  matrix.setTextWrap(false);               // Allow text off edge

  int16_t x1, y1;
  uint16_t w;
  uint16_t h;//
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
int pos_text_line_1 = 15;
int pos_text_line_2 = 30;
void loop(void) {

  if (millis() - prevMill > interval) {
    prevMill = millis();

    // Every frame, we clear the background and draw everything anew.
    // This happens "in the background" with double buffering, that's
    // why you don't see everything flicker. It requires double the RAM,
    // so it's not practical for every situation.

    matrix.fillScreen(0);  // Fill background black

    // Draw the big scrolly text
    matrix.setTextColor(color_line_1);
    if(noFont == true){
      matrix.setCursor(textX, pos_text_line_1 - 10);
    } else {
    matrix.setCursor(textX, pos_text_line_1);
    }
   
    matrix.print(text_line_1);

    matrix.setTextColor(color_line_2);
    if(noFont == true){
      matrix.setCursor(textX, pos_text_line_2 - 10);
    } else {
    matrix.setCursor(textX, pos_text_line_2);
    }
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


  } else // else if (data[0] == 'F') {

   // Serial.print("Updating Font: "); 

 
                                             

     if(strcmp(data, "FontReset") == 0) {
       Serial.println("NO FONT");
       matrix.setFont(NULL);  // Use nice bitmap font
       noFont = true;
     } else {
       noFont = false;

    //       if(strcmp(data, "FreeMono9pt7b") == 0) matrix.setFont(&FreeMono9pt7b);  // Use nice bitmap font
    //  if(strcmp(data, "FreeMono12pt7b") == 0) matrix.setFont(&FreeMono12pt7b);  // Use nice bitmap font
    //   if(strcmp(data, "FreeMono18pt7b") == 0) matrix.setFont(&FreeMono18pt7b);  // Use nice bitmap font
    //    if(strcmp(data, "FreeMono24pt7b") == 0) matrix.setFont(&FreeMono24pt7b);  // Use nice bitmap font
    //     if(strcmp(data, "FreeMonoBold9pt7b") == 0) matrix.setFont(&FreeMonoBold9pt7b);  // Use nice bitmap font
    //      if(strcmp(data, "FreeMonoBold12pt7b") == 0) matrix.setFont(&FreeMonoBold12pt7b);  // Use nice bitmap font
    //       if(strcmp(data, "FreeMonoBold18pt7b") == 0) matrix.setFont(&FreeMonoBold18pt7b);  // Use nice bitmap font
    //        if(strcmp(data, "FreeMonoBold24pt7b") == 0) matrix.setFont(&FreeMonoBold24pt7b);  // Use nice bitmap font
    //         if(strcmp(data, "FreeMonoBoldOblique9pt7b") == 0) matrix.setFont(&FreeMonoBoldOblique9pt7b);  // Use nice bitmap font
    //          if(strcmp(data, "FreeMonoBoldOblique12pt7b") == 0) matrix.setFont(&FreeMonoBoldOblique12pt7b);  // Use nice bitmap font
    //           if(strcmp(data, "FreeMonoBoldOblique18pt7b") == 0) matrix.setFont(&FreeMonoBoldOblique18pt7b);  // Use nice bitmap font
    //            if(strcmp(data, "FreeMonoBoldOblique24pt7b") == 0) matrix.setFont(&FreeMonoBoldOblique24pt7b);  // Use nice bitmap font
    //             if(strcmp(data, "FreeMonoOblique9pt7b") == 0) matrix.setFont(&FreeMonoOblique9pt7b);  // Use nice bitmap font
    //              if(strcmp(data, "FreeMonoOblique12pt7b") == 0) matrix.setFont(&FreeMonoOblique12pt7b);  // Use nice bitmap font
    //               if(strcmp(data, "FreeMonoOblique18pt7b") == 0) matrix.setFont(&FreeMonoOblique18pt7b);  // Use nice bitmap font
    //                if(strcmp(data, "FreeMonoOblique24pt7b") == 0) matrix.setFont(&FreeMonoOblique24pt7b);  // Use nice bitmap font
    //                 if(strcmp(data, "FreeSans9pt7b") == 0) matrix.setFont(&FreeSans9pt7b);  // Use nice bitmap font
    //                  if(strcmp(data, "FreeSans12pt7b") == 0) matrix.setFont(&FreeSans12pt7b);  // Use nice bitmap font
    //                   if(strcmp(data, "FreeSans18pt7b") == 0) matrix.setFont(&FreeSans18pt7b);  // Use nice bitmap font
    //                    if(strcmp(data, "FreeSans24pt7b") == 0) matrix.setFont(&FreeSans24pt7b);  // Use nice bitmap font
    //                     if(strcmp(data, "FreeSansBold9pt7b") == 0) matrix.setFont(&FreeSansBold9pt7b);  // Use nice bitmap font
    //                      if(strcmp(data, "FreeSansBold12pt7b") == 0) matrix.setFont(&FreeSansBold12pt7b);  // Use nice bitmap font
    //                       if(strcmp(data, "FreeSansBold18pt7b") == 0) matrix.setFont(&FreeSansBold18pt7b);  // Use nice bitmap font
    //                        if(strcmp(data, "FreeSansBold24pt7b") == 0) matrix.setFont(&FreeSansBold24pt7b);  // Use nice bitmap font
    //                         if(strcmp(data, "FreeSansBoldOblique9pt7b") == 0) matrix.setFont(&FreeSansBoldOblique9pt7b);  // Use nice bitmap font
    //                          if(strcmp(data, "FreeSansBoldOblique12pt7b") == 0) matrix.setFont(&FreeSansBoldOblique12pt7b);  // Use nice bitmap font
    //                           if(strcmp(data, "FreeSansBoldOblique18pt7b") == 0) matrix.setFont(&FreeSansBoldOblique18pt7b);  // Use nice bitmap font
    //                            if(strcmp(data, "FreeSansBoldOblique24pt7b") == 0) matrix.setFont(&FreeSansBoldOblique24pt7b);  // Use nice bitmap font
    //                             if(strcmp(data, "FreeSansOblique9pt7b") == 0) matrix.setFont(&FreeSansOblique9pt7b);  // Use nice bitmap font
    //                              if(strcmp(data, "FreeSansOblique12pt7b") == 0) matrix.setFont(&FreeSansOblique12pt7b);  // Use nice bitmap font
    //                               if(strcmp(data, "FreeSansOblique18pt7b") == 0) matrix.setFont(&FreeSansOblique18pt7b);  // Use nice bitmap font
    //                                if(strcmp(data, "FreeSansOblique24pt7b") == 0) matrix.setFont(&FreeSansOblique24pt7b);  // Use nice bitmap font
    //                                 if(strcmp(data, "FreeSerif9pt7b") == 0) matrix.setFont(&FreeSerif9pt7b);  // Use nice bitmap font
    //                                  if(strcmp(data, "FreeSerif12pt7b") == 0) matrix.setFont(&FreeSerif12pt7b);  // Use nice bitmap font
    //                                   if(strcmp(data, "FreeSerif18pt7b") == 0) matrix.setFont(&FreeSerif18pt7b);  // Use nice bitmap font
    //                                    if(strcmp(data, "FreeSerif24pt7b") == 0) matrix.setFont(&FreeSerif24pt7b);  // Use nice bitmap font
    //                                     if(strcmp(data, "FreeSerifBold9pt7b") == 0) matrix.setFont(&FreeSerifBold9pt7b);  // Use nice bitmap font
    //                                      if(strcmp(data, "FreeSerifBold12pt7b") == 0) matrix.setFont(&FreeSerifBold12pt7b);  // Use nice bitmap font
    //                                       if(strcmp(data, "FreeSerifBold18pt7b") == 0) matrix.setFont(&FreeSerifBold18pt7b);  // Use nice bitmap font
    //                                        if(strcmp(data, "FreeSerifBold24pt7b") == 0) matrix.setFont(&FreeSerifBold24pt7b);  // Use nice bitmap font
    //                                         if(strcmp(data, "FreeSerifBoldItalic9pt7b") == 0) matrix.setFont(&FreeSerifBoldItalic9pt7b);  // Use nice bitmap font
    //                                          if(strcmp(data, "FreeSerifBoldItalic12pt7b") == 0) matrix.setFont(&FreeSerifBoldItalic12pt7b);  // Use nice bitmap font
    //                                           if(strcmp(data, "FreeSerifBoldItalic18pt7b") == 0) matrix.setFont(&FreeSerifBoldItalic18pt7b);  // Use nice bitmap font
    //                                            if(strcmp(data, "FreeSerifBoldItalic24pt7b") == 0) matrix.setFont(&FreeSerifBoldItalic24pt7b);  // Use nice bitmap font
    //                                             if(strcmp(data, "FreeSerifItalic9pt7b") == 0) matrix.setFont(&FreeSerifItalic9pt7b);  // Use nice bitmap font
    //                                              if(strcmp(data, "FreeSerifItalic12pt7b") == 0) matrix.setFont(&FreeSerifItalic12pt7b);  // Use nice bitmap font
    //                                               if(strcmp(data, "FreeSerifItalic18pt7b") == 0) matrix.setFont(&FreeSerifItalic18pt7b);  // Use nice bitmap font
    //                                                if(strcmp(data, "FreeSerifItalic24pt7b") == 0) matrix.setFont(&FreeSerifItalic24pt7b);  // Use nice bitmap font

    //                                                if(strcmp(data, "Fledcounter7regular7pt7b") == 0) matrix.setFont(&ledcounter7regular7pt7b);  // Use nice bitmap font
    //                                                 if(strcmp(data, "Fledcounter7regular9pt7b") == 0) matrix.setFont(&ledcounter7regular9pt7b);
    //                                                  if(strcmp(data, "Fledcounter7regular10pt7b") == 0) matrix.setFont(&ledcounter7regular10pt7b);
    //                                                   if(strcmp(data, "Fledcounter7regular12pt7b") == 0) matrix.setFont(&ledcounter7regular12pt7b);
    //                                                    if(strcmp(data, "Fledcounter7regular14pt7b") == 0) matrix.setFont(&ledcounter7regular14pt7b);

    //                                                     if(strcmp(data, "FTomatoCoffee7") == 0) matrix.setFont(&TomatoCoffee7pt7b);
    //                                                      if(strcmp(data, "FTomatoCoffee9") == 0) matrix.setFont(&TomatoCoffee9pt7b);
    //                                                       if(strcmp(data, "FTomatoCoffee10") == 0) matrix.setFont(&TomatoCoffee10pt7b);
    //                                                        if(strcmp(data, "FTomatoCoffee12") == 0) matrix.setFont(&TomatoCoffee12pt7b);
    //                                                         if(strcmp(data, "FTomatoCoffee14") == 0) matrix.setFont(&TomatoCoffee14pt7b);

    //                                                          if(strcmp(data, "FJdLcdRoundedRegular7") == 0) matrix.setFont(&JdLcdRoundedRegular7pt7b);
    //                                                           if(strcmp(data, "FJdLcdRoundedRegular9") == 0) matrix.setFont(&JdLcdRoundedRegular9pt7b);
    //                                                            if(strcmp(data, "FJdLcdRoundedRegular10") == 0) matrix.setFont(&JdLcdRoundedRegular10pt7b);
    //                                                             if(strcmp(data, "FJdLcdRoundedRegular12") == 0) matrix.setFont(&JdLcdRoundedRegular12pt7b);
    //                                                              if(strcmp(data, "FJdLcdRoundedRegular14") == 0) matrix.setFont(&JdLcdRoundedRegular14pt7b);

    //                                                               if(strcmp(data, "FLEDCalculator7") == 0) matrix.setFont(&LEDCalculator7pt7b);
    //                                                                if(strcmp(data, "FLEDCalculator9") == 0) matrix.setFont(&LEDCalculator9pt7b);
    //                                                                 if(strcmp(data, "FLEDCalculator10") == 0) matrix.setFont(&LEDCalculator10pt7b);
    //                                                                  if(strcmp(data, "FLEDCalculator12") == 0) matrix.setFont(&LEDCalculator12pt7b);
    //                                                                   if(strcmp(data, "FLEDCalculator14") == 0) matrix.setFont(&LEDCalculator14pt7b);

    //                                                                    if(strcmp(data, "Fjdstars7") == 0) matrix.setFont(&jdstars7pt7b);
    //                                                                     if(strcmp(data, "Fjdstars9") == 0) matrix.setFont(&jdstars9pt7b);
    //                                                                      if(strcmp(data, "Fjdstars10") == 0) matrix.setFont(&jdstars10pt7b);
    //                                                                       if(strcmp(data, "Fjdstars12") == 0) matrix.setFont(&jdstars12pt7b);
    //                                                                        if(strcmp(data, "Fjdstars14") == 0) matrix.setFont(&jdstars14pt7b);
                                                                    
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
