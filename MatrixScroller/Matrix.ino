


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
    int endXpos = -matrix.width() -findMaxLen(sentenceIndex)*4;
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
      Serial.print("Sentence len: "); Serial.println(maxLen);

    return maxLen;
}

void nextSentence()
{
  sentenceIndex++;
    if(sentenceIndex >= MaxSentences || sentences[sentenceIndex][0] == 0 || findMaxLen(sentenceIndex) < 3) sentenceIndex = 0;
    
    Serial.print("New sentence: ");Serial.print(sentenceIndex); 
    Serial.print(", "); Serial.println(sentences[sentenceIndex]);

    currentColor++;
    if(currentColor > 8) currentColor = 0;

    Serial.print("New color: ");Serial.print(currentColor); 
    Serial.print(", "); Serial.println(colors[currentColor]);
    
    xposition_end = findEndXPos(sentenceIndex);

    xposition = matrix.width();
}