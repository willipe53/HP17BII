#include <Keypad.h>

#define ROWS   3 
#define COLS   10

#define CLR 'C'
#define CE  'E'
#define CHS 'H'
#define ROOT 'R'
#define INV 'N'
#define SQR 'S'
#define MM 'M'
#define MP 'Q'
#define RM 'R'
#define CM 'T'
#define CLR 'C'
 
char keys[ROWS][COLS] = {
  {CE,'.','*','/','-','+',MM,MP,'=','%'},
  {'k','l',SQR,INV,CHS,'P',CM,RM,ROOT,CLR},
  {'9','8','7','6','5','4','3','2','1','0'}
};
byte rowPins[ROWS] = {23,22,21}; 
byte colPins[COLS] = {20,19,18,17,16,15,14,13,12,11}; 

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(115200);
  Serial.println("AWAKE!");
}
  
void loop(){
  char customKey = customKeypad.getKey();
  if (customKey){
    Serial.printf("KEY: %c (%d)\n", customKey, customKey);
  }
}
