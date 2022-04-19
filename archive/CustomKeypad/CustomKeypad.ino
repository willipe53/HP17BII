/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

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


void(* resetFunc) (void) = 0;//declare reset function at address 0

void clearFunc() { Serial.println("User pressed clear."); }

void shiftFunc() { Serial.println("User pressed shift."); }

void setup(){
  Serial.begin(115200);
  pinMode(EXIT, INPUT_PULLUP);
  pinMode(CLEAR, INPUT_PULLUP);
  pinMode(SHIFT, INPUT_PULLUP);
  
  while (!(Serial)) {}
  Serial.println("AWAKE!");

//  for (int i=0; i<ROWS; i++) {
//   pinMode(rowPins[i], INPUT);
//  }
//  for (int i=0; i<COLS; i++) {
//   pinMode(colPins[i], INPUT);
//  }
}
  
void loop(){
  char customKey = customKeypad.getKey();
  if (digitalRead(EXIT) == 0) resetFunc();
  if (digitalRead(CLEAR) == 0) clearFunc();
  if (digitalRead(SHIFT) == 0) shiftFunc();  
  if (customKey){
    Serial.printf("KEY: %c (%d)\n", customKey, customKey);
  }
  
//resetFunc(); //call reset
}
