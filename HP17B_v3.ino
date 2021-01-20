#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <FlashStorage.h>
#include <math.h>
#include <Adafruit_SleepyDog.h>
#include <Ticker.h>

#define SHARP_SCK   17
#define SHARP_MOSI  18
#define SHARP_SS    19
#define BUZZER      A0
#define BLACK        0
#define WHITE        1
#define MAIN_H     100
#define CALC_MODE    0
#define PIANO_MODE   1
#define LOGO_MODE    2
#define GAME_MODE    3
#define B_MODE       4 //not used, overloaded for backspace key
#define SHFT_MODE    5
#define VISIBLE_ROWS 4
#define FULL_PRECIS 13

Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 400, 240);
FlashStorage(start_y_flash, int);
GFXcanvas1 gameCanvas(400, 240); 

int displayPrecis = 2;
int currentPrecis = 2;
int startY;
int currentMode = CALC_MODE;
int tokenCount = 0;
int lastOperandStart = 0;
int gunX = 200;
int gunY = startY+97;
int bulletX = -1;
int bulletY = -1;
int bombX = -1;
int bombY = -1;
int alienX = 112;
int alienY;
int barrierX[4] = {130, 170, 210, 250};
int barrierY = startY+70;
int barrierW = 20;
int barrierH = 15;
int frontLineAliens[10] = {20,21,22,23,24,25,26,27,28,29};
int numFrontLineAliens = 10;
int gameScore = 0;
int toneFreq[100];
unsigned long seed;
unsigned long toneStart[100];
unsigned long toneEnd[100];
unsigned long gameStartTime;
bool lastWasOperator = true;
bool resultPrinted = false;
bool aliensOpen = false;
bool goingLeft = false;
bool gameStarted = false;
int barrierState[4][12]; //0=none, 1=broken, 2=solid
bool deadAliens[30];
char tokens[40];
float floats[40];
float precisFloats[40];
String registers[VISIBLE_ROWS];
void (*fcns[255][6]) (char key);

void switchToMode(int m) {
  currentMode = m;
  redrawDisplay();
}

void updateSeed() {
  if (seed > 4000000000) seed = 0;
  seed += millis();
}

void setup(){
  Serial.begin(115200);
  startY = start_y_flash.read();
  if (startY < 10) {
    startY = 113;
    saveScreenPosition(0);
  }
  initDisplay();
  initKeyboard();
  gunY = startY+97;
  alienY = startY+24;
  barrierY = startY+70;
  alienX = 100;
  switchToMode(CALC_MODE);
  Serial.print("HP 17B II ready to go!\n");
}

String serialCommand;
int toneIdx;
int lastToneIdx;
  
void loop() {
  char key = getKeyPressed();
  static int shiftCounter = 0;
  //calculator
  if (key != NO_KEY) {
    updateSeed();
    Serial.printf("%c (mode:%d)\n", key, currentMode);
    (*fcns[key][currentMode]) (key);
    if (currentMode == SHFT_MODE) {
      drawShiftIndicator(true);
      shiftCounter++;
    }
    if (shiftCounter > 1) {
      drawShiftIndicator(false);
      currentMode = CALC_MODE;
      shiftCounter = 0;
    }
  }
  //space invaders
  if (currentMode == GAME_MODE) {
    drawPlayArea();
    unsigned long nowSec = millis()/1000;
    static unsigned long lastJumpSec;
    aliensOpen = (nowSec % 2); //alien legs open on odd seconds
    if (nowSec > lastJumpSec + 1) {
      if (goingLeft) {
        if (gameStarted) alienX -= 5;      
        if (alienX < 95) {
          goingLeft = false;
          if (gameStarted) alienY += 20;
        }
      } else {
        if (gameStarted) alienX += 5;              
        if (alienX > 150) {
          goingLeft = true;
          if (gameStarted) alienY += 20;
        }
      }
      lastJumpSec = nowSec;
      if (alienY + 10 >= gunY) gameOver(0);
    }
  }
  //sound thread
  toneIdx = toneScheduledFor(millis());
  if (toneFreq[toneIdx] > 0) {
    if (toneIdx != lastToneIdx) {
      tone(BUZZER, toneFreq[toneIdx]);
      lastToneIdx = toneIdx;
    }
  } else {
    noTone(BUZZER);    
  }
 
}
