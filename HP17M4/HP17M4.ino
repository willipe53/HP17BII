#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>
//FreeSansBoldOblique12pt7b.h
#include <Adafruit_SharpMem.h>
#include <Adafruit_DotStar.h>
#include <Keypad.h>
#include "RTC_SAMD51.h"
//#include "DateTime.h"
#include "images.h"

#define PIN_CS       23
#define PIN_CLEAR     9
#define PIN_SHIFT     3
#define PIN_BUZZER   A5
#define ROWS          7 
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
#define KEY_F1        1
#define KEY_F2        2
#define KEY_F3        3
#define KEY_F4        4
#define KEY_F5        5
#define KEY_F6        6
#define KEY_STO       7
#define KEY_RCL       8
#define KEY_PCT       9
#define KEY_DSP      10
#define KEY_PRT      11 
#define KEY_INP      12 
#define KEY_CHS      13 
#define KEY_PAREN    14 
#define KEY_BKSP     15 
#define KEY_UP       16 
#define KEY_DN       17 
#define KEY_CLR      18 
#define KEY_EQ       19 
#define KEY_SQUARE   20 
#define KEY_SQRT     21
#define KEY_SHOW     22
#define KEY_INVERSE  23
#define KEY_MEM      24
#define KEY_RND      25
#define KEY_MODES    26
#define KEY_SWAP     27
#define KEY_LAST     28
#define KEY_RDOWN    29
#define KEY_OFF      30
#define KEY_CDATA    31
#define KEY_MATH     32
#define KEY_PRINTER  33
#define KEY_POW      34

Adafruit_SharpMem display(SCK, MOSI, PIN_CS, 400, 240);
Adafruit_DotStar board_dotstar = Adafruit_DotStar(1, 8, 6, DOTSTAR_BGR);

char keys[ROWS][COLS] = {
  {KEY_F1,   KEY_F2,  KEY_F3,  KEY_F4,  KEY_F5,    KEY_F6},
  {KEY_STO,  KEY_RCL, KEY_PCT, KEY_DSP, KEY_PRT,   KEY_NULL},
  {KEY_NULL, KEY_INP, KEY_CHS, '(',     KEY_PAREN, KEY_BKSP},
  {KEY_UP,   '7',     '8',     '9',     KEY_NULL,  '/'},
  {KEY_DN,   '4',     '5',     '6',     KEY_NULL,  'x'},
  {KEY_NULL, '1',     '2',     '3',     KEY_NULL,  '-'},
  {KEY_NULL, '0',     '.',     KEY_EQ,  KEY_NULL,  '+'}
};
byte rowPins[ROWS] = {A0,A1,A2,A3,A4,7,2}; 
byte colPins[COLS] = {13,12,11,10,0,4};  

char shiftKeys[255];

bool clearPressed = false;
bool shiftPressed = false;
int currentMode = MODE_CALC;

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
RTC_SAMD51 rtc;

void (*keyFcns[255][6]) ();
String reg[LAST_REG+1];

void interrupt() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  board_dotstar.begin();
  board_dotstar.setPixelColor(0, Adafruit_DotStar::Color(0, 0, 0));  
  board_dotstar.show();
  rtc.begin();
  //to reset time use the below:
  //DateTime now = DateTime(F(__DATE__), F(__TIME__));
  //rtc.adjust(now);
  pinMode(PIN_CLEAR, INPUT_PULLUP);
  pinMode(PIN_SHIFT, INPUT_PULLUP);
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
  attachInterrupt(PIN_CLEAR, interrupt, LOW);

  //set up SAMD51 to enter low power STANDBY mode
  //PM->SLEEPCFG.bit.SLEEPMODE = 0x4;
  //while(PM->SLEEPCFG.bit.SLEEPMODE != 0x4);
  //delay(1000);
  //const uint32_t timeout = 5*1000;
  //uint32_t ts_start = millis();
  //while((!Serial) && ((millis() - ts_start) < timeout)) {}
  Serial.println("Calculator ready!");
  Serial.println("Input row, text as 0TEXT to test display.");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  char customKey = customKeypad.getKey();
  if (digitalRead(PIN_SHIFT) == 0) toggleShift();  
  if (digitalRead(PIN_CLEAR) == 0) {
    if (shiftPressed) goToSleep();
    else customKey = KEY_CLR;
    delay(250);
  }
  if (customKey){
    if (shiftPressed) customKey = shiftKeys[customKey];
    Serial.printf("Key %c (%d) pressed for mode %d. ", customKey, customKey, currentMode);
    if (customKey < '(') {
      Serial.printf("calling keyFcns[%d][%d]: ", customKey, currentMode);
      (*keyFcns[customKey][currentMode]) ();
    } else {
      addToRegister(customKey);
    }
    shiftPressed = false;
  }

  if (Serial.available() > 0) {
    String inp = Serial.readString();
    int row = inp.substring(0,1).toInt();
    inp = inp.substring(1,inp.length()-1);
    Serial.printf("%01d%s\n", row, inp.c_str());
    testDisplayText2(row, inp);
  }  
}
