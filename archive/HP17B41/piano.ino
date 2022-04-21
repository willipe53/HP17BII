/*
 * C        262 Hz
 * C#4/Db4  277
 * D4       294
 * D#4/Eb4  311
 * E4       330
 * F4       349  
 * F#4/Gb4  370  
 * G4       392  
 * G#4/Ab4  415  
 * A4       440
 * A#4/Bb4  466  
 * B4       494
 */
char pianoKeys[] = {  '1', '2',  '3', '-',  'D',  '4', '5',  '6', 'x',  'U', '7',  '8',  '9', 245,  'I', 240,  '(',  ')', 'B',  'S', 'R',  '%', 'L',  'P',  'E'};
int freqs[]      = {  262, 277,  294, 311,  330,  349, 370,  392, 415,  440, 466,  494,  523, 554,  587, 622,  659,  698, 740,  784, 831,  880, 932,  988, 1047};
int keyX[]       = {    0,  24,   32,  56,   64,   96, 120,  128, 152,  160, 184,  192,  224, 248,  256, 280,  288,  320, 344,  352, 376,  384, 408,  416,  448};
bool keyBlack[]  = {false,true,false,true,false,false,true,false,true,false,true,false,false,true,false,true,false,false,true,false,true,false,true,false,false};

void playFreqForChar(char c) {
  int freq;
  for (int i=0; i<25; i++) {
    if (pianoKeys[i] == c) {
      scheduleTone(freqs[i], 1000, millis());
      drawPianoKey(i, c, true);
      display.refresh();
      delay(200);
      drawPiano();
      display.refresh();
      break;
    }
  }
}

void drawPiano() {
  //presumes we start with C
  display.clearDisplay();
  for (int i=0; i<25; i++) {
    drawPianoKey(i, pianoKeys[i], false);
  }
  drawButtonBar();
}

void drawPianoKey(int keyNum, char c, bool invert) {
  int whiteKeyWidth = 32;
  int blackKeyWidth = whiteKeyWidth / 2;
  int blackKeyH = MAIN_H - 30;
  int blackKeyOffset = 24;
  if (keyBlack[keyNum]) {
    display.setTextColor((invert) ? BLACK : WHITE);   //invert
    display.fillRect(keyX[keyNum],startY,blackKeyWidth,blackKeyH,(invert) ? WHITE : BLACK);  //invert   
    display.setCursor(keyX[keyNum]+3,startY+blackKeyH-20);
    Serial.printf("Putting black key %c (idx %d) at %d %d %d %d\n",c, keyNum, keyX[keyNum],startY,blackKeyWidth,blackKeyH);
  } else {
    display.setTextColor((invert) ? WHITE : BLACK);  //invert
    if (invert) display.fillRect(keyX[keyNum],startY,whiteKeyWidth,MAIN_H,BLACK);   //invert 
    else display.drawRect(keyX[keyNum],startY,whiteKeyWidth,MAIN_H,BLACK);  //invert   
    display.setCursor(keyX[keyNum]+11,startY+blackKeyH+5);
    Serial.printf("Putting white key %c (idx %d) at %d %d %d %d\n",c, keyNum, keyX[keyNum],startY,whiteKeyWidth,MAIN_H);
  }
  display.write(c);
}

void buzzerAlert(char key) {
  Serial.printf("Buzzing the buzzer\n");    
  tone(BUZZER, 830);
  delay(100);    
  tone(BUZZER, 784);
  delay(100);    
  tone(BUZZER, 466);
  delay(200);    
  noTone(BUZZER);
}
