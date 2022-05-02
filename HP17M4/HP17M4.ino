#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>
#include <Adafruit_SharpMem.h>
#include <Adafruit_DotStar.h>
#include <Keypad.h>
#include "RTC_SAMD51.h"
//#include "DateTime.h"
#include "images.h"

#define PIN_CS       23
#define PIN_BUZZER   A5
#define ROWS          6 
#define COLS          6
#define BLACK         0
#define WHITE         1
#define LAST_REG      9
#define X_OFFSET     20
#define Y_OFFSET     10
#define ROW_HEIGHT   52

#define MODE_CALC     0
#define MODE_F2       1
#define MODE_F3       2
#define MODE_F4       3
#define MODE_F5       4
#define MODE_F6       5

#define KEY_NULL      0
#define KEY_PCT       1
#define KEY_DSP       2
#define KEY_PRT       3 
#define KEY_EXIT      4
#define KEY_INP       5 
#define KEY_CHS       6 
#define KEY_PAREN     7 
#define KEY_BKSP      8 
#define KEY_UP        9 
#define KEY_DN       10 
#define KEY_CLR      11 
#define KEY_EQ       12 
#define KEY_SQUARE   13 
#define KEY_SQRT     14
#define KEY_SHOW     15
#define KEY_INVERSE  16
#define KEY_MEM      17
#define KEY_RND      18
#define KEY_SWAP     19
#define KEY_LAST     20
#define KEY_RDOWN    21
#define KEY_OFF      22
#define KEY_CDATA    23
#define KEY_MATH     24
#define KEY_PRINTER  25
#define KEY_MAIN     26
#define KEY_E        27
#define KEY_SHIFT    28 
#define KEY_STO      29 
#define KEY_RCL      30 
#define KEY_MODES    31 

char keys[ROWS][COLS] = {
  {KEY_STO,  KEY_RCL, KEY_PCT, KEY_DSP, KEY_PRT,   KEY_EXIT},
  {KEY_NULL, KEY_INP, KEY_CHS, '(',     KEY_PAREN, KEY_BKSP},
  {KEY_UP,   '7',     '8',     '9',     KEY_NULL,  '/'},
  {KEY_DN,   '4',     '5',     '6',     KEY_NULL,  '*'},
  {KEY_SHIFT,'1',     '2',     '3',     KEY_NULL,  '-'},
  {KEY_CLR , '0',     '.',     KEY_EQ,  KEY_NULL,  '+'}
};
byte rowPins[ROWS] = {A1,A2,A3,A4,7,2}; 
byte colPins[COLS] = {13,12,11,10,0,4};  

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
RTC_SAMD51 rtc;
Adafruit_SharpMem display(SCK, MOSI, PIN_CS, 400, 240);
Adafruit_DotStar board_dotstar = Adafruit_DotStar(1, 8, 6, DOTSTAR_BGR);

void (*keyFcns[255][6]) ();
String reg[LAST_REG+1];
char shiftKeys[255];
int currentMode = MODE_CALC;
bool shiftPressed = false;
bool waitingToStore = false;
bool waitingToFetch = false;
bool waitingForMode = false;

void setup() {
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, LOW);
  board_dotstar.begin();
  board_dotstar.setPixelColor(0, Adafruit_DotStar::Color(0, 0, 0));  
  board_dotstar.show();
  rtc.begin();
  //to reset time use the below:
  //DateTime now = DateTime(F(__DATE__), F(__TIME__));
  //rtc.adjust(now);
  pinMode(PIN_BUZZER, OUTPUT);
  initFunctions();
  display.begin();
  display.setTextWrap(false);
  display.setFont(&FreeSansBoldOblique24pt7b);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.cp437(true);
  flashLogo();
  display.clearDisplay();
  //attachInterrupt(PIN_CLEAR, interrupt, LOW);
  //set up SAMD51 to enter low power STANDBY mode
  //PM->SLEEPCFG.bit.SLEEPMODE = 0x4;
  //while(PM->SLEEPCFG.bit.SLEEPMODE != 0x4);
  //delay(1000);
  //const uint32_t timeout = 5*1000;
  //uint32_t ts_start = millis();
  //while((!Serial) && ((millis() - ts_start) < timeout)) {}
  Serial.println("Calculator ready!");
  //Serial.println("Input row, text as 0TEXT to test display.");
  //Serial.println("Drawrect as 0304050600 for x, y, w, h, color to test display.");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.printf("Key %c (%d) pressed for mode %d.\n", customKey, customKey, currentMode);
    if (customKey == KEY_SHIFT) {
      shiftPressed = true;
    } else {
      if (shiftPressed) {
        customKey = shiftKeys[customKey];
        Serial.printf(" - Shifted to %d.\n", customKey);
        shiftPressed = false;      
      }
      if (waitingToStore) {
        if (isDigit(customKey)) {
          Serial.printf("TODO: Saving %s to reg[%d].\n", reg[0].c_str(), customKey-48);
        } else {
          buzzerAlert();
        }
        waitingToStore = false;
      } else if (waitingToFetch) {
        if (isDigit(customKey)) {
          Serial.printf("TODO: appending contents of reg[%d] to reg[0].\n", customKey-48);
        } else {
          buzzerAlert();
        }
        waitingToFetch = false;
      } else if (waitingForMode) {
        if (isDigit(customKey)) {
          Serial.printf("TODO: Switching currentMode to %d.\n", customKey-48);
        } else {
          buzzerAlert();
        }
        waitingForMode = false;
      } else if (isDisplayKey(customKey)) {
        addToRegister(customKey);
      } else {
        Serial.printf("calling keyFcns[%d][%d]: ", customKey, currentMode);
        (*keyFcns[customKey][currentMode]) ();
      }
    }
  }
}

/*
 * 
 * 
 * 
 * 
    if (isModifier(customKey) {
      Serial.printf("setting modKey to %d\n", customKey);
      modKey = customKey; 
    } else if (modKey != KEY_NULL) (
      //if it is pow and this is a digit, call power then display result as last token
      //if it is sto and this is a digit, call store
      //if it is rcl and this is a digit, fetch register n and append to reg0
      //if it is shift
      modKey = KEY_NULL;
    } else if {
      //this is a key for display
      modKey = KEY_NULL;
    
    redrawRegisters();
  }

  
  
  digitalWrite(LED_BUILTIN, HIGH);
  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.printf("Key %c (%d) pressed for mode %d. ", customKey, customKey, currentMode);
    
    if ((isDigit(customKey)) &&  {
      if (modKey == KEY_STO) reg[customKey] = reg[0];
      else if (modKey == KEY_RCL) reg[0] = reg[customKey];
      else if (modKey == KEY_MODES) currentMode = customKey;
//      else if (modKey == KEY_POW) ;//TODO call the power command;
    } else
    } else {
//    } else {
//        
//      modKey = KEY_NULL;
    }
  }

  if (Serial.available() > 0) {
    String inp = Serial.readString();
    
    //int row = inp.substring(0,1).toInt();
    //inp = inp.substring(1,inp.length()-1);
    //Serial.printf("%01d%s\n", row, inp.c_str());
    //drawRegText(row, inp);

    int x = inp.substring(0,2).toInt();
    int y = inp.substring(2,4).toInt();
    int w = inp.substring(4,6).toInt();
    int h = inp.substring(6,8).toInt();
    int c = inp.substring(8,10).toInt();
    Serial.printf("%02d%02d%02d%02d%02d\n", x,y,w,h,c);
    display.fillRect(x, y, w, h, c);
    display.refresh();
  }
 * 
 * 
 * 
 * 
 * 
 */
