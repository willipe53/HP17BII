#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Keypad.h>

#define PIN_CS       23
#define PIN_CLEAR     9
#define PIN_SHIFT     3
#define PIN_BUZZER   A5
#define ROWS          7 
#define COLS          6
#define BLACK         0
#define WHITE         1
#define LAST_REG      9

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

Adafruit_SharpMem display(SCK, MOSI, PIN_CS, 400, 240);

char keys[ROWS][COLS] = {
  {KEY_F1,   KEY_F2,  KEY_F3,  KEY_F4,  KEY_F5,    KEY_F6},
  {KEY_STO,  KEY_RCL, KEY_PCT, KEY_DSP, KEY_PRT,   KEY_NULL},
  {KEY_NULL, KEY_INP, KEY_CHS, '(',     KEY_PAREN, KEY_BKSP},
  {KEY_UP,   '7',     '8',     '9',     KEY_NULL,  '/'},
  {KEY_DN,   '4',     '5',     '6',     KEY_NULL,  'x'},
  {KEY_NULL, '1',     '2',     '3',     KEY_NULL,  '-'},
  {KEY_NULL, '0',     '.',     '=',     KEY_NULL,  '+'}
};
byte rowPins[ROWS] = {A0,A1,A2,A3,A4,7,2}; 
byte colPins[COLS] = {13,12,11,10,0,4};  

char shiftKeys[255];

bool clearPressed = false;
bool shiftPressed = false;
int currentMode = MODE_CALC;

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void (*keyFcns[255][6]) ();
String reg[LAST_REG+1];

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
  pinMode(PIN_CLEAR, INPUT_PULLUP);
  pinMode(PIN_SHIFT, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.begin(9600);
  initFunctions();
  display.begin();
  display.clearDisplay();
  testdrawline();
  delay(500);
  display.clearDisplay();
  Serial.println("Calculator ready!");
}

void loop() {
  char customKey = customKeypad.getKey();
  if (digitalRead(PIN_SHIFT) == 0) toggleShift();  
  if (digitalRead(PIN_CLEAR) == 0) {
    if (shiftPressed) goToSleep();
    else customKey = KEY_CLR;
    delay(250);
  }
  if (customKey){
    if (shiftPressed) customKey = shiftKeys[customKey];
    if (customKey < '(') {
      Serial.printf("calling keyFcns[%d][%d]\n", customKey, currentMode);
      (*keyFcns[customKey][currentMode]) ();
    } else {
      addToRegister(customKey);
    }
    shiftPressed = false;
  }
}
