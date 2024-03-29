#include <SPI.h>
#include <Keypad.h>

#define CS    10
#define EXIT   4
#define CLEAR  1
#define SHIFT  0
#define ROWS   7 
#define COLS   6
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
 
char keys[ROWS][COLS] = {
  {'F','G','H','J','K','L'},
  {'S','R','%','L','P','u'},
  {'c','I',240,'(',')','A'},
  {'U','7','8','9','v',245},
  {'D','4','5','6','w','x'},
  {'b','1','2','3','y','-'},
  {'a','0','.','=','z','+'}
};
byte rowPins[ROWS] = {19,22,23,5,18,7,15}; 
byte colPins[COLS] = {21,3,20,6,17,8}; 

bool exitPressed = false;
bool clearPressed = false;
bool shiftPressed = false;

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 



// MLCD write line command 0x80, reversed 0x01
// MLCD clear memory command 0x20, reversed 0x04
// MLCD static mode command 0x00 
// MLCD VCOM bit 0x40 , reversed 0x02

byte Vcom;
byte Line;

//void(* resetFunc) (void) = 0;//declare reset function at address 0

//void resetFunc() { Serial.println("User pressed reset."); CPU_RESTART ;}

void resetFunc() { Serial.println("User pressed reset."); }

void clearFunc() { Serial.println("User pressed clear."); }

void shiftFunc() { Serial.println("User pressed shift."); }

void clearDisplay() {
  digitalWrite(CS,HIGH);
  SPI.transfer(0x04 | Vcom);
  //Serial.print("clear screen ");
  //Serial.println(0x04 | Vcom);
  SPI.transfer(0x00);
  digitalWrite(CS,LOW);
}

void drawLine(byte lineNumber) {
  
  digitalWrite(CS,HIGH);
  SPI.transfer(0x01 | Vcom); //write line command (bits reversed)
  SPI.transfer(lineNumber);
  for (int i = 0; i <50; i++) {
    SPI.transfer(0b10010010);
  }
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.endTransaction();
  digitalWrite(CS,LOW);
}

void setup() {
  
  pinMode(EXIT, INPUT_PULLUP);
  pinMode(CLEAR, INPUT_PULLUP);
  pinMode(SHIFT, INPUT_PULLUP);
  pinMode (10, OUTPUT);
  SPI.begin(); 
  SPI.beginTransaction(SPISettings(2000000, LSBFIRST, SPI_MODE0));
  
  clearDisplay();  
  delay(1000);  
}

void loop() {
  
  Vcom = Vcom ? 0x00 : 0x02;

  if (Line < 240) {Line++;} else {Line =0; clearDisplay();}
  
  //Serial.print(Vcom);
  //Serial.print(" ");
  //Serial.println(Line);
  
  drawLine(Line);
  delay(30);
  char customKey = customKeypad.getKey();
  if (digitalRead(EXIT) == 0) resetFunc();
  if (digitalRead(CLEAR) == 0) clearFunc();
  if (digitalRead(SHIFT) == 0) shiftFunc();  
  if (customKey){
    Serial.printf("KEY: %c (%d)\n", customKey, customKey);
  }
}
