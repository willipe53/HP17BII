# HP17BII

This is the code I created in the Arduino IDE to drive a hollowed-out old HP 17BII calculator.  It uses an Adafruit Feather M4 Express chip to drive a Sharp monochrome memory display and a hand-made button-and-diode matrix that I mounted under the keyboard.

![HP17Bii.png](../images/HP17Bii.png)


## Calculator Modes

1. Calculator (performs as you might expect)
2. Piano (plays notes on the peizo buzzer)
3. Logo (shows a custom "Hacked by Peter" logo)
4. Game (a rudimentary version of Space Invaders)

If you don't have the Arduino GUI just change the *.ino files to *.cpp files and open them in any C++ IDE.

## Basic Calculator Tokenizer

1. `loop()` in `HP17B41.ino` waits for a key press with `char key = getKeyPressed()`.
2. It then calls the function associated with that key: `(*fcns[key][currentMode]) (key)`. The default function for any key is `addKeyToDisplay` which can be seen in `initKeyboard()` function in `keyboard.ino`.
3. When `=` is pressed, it will call the `evaluateRow` function.  `evaluateRow` will call `tokenize` which splits up the contents of the calculator's display stored in `register[0]` into tokens.  If the string is valid for an arithmatic operation, such as `2+3` it will convert the first and last tokens into float values, then call the function associated with the operator.  In this example it would call `result = (*floatFcns['+']) (2.0, 3.0)` which with any luck would return 5.
4. The `tokenize` function is also in `keyboard.ino`.  It loops thorough `register[0]` character by character, figuring out which parts are numbers with `isValidFloatChar` and assuming other characters are operators.
5. An operator needs to have a float function defined in `initKeyboard`.  In our example, `floatFcns['+'] = addTokens` which is simple enough: `float addTokens (float a, float b) { return a + b; }`
