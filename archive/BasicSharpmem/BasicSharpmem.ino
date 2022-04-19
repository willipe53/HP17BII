#include <SPI.h>
#include <Keypad.h>

#define CS    10
#define EXIT   2
#define CLEAR 20
#define SHIFT 12
#define ROWS   7 
#define COLS   6
 
char keys[ROWS][COLS] = {
  {'F','G','H','I','J','K'},
  {'S','R','%','L','P','u'},
  {'c','I',240,'(',')','A'},
  {'U','7','8','9','v',245},
  {'D','4','5','6','w','x'},
  {'b','1','2','3','y','-'},
  {'a','0','.','=','z','+'}
};
byte rowPins[ROWS] = {7,19,17,4,3,1,0}; 
byte colPins[COLS] = {21,22,23,8,6,5}; 

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

void setup() {
  
  pinMode (10, OUTPUT);
  SPI.begin(); 
  SPI.beginTransaction(SPISettings(2000000, LSBFIRST, SPI_MODE0));
  
  clearDisplay();  
  delay(1000);  
}

void loop() {
  
  Vcom = Vcom ? 0x00 : 0x02;

  if (Line < 240) {Line++;} else {Line =0; clearDisplay();}
  
  Serial.print(Vcom);
  Serial.print(" ");
  Serial.println(Line);
  
  drawLine(Line);
  delay(30);
}

void clearDisplay() {
  digitalWrite(CS,HIGH);
  SPI.transfer(0x04 | Vcom);
  Serial.print("clear screen ");
  Serial.println(0x04 | Vcom);
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
