void initFunctions() { 
  shiftKeys['(']     = KEY_RDOWN;
  shiftKeys[')']     = KEY_SWAP;
  shiftKeys['+']     = KEY_SQUARE;
  shiftKeys['-']     = KEY_SQRT;
  shiftKeys['.']     = KEY_SHOW;
  shiftKeys['/']     = KEY_INVERSE;
  shiftKeys['0']     = KEY_MEM;
  shiftKeys['x']     = KEY_POW;
  shiftKeys[KEY_CLR] = KEY_OFF;
  shiftKeys[KEY_DSP] = KEY_MODES;
  shiftKeys[KEY_EQ]  = KEY_LAST;
  shiftKeys[KEY_INP] = KEY_CDATA;
  shiftKeys[KEY_PCT] = KEY_MATH;
  shiftKeys[KEY_PRT] = KEY_PRINTER;
  shiftKeys[KEY_RCL] = KEY_RND;
  //initiate all unused functions to null
  for (int i=0; i<255; i++) {
    for (int j=0; j<6; j++) {
      keyFcns[i][j] = nullFunc;  
    }
  }
  keyFcns[KEY_OFF][MODE_CALC] = goToSleep;
  keyFcns[KEY_PRT][MODE_CALC] = buzzerAlert;
  keyFcns[KEY_DSP][MODE_CALC] = flashLogo;

  //next digit commands
  //keyFcns[KEY_STO][MODE_CALC] = //display.print "Store in reg no:" then execute store on next digit pressed
  //keyFcns[KEY_RCL][MODE_CALC] = //fetch regNo of next key pressed and append to reg[0]
  
  //last token commands
  //keyFcns[KEY_PCT][MODE_CALC] = //multiply the last token of reg[0] by .01
  //keyFcns[KEY_CHS][MODE_CALC] = //multiply the last token of reg[0] by -1
  //keyFcns[KEY_PAREN][MODE_CALC] = //eval the portion of reg[0] within the parentheses and append result to reg[0]
  //keyFcns[KEY_SQUARE][MODE_CALC] = //square the last token of reg[0]
  //keyFcns[KEY_SQRT][MODE_CALC] = //take the sqrt of the last token of reg[0]

  //other commands
  //keyFcns[KEY_BKSP][MODE_CALC] = //remove one character from reg[0] and repaint
  //keyFcns[KEY_UP][MODE_CALC] = //move reg[n] to reg[n+1] (LAST_REG to 0)
  //keyFcns[KEY_DN][MODE_CALC] = //move reg[n+1] to reg[n] (reg[0] to LAST_REG]
  //keyFcns[KEY_CLR][MODE_CALC] = //set reg[0] to "" and repaint
  //keyFcns[KEY_EQ][MODE_CALC] = //eval reg[0] and set reg[0] = to the result  
  //keyFcns[KEY_SHOW][MODE_CALC] = //show full precision of reg[0] for 5 seconds
  //keyFcns[KEY_INVERSE][MODE_CALC] = //take the inverse of reg[0] and set reg[0] = to the result
  //keyFcns[KEY_RND][MODE_CALC] = //append a random number to the end of reg[0]
  //keyFcns[KEY_SWAP][MODE_CALC] = //swap reg[0] and reg[1]
  //keyFcns[KEY_LAST][MODE_CALC] = //append the value of reg[1] to reg[0]
  //keyFcns[KEY_F1][MODE_CALC] =
  //keyFcns[KEY_F2][MODE_CALC] =
  //keyFcns[KEY_F3][MODE_CALC] =
  //keyFcns[KEY_F4][MODE_CALC] =
  //keyFcns[KEY_F5][MODE_CALC] =
  //keyFcns[KEY_F6][MODE_CALC] =
  //keyFcns[KEY_INP][MODE_CALC] =
  //keyFcns[KEY_MEM][MODE_CALC] = 
  //keyFcns[KEY_MODES][MODE_CALC] =
  //keyFcns[KEY_RDOWN][MODE_CALC] =
  //keyFcns[KEY_CDATA][MODE_CALC] =
  //keyFcns[KEY_MATH][MODE_CALC] =
  //keyFcns[KEY_PRINTER][MODE_CALC] =
  Serial.println("Function pointer array initialized."); 
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


void nullFunc() { Serial.println("Null key somehow was pressed."); }

void clearFunc() { 
  Serial.println("TODO: User pressed clear."); 
  delay(250); //debounce
}

void toggleShift() { 
  shiftPressed = !shiftPressed;
  Serial.printf("Toggled shift to %d\n", shiftPressed); 
  delay(250); //debounce
}

void goToSleep() { 
  Serial.println("TODO: Going to sleep."); 
  shiftPressed = false;
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
//  USBDevice.detach();
//  USBDevice.end();
//  USBDevice.standby();
//  __DSB(); //complete data memory operations
//  __WFI(); //put the SAMDx1 into deep sleep
//  USBDevice.init();
//  USBDevice.attach();
}

void addToRegister(char key) {
  Serial.printf("Adding %c to register.\n", key); 
  reg[0].concat(key);
  reformatRegister(0);
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
