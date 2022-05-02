void initFunctions() { 
  shiftKeys['(']      = KEY_RDOWN;
  shiftKeys[')']      = KEY_SWAP;
  shiftKeys['+']      = KEY_SQUARE;
  shiftKeys['-']      = KEY_SQRT;
  shiftKeys['.']      = KEY_SHOW;
  shiftKeys['/']      = KEY_INVERSE;
  shiftKeys['0']      = KEY_MEM;
  shiftKeys['*']      = '^';
  shiftKeys[KEY_CLR]  = KEY_OFF;
  shiftKeys[KEY_DSP]  = KEY_MODES;
  shiftKeys[KEY_EQ]   = KEY_LAST;
  shiftKeys[KEY_INP]  = KEY_CDATA;
  shiftKeys[KEY_PCT]  = KEY_MATH;
  shiftKeys[KEY_PRT]  = KEY_PRINTER;
  shiftKeys[KEY_RCL]  = KEY_RND;
  shiftKeys[KEY_EXIT] = KEY_MAIN;
  shiftKeys[KEY_CHS]  = KEY_E;
  
  //initiate all unused functions to null
  for (int i=0; i<255; i++) {
    for (int j=0; j<6; j++) {
      keyFcns[i][j] = nullFunc;  
    }
  }
  
  //wait for input commands
  keyFcns[KEY_STO][MODE_CALC] = storeValue; //store reg[0] to reg[n] based on next char typed
  keyFcns[KEY_RCL][MODE_CALC] = recallValue; //fetch reg[n] based on next char typed, and append to reg[0]
  keyFcns[KEY_MODES][MODE_CALC] = changeMode; //reset the mode based on next char typed
  
  //last token commands
  //keyFcns[KEY_PCT][MODE_CALC] = //multiply the last token of reg[0] by .01
  //keyFcns[KEY_PAREN][MODE_CALC] = //eval the portion of reg[0] within the parentheses and append result to reg[0]
  //keyFcns[KEY_SQUARE][MODE_CALC] = //square the last token of reg[0]
  //keyFcns[KEY_SQRT][MODE_CALC] = //take the sqrt of the last token of reg[0]

  //compute commands
  keyFcns[KEY_DSP][MODE_CALC] = flashLogo;
  keyFcns[KEY_PRT][MODE_CALC] = buzzerAlert;
  //keyFcns[KEY_INVERSE][MODE_CALC] = //take the inverse of reg[0] and set reg[0] = to the result
  //keyFcns[KEY_CHS][MODE_CALC] = //multiply the last token of reg[0] by -1
  //keyFcns[KEY_BKSP][MODE_CALC] = //remove one character from reg[0] and repaint
  //keyFcns[KEY_UP][MODE_CALC] = //move reg[n] to reg[n+1] (LAST_REG to 0)
  //keyFcns[KEY_DN][MODE_CALC] = //move reg[n+1] to reg[n] (reg[0] to LAST_REG]
  //keyFcns[KEY_CLR][MODE_CALC] = //set reg[0] to "" and repaint
  //keyFcns[KEY_EQ][MODE_CALC] = //eval reg[0] and set reg[0] = to the result  
  //keyFcns[KEY_SHOW][MODE_CALC] = //show full precision of reg[0] for 5 seconds
  //keyFcns[KEY_SWAP][MODE_CALC] = //swap reg[0] and reg[1]

  //add float value to register
  //keyFcns[KEY_RND][MODE_CALC] = //append a random number to the end of reg[0]
  //keyFcns[KEY_LAST][MODE_CALC] = //append the value of reg[1] to reg[0]
  
  //unused commands
  //keyFcns[KEY_INP][MODE_CALC] =
  //keyFcns[KEY_MEM][MODE_CALC] = 
  //keyFcns[KEY_RDOWN][MODE_CALC] =
  //keyFcns[KEY_CDATA][MODE_CALC] =
  //keyFcns[KEY_MATH][MODE_CALC] =
  //keyFcns[KEY_PRINTER][MODE_CALC] =
  //keyFcns[KEY_MAIN][MODE_CALC] =
  //keyFcns[KEY_EXIT][MODE_CALC] =
  //keyFcns[KEY_E][MODE_CALC] =
  //keyFcns[KEY_OFF][MODE_CALC] = goToSleep;

  Serial.println("Function pointer array initialized."); 
}

void nullFunc() { Serial.println("Key function not implemented."); }
void storeValue() { waitingToStore = true; }
void recallValue() { waitingToFetch = true; }
void changeMode() { waitingForMode = true; }

bool isDisplayKey(char key) {
  if (key < 40) return false;
  return true;
}

void clearFunc() { 
  Serial.println("TODO: User pressed clear."); 
  delay(250); //debounce
}

void toggleShift() { 
  Serial.printf("TODO: Toggled shift to x\n"); 
  delay(250); //debounce
}

void addToRegister(char key) {
  Serial.printf("Adding %c to register.\n", key); 
  reg[0].concat(key);
  //reformatRegister(0);
  redrawRegisters();
} 

void buzzerAlert() {
  Serial.printf("Buzzing the buzzer\n");    
  tone(PIN_BUZZER, 830);
  delay(100);    
  tone(PIN_BUZZER, 784);
  delay(100);    
  tone(PIN_BUZZER, 466);
  delay(200);    
  noTone(PIN_BUZZER);
}

void dateTimeFunctionTODO() {
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();  
}

void loopThroughHighASCII() {
  //int textSize = 3;
  //display.setTextSize(textSize);
  //display.setTextColor(BLACK);
  for (int i=32; i<256; i++) {
    display.clearDisplay();
    display.setCursor(10,100);
    display.write(i);
    display.print("::");
    display.print(i);
    display.refresh();
    delay(1000);
  }
}

void goToSleep() { 
  Serial.println("TODO: Going to sleep."); 
//  USBDevice.detach();
//  USBDevice.end();
//  USBDevice.standby();
//  __DSB(); //complete data memory operations
//  __WFI(); //put the SAMDx1 into deep sleep
//  USBDevice.init();
//  USBDevice.attach();
}
