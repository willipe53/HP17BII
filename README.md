# HP17BII

This is the code I created in the Arduino IDE to drive a hollowed-out old HP 17BII calculator.  It uses an Adafruit Feather M4 Express chip to drive a Sharp monochrome memory display and a hand-made button-and-diode matrix that I mounted under the keyboard.

Calculator has four modes:

1. Calculator (performs as you might expect)
2. Piano (plays notes on the peizo buzzer)
3. Logo (shows a custom "Hacked by Peter" logo)
4. Game (a rudimentary version of Space Invaders)

If you don't have the Arduino GUI just change the *.ino files to *.cpp files and open them in any C++ IDE.
