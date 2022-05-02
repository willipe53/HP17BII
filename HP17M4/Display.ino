void flashLogo() {
  display.clearDisplay();
  //drawLogo(BLACK);
  //delay(500);
  //drawLogo(WHITE);
  //delay(500);
  drawLogo(BLACK);
  delay(250);
  drawLogo(WHITE);
  delay(250);
  drawLogo(BLACK);
  delay(500);
}

void drawLogo(int fgColor) {
  int bgColor = !fgColor;
  display.fillRect(0, 0, 400, 240, fgColor);
  display.fillRect(0, 70, 400, 100, bgColor);
  display.drawBitmap(0, 70, hplogo, 400, 100, fgColor);
  display.refresh();
}

void drawRegText(int row, String text) {
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
  if (text.length() > 0) Serial.printf("%s written at x:%d y:%d w:%d h:%d row:%d\n", text.c_str(), x, y, w, h, row);
}

void redrawRegisters() {
  display.clearDisplay();
  for (int i=0; i<=LAST_REG; i++) {
    drawRegText(i,reg[i]);
  }
  if (waitingToStore) display.drawBitmap(4, 10, mod_img[0], 104, 30, BLACK);
  else if (waitingToFetch) display.drawBitmap(4, 10, mod_img[1], 104, 30, BLACK);
  else if (shiftPressed) display.drawBitmap(4, 10, mod_img[2], 104, 30, BLACK);
  else if (waitingForMode) display.drawBitmap(4, 10, mod_img[3], 104, 30, BLACK);
  display.refresh();  
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
    reg[regNum] = reg[regNum] + dot + deci;
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
