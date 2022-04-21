
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
