


// Sundry globals used for animation ---------------------------------------



void setupMatrix()
{
  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for (;;)
      ;
  }


  //matrix.setFont(&FreeSans9pt7b);  // Use nice bitmap font
  matrix.setTextWrap(false);               // Allow text off edge

  int16_t x1, y1;
  uint16_t w;
  uint16_t h;//
  matrix.getTextBounds(text_line_1, 0, 0, &x1, &y1, &w, &h);  // How big is it?
  textMin = -w;                                               // All text is off left edge when it reaches this point
                                                              // textY = matrix.height() / 2 - (y1 + h / 2); // Center text vertically
                                                              textMin = -36; 
  textY = 100;
}

int findEndXPos(int sentIndex)
{
    int endXpos = -matrix.width() -findMaxLen(sentenceIndex) - 30;
    //Serial.print("xposition_end: "); Serial.println(xposition_end);
    Serial.print("xposition_end: "); Serial.println(xposition_end);

    return endXpos;
}

int findMaxLen(int sentIndex){

      int maxLen = 0;
      char tempArray[100];

      for(int i =0 ; i<100; i++){
        tempArray[i] = sentences[sentIndex][i];
      }

      maxLen = strlen(tempArray);
      Serial.println(maxLen);

    return maxLen;
}