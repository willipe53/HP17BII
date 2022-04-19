#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Keypad.h>

#define SHARP_SCK  13
#define SHARP_MOSI 12
#define SHARP_SS   27
#define CLEAR      16
#define SHIFT      17
#define ROWS        7 
#define COLS        6
#define BLACK       0
#define WHITE       1

Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 400, 240);

int minorHalfSize; // 1/2 of lesser of display width or height

char keys[ROWS][COLS] = {
  {'F','G','H','J','K','L'},
  {'S','R','%','L','P','u'},
  {'c','I',240,'(',')','A'},
  {'U','7','8','9','v',245},
  {'D','4','5','6','w','x'},
  {'b','1','2','3','y','-'},
  {'a','0','.','=','z','+'}
};
byte rowPins[ROWS] = {22,15,33,18,23,4,39}; 
byte colPins[COLS] = {32,19,14,5,21,34}; 

bool exitPressed = false;
bool clearPressed = false;
bool shiftPressed = false;

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void clearFunc() { Serial.println("User pressed clear."); }

void shiftFunc() { Serial.println("User pressed shift."); }

void testdrawline() {
  for (int i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, BLACK);
    display.refresh();
  }
  for (int i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, BLACK);
    display.refresh();
  }
  delay(250);
}

void setup() {
  pinMode(CLEAR, INPUT_PULLUP);
  pinMode(SHIFT, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Hello!");

  // start & clear the display
  display.begin();
  display.clearDisplay();
  minorHalfSize = min(display.width(), display.height()) / 2;
  testdrawline();
  delay(500);
  display.clearDisplay();
}

void loop() {
  char customKey = customKeypad.getKey();
  if (digitalRead(CLEAR) == 0) clearFunc();
  if (digitalRead(SHIFT) == 0) shiftFunc();  
  if (customKey){
    Serial.printf("KEY: %c (%d)\n", customKey, customKey);
  }
}
