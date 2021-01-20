
/*
 * 127 = shift    F = on/clr  Q = F5      
 * 246 = divide   I = input   R = rcl 
 * 241 = chg sgn  J = F1      S = sto
 * B   = bksp     K = F2      T = F6
 * D   = down     M = F3      U = up    
 * E   = exit     N = F4      x = times     
 * L   = display  P = print   # = n/a             
 */

const byte ROWS = 7; 
const byte COLS = 6; 
char keys[ROWS][COLS] = {
  {'J','K','M','N','Q','#'},
  {'S','R','%','L','P','E'},
  {'I','#',240,'(',')','B'},
  {'U','7','#','8','9',245},
  {'D','4','#','5','6','x'},
  {127,'1','#','2','3','-'},
  {'C','0','#','.','=','+'}
};
byte rowPins[ROWS] = {5,11,6,12,22,23,0}; 
byte colPins[COLS] = {13,10,9,21,24,1}; 
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

float (*floatFcns[255]) (float a, float b);
  
void initKeyboard() {
  //defaults
  for (int i=0; i<255; i++) {
    fcns[i][CALC_MODE] = fcns[i][SHFT_MODE] = addKeyToDisplay;
    fcns[i][PIANO_MODE] = playFreqForChar;
    fcns[i][LOGO_MODE]  = buzzerAlert; //default: not implemented
    //fcns[i][GAME_MODE]    = buzzerAlert; //default: not implemented
    //fcns[i][B_MODE]    = buzzerAlert; //default: not implemented
  }
  //button bar
  for (int i=CALC_MODE; i<=SHFT_MODE; i++) {
    fcns['J'][i] = fcns['K'][i] = fcns['M'][i] = fcns[127][i] = fcns['N'][i] = switchToModeForChar;
    fcns['Q'][i] = backSpace; 
  }
  //adjust mode
  fcns['U'][LOGO_MODE] = fcns['D'][LOGO_MODE] = shiftScreenPosition;
  fcns['I'][LOGO_MODE] = saveScreenPosition;
  //calculator (and shift) mode
  fcns['C'][CALC_MODE] = clearRegister;
  fcns['C'][SHFT_MODE] = goToSleep;
  fcns[')'][SHFT_MODE] = swapRegisters;
  fcns['R'][SHFT_MODE] = addRandomNumber;
  fcns['%'][CALC_MODE] = percentLastToken;
  fcns[240][CALC_MODE] = changeSignLastToken;
  fcns['B'][CALC_MODE] = backSpace;
  fcns['U'][CALC_MODE] = shiftRegistersUp;
  fcns[245][SHFT_MODE] = invertLastToken;
  fcns['D'][CALC_MODE] = shiftRegistersDown;
  fcns['-'][SHFT_MODE] = rootLastToken; //sqrt symbol is 250
  fcns['.'][SHFT_MODE] = changeRegisterPrecision;
  fcns['='][CALC_MODE] = evaluateRow;
  fcns['='][SHFT_MODE] = insertLastIntoCurrent;
  fcns['+'][SHFT_MODE] = squareLastToken; //subscript 2 symbol is 252
  fcns['x'][SHFT_MODE] = addSuperscriptToDisplay;
  fcns['P'][CALC_MODE] = toggleCommasInDisplay;
  fcns['P'][SHFT_MODE] = loopThroughHighASCII;
  //game mode
  fcns['7'][GAME_MODE] = moveGun;
  fcns['8'][GAME_MODE] = fireGun;
  fcns['9'][GAME_MODE] = moveGun;
  fcns['C'][GAME_MODE] = startGame;
  //fcns['1'][GAME_MODE] = invaderDeath;
  //fcns['2'][GAME_MODE] = playerDeath;
  //fcns['0'][GAME_MODE] = dropBomb;
  //fcns['='][GAME_MODE] = gameOver;  

  floatFcns[251] = powerTokens;
  floatFcns['+'] = addTokens;
  floatFcns['-'] = subtractTokens;
  floatFcns['x'] = multiplyTokens;
  floatFcns[245] = divideTokens;
}

float powerTokens   (float a, float b) { return pow(a,b); }
float addTokens     (float a, float b) { return a + b;    }
float subtractTokens(float a, float b) { return a - b;    }
float multiplyTokens(float a, float b) { return a * b;    }
float divideTokens  (float a, float b) { return a / b;    }

void switchToModeForChar(char key) {
  int newMode = CALC_MODE;
  if (key == 'K') newMode = PIANO_MODE;
  else if (key == 'M') newMode = LOGO_MODE;
  else if (key == 'N') newMode = GAME_MODE;
  //else if (key == 'Q') newMode = B_MODE; //used for broken backspace key
  else if (key == 127) newMode = SHFT_MODE;
  if (newMode != currentMode) switchToMode(newMode);
  Serial.printf("Mode switched to %d (%c)\n", currentMode, key);
}

char getKeyPressed() {
  return customKeypad.getKey();
}

void loopThroughHighASCII(char c) {
  int textSize = 3;
  display.setTextSize(textSize);
  display.setTextColor(BLACK);
  for (int i=240; i<256; i++) {
    display.clearDisplay();
    display.setCursor(10,startY+MAIN_H-24);
    display.write(i);
    display.print(" ");
    display.print(i);
    display.refresh();
    delay(1000);
  }
}

void addSuperscriptToDisplay(char key) {
  addKeyToDisplay(251);
}

void addKeyToDisplay(char key) {
  Serial.printf("Added %c (%d) to display\n", key, key);
  if (resultPrinted) {
    if (isValidFloatChar(key, lastWasOperator)) {
      registers[VISIBLE_ROWS-1] = "";
      shiftRegistersUp(0);      
    }
    resultPrinted = false;
  }
  tokenize(key);
  if (tokenCount > 3) evaluateRow(key);
  else redrawDisplay();
}

void goToSleep(char c) {
  Serial.print("Going to sleep.  Press reset to restart.\n");
  display.clearDisplay();
  display.refresh();
  int sleepMS = Watchdog.sleep();
}

void clearRegister(char key) {
  registers[0] = "";    
  redrawDisplay();
}

void swapRegisters(char key) {
  swapRegisters(0, 1);
}

void swapRegisters(int a, int b) {
  Serial.printf("Swapping registers %d and %d\n", a, b);  
  String temp = registers[a];
  registers[a] = registers[b];
  registers[b] = temp;
  redrawDisplay();
}

void addRandomNumber(char key) {
  randomSeed(seed);
  float r = random(111111,999999) / 1000000.0;
  Serial.printf("Generating random number %f\n", r);
  registers[0] += String(r, currentPrecis);
  redrawDisplay();
}

void backSpace(char key) {
  Serial.printf("Back-spacing\n");    
  int newLineLength = registers[0].length()-1;
  if (newLineLength < 1) registers[0] = "";
  else registers[0] = registers[0].substring(0,newLineLength);
  redrawDisplay();
}

void shiftRegistersDown(char key) {
  Serial.printf("Shifting registers down\n");   
  String store = registers[0];
  for (int i=0; i<VISIBLE_ROWS-1; i++) {
    registers[i] = registers[i+1];
  }  
  registers[VISIBLE_ROWS-1] = store;
  redrawDisplay();
}

void shiftRegistersUp(char key) {
  Serial.printf("Shifting registers up\n");    
  String store = registers[VISIBLE_ROWS-1];
  for (int i=VISIBLE_ROWS-1; i>0; i--) {
    registers[i] = registers[i-1];
  }  
  registers[0] = store;
  redrawDisplay();
}

void rootLastToken(char key) {
  tokenize(key);
  Serial.printf("Taking sqrt of %f\n", floats[tokenCount-2]);    
  updateLastToken(sqrt(floats[tokenCount-2]));
}

void squareLastToken(char key) {
  tokenize(key);
  Serial.printf("Squaring %f\n", floats[tokenCount-2]);    
  updateLastToken(floats[tokenCount-2] * floats[tokenCount-2]);  
}

void percentLastToken(char key) {
  tokenize(key);
  Serial.printf("Applying percentage to %f\n", floats[tokenCount-2]);    
  updateLastToken(floats[tokenCount-2] / 100.0);
}

void changeSignLastToken(char key) {
  tokenize(key);
  Serial.printf("Changing sign of %f\n", floats[tokenCount-2]);    
  updateLastToken(floats[tokenCount-2] * -1.0);
}

void invertLastToken(char key) {
  tokenize(key);
  Serial.printf("Inverting %f\n", floats[tokenCount-2]);    
  updateLastToken(1.0 / floats[tokenCount-2]);
}

void changeRegisterPrecision(char key) {
  if (currentPrecis == FULL_PRECIS) currentPrecis = displayPrecis;
  else currentPrecis = FULL_PRECIS;
  Serial.printf("Showing precision as %d\n", currentPrecis);
  float precisFloat = getPrecisFloat(registers[0]);    
  registers[0] = String(precisFloat, currentPrecis);
  redrawDisplay();
}

void updateLastToken(float f) {
  floats[tokenCount-2] = f;
  savePrecisFloat(f);
  Serial.printf("f:%f\n", floats[tokenCount-2]);    
  registers[0] = registers[0].substring(0,lastOperandStart);
  registers[0] += String(floats[tokenCount-2], currentPrecis);
  Serial.printf("f:%f tc:%d ftc2:%f\n", f, tokenCount, floats[tokenCount-2]);    
  redrawDisplay();  
}

void insertLastIntoCurrent(char key) {
  Serial.printf("Inserting last into register\n");    
  registers[0] += registers[1];
  redrawDisplay();
}

void evaluateRow(char key) {
  tokenize(key);
  Serial.printf("Evalutating row: (%f, %c, %f)\n", floats[0], tokens[1], floats[2]);   
  float result = 0;
  bool success = false;
    result = (*floatFcns[tokens[1]]) (floats[0], floats[2]);
    tokenCount = 1;
    floats[0] = result;
    savePrecisFloat(result);
    registers[0] = String(result, currentPrecis);
    tokens[0] = 'F';
    if (key != '=') registers[0] += key;
    else resultPrinted = true;     
    redrawDisplay();
}

bool isValidFloatChar(char c, bool lastWasOperator) {
  if ((c == '-') && (lastWasOperator)) return true;
  if ((c == '.') || (c >= '0' && c <= '9')) return true;
  return false;
}

void tokenizeRegister(char key) {
  registers[1] = registers[2] = "";
  tokenize(key);
  for (int i=0; i<tokenCount; i++) {
    registers[1] += tokens[i]; 
    registers[2] += String(floats[i]) + "|"; 
  }
  redrawDisplay();
}

void tokenize(char key) {
  registers[0] += key;
  bool lastWasOperator = true;
  tokenCount = 0;
  int fStart = 0;
  for (int i=0; i<registers[0].length(); i++) {
    char c = registers[0].charAt(i);
    if (isValidFloatChar(c,lastWasOperator)) {
      if (lastWasOperator) {
        fStart = i;
        lastWasOperator = false;
      }
    } else {
      tokens[tokenCount] = 'F';
      floats[tokenCount++] = registers[0].substring(fStart, i).toFloat();
      lastOperandStart = fStart;
      tokens[tokenCount++] = c;
      lastWasOperator = true;
    }
  }
  for (int i=0; i<tokenCount; i++) {
    Serial.printf("tc=%d token[%d]=%c  floats[%d]=%f lastOperandStart=%d (%s)\n", tokenCount, i, tokens[i], i, floats[i], lastOperandStart, registers[0].c_str());
  }
}  

float getPrecisFloat(String s) {
  for (int i=0; i<tokenCount; i++) {
    Serial.printf("Testing string %s against stored precision float %f\n", s.c_str(), precisFloats[i]);    
    if (precisFloats[i] - s.toFloat() < .01) {
      Serial.printf("Found precision float %f for %s\n", precisFloats[i], s.c_str());    
      return precisFloats[i];
    }
  }
  Serial.printf("Unable to find precision float for %s\n", s.c_str());    
  return s.toFloat();
}

void savePrecisFloat(float precisF) {
  static int idx = 0;
  Serial.printf("Saving precision float %f at idx %d\n", precisF, idx);    
  precisFloats[idx++] = precisF;
  if (idx > 39) idx = 0;
}
