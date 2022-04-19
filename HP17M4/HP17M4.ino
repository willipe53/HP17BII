#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Keypad.h>

#define SHARP_SCK   SCK
#define SHARP_MOSI MOSI
#define SHARP_SS     23
#define CLEAR         9
#define SHIFT         3
#define BUZZER       A5
#define ROWS          7 
#define COLS          6
#define BLACK         0
#define WHITE         1

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
byte rowPins[ROWS] = {A0,A1,A2,A3,A4,7,2}; 
byte colPins[COLS] = {13,12,11,10,0,4};  

bool exitPressed = false;
bool clearPressed = false;
bool shiftPressed = false;

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void clearFunc() { Serial.println("User pressed clear."); }

void shiftFunc() { Serial.println("User pressed shift."); buzzerAlert(0); }

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
  pinMode(BUZZER, OUTPUT);
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
