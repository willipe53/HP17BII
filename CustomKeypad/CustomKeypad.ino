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
#define ROWS   3 
#define COLS   10
 
char keys[ROWS][COLS] = {
  {'E','b','c','d','e','f','g','h','i','j'},
  {'k','l','m','n','o','p','q','r','s','C'},
  {'u','v','w','x','y','z','1','2','3','4'}
};
byte rowPins[ROWS] = {23,22,21}; 
byte colPins[COLS] = {20,19,18,17,16,15,14,13,12,11}; 

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
  // if (digitalRead(EXIT) == 0) resetFunc();
  // if (digitalRead(CLEAR) == 0) clearFunc();
  // if (digitalRead(SHIFT) == 0) shiftFunc();  
  if (customKey){
    Serial.printf("KEY: %c (%d)\n", customKey, customKey);
  }
  
//resetFunc(); //call reset
}
