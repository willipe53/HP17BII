void testDisplayText2(int row, String text) {
  int16_t  x1, y1;
  uint16_t w, h;
  int x, y;
  display.getTextBounds(text.c_str(), 0, 0, &x1, &y1, &w, &h);
  //display.clearDisplay();
  x = 400-X_OFFSET-w;
  y = 240-Y_OFFSET-(ROW_HEIGHT*row);
  display.setCursor(x, y);
  display.write(text.c_str());
  display.refresh();
  Serial.printf("%s written at (%d,%d) row:%d\n", text.c_str(), x, y, row);
}

void flashLogo() {
  display.clearDisplay();
  drawLogo(BLACK);
  delay(500);
  drawLogo(WHITE);
  delay(500);
  drawLogo(BLACK);
  delay(500);
  drawLogo(WHITE);
  delay(500);
  drawLogo(BLACK);
  delay(1000);
}

void drawLogo(int fgColor) {
  int bgColor = !fgColor;
  display.fillRect(0, 0, 400, 240, fgColor);
  display.fillRect(0, 70, 400, 100, bgColor);
  display.drawBitmap(0, 70, hplogo, 400, 100, fgColor);
  display.refresh();
}


void redrawRegisters() {
  Serial.println("TODO: redrawing registers.");
  display.clearDisplay();
  for (int i=0; i<=LAST_REG; i++) {
//    Serial.printf("TODO: use GFX to redraw register %d as %s\n", i, reg[i].c_str());
//    int x = 400 - getPixelLength(reg[i]);
//    for (unsigned int j=0; j<reg[i].length(); j++) {
//      char digit = reg[i].charAt(j); 
//      int y = 240 - (i+1)*32;
//      Serial.printf("   Digit %d (%c) being placed at (%d,%d)\n", j, digit, x, y);
//      int w = drawDigit(digit, x, y);
//      x += w;
//    }
  }
}

void reformatRegister(int regNum) {
  reg[regNum].replace(",",""); //take commas out
  int dotAt = reg[regNum].indexOf(".");
  String base = reg[regNum];
  String deci = "";
  String dot = ".";
  if (dotAt >= 0) {
    base = reg[regNum].substring(0, dotAt); 
    deci = reg[regNum].substring(dotAt+1);
    deci.replace(".","");  
  }
  reg[regNum] = thousandsSeparated(base.toInt());
  if (dotAt >= 0) {
    //reg[regNum].concat(dot).concat(deci);
  }
}

String thousandsSeparated(int num) {  
  int i,j,len;
  char c[9+10],ch; //can handle numbers with a max of 9 commas in them
  i=0;
  j=1;
  while(num>0) {
    c[i] = num % 10 + '0';
    num /= 10;
    ++i;
    if(j%3==0 && num>0) {
      c[i++]=',';
    }
    ++j;
  }
  c[i]='\0';                      //make the chars a string
  len = strlen(c);   
  for(i=0,j=len-1;i<j;i++,j--) {  //reverse the string
    ch = c[i];
    c[i]=c[j];
    c[j]=ch;
  }
  return String(c);    
}
