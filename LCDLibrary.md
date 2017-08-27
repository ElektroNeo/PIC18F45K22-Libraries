# LCD Library for PIC18F45K22

With this library you can use 2x16 LCD easly on pic18f45k22 microcontroller. Also you can use same algorithm to make library for other microcontroller.


# Features

  - Write character to LCD
  - Write string to LCD
  - Set position of cursor

You can also:
  - Send instructions to LCD.

### Functions
There are 13 functions in this library, but we mainly use 5 of them. Other functions are used in these 5 functions. These functions are;
- void LCDInitialize ();
- void LCDInstruction (uint8_t byte);
- void LCDSetPos (uint8_t x, uint8_t y);
- void LCDPutChar (uint8_t byte, uint8_t x, uint8_t y);
- void LCDPutString (char string [], uint8_t x, uint8_t y);

Let's examine these functions.

#### LCD Initialize

This function used before all of others in main function and it must called once. With this function you will send some important codes to LCD. 

```c
//Configuration codes

#define _XTAL_FREQ 4000000

#include <xc.h>
#include "lcd.h"

void main() {
    LCDInitialize();
    
    while(1) {
        // User codes
    }
}
```

#### LCD Instruction



| Variable | Feature |
| ------ | ------ |
| ClearDisplay | Clears entire display and sets DDRAM address 0 in address counter. |
| ReturnHome | Sets DDRAM address 0 in address counter. Also returns display from being shifted to original position. DDRAM contents remain unchanged. |
| EntryModeSet| Sets cursor move direction and specifies display shift. These operations are performed during data write and read.|
| DisplayOn | Sets entire display (D) on, cursor off (C), and blinking of cursor position character (B) off. |
| DisplayOff | Sets entire display (D) off, cursor off (C), and blinking of cursor position character (B) off. |
| ShiftCursorToLeft | Shifts the cursor position to the left. (AC is decremented by one.) |
| ShiftCursorToRight | Shifts the cursor position to the right. (AC is incremented  by one.) |
| ShiftDisplayToLeft | Shifts the entire display to the left. The cursor follows the display shift. |
| ShiftDisplayToRight| Shifts the entire display to the right. The cursor follows the display shift. |
| FunctionSet8Bit | Sets interface data length 8-bit(DL), number of display lines 2 (N), and character font (F). |
| FunctionSet4Bit | Sets interface data length 4-bit(DL), number of display lines 2 (N), and character font (F). |
| FirstLine | Sets cursor position to first line. |
| SecondLine | Sets cursor position to second line. |

In the main function you should send to LCD some instruction codes to set LCD for showing something. These codes are;

```c
void main(void) {
    LCDInitialize();
    while (1)
    {
        LCDInstruction(ClearDisplay);
        LCDInstruction(DisplayOn);
        LCDInstruction(EntryModeSet);
        // ********************
        // Other codes for LCD.
        // ********************
        LCDInstruction(ReturnHome);
    }
}
```

#### LCD Set Position
Sometimes you want show characters, strings or numbers with special position. For that, this function sets the cursor position to your request. The function have two parameter; x and y. X indicates column number and y indicates row number. The x number must be between 0 and 15, and, y must be 0 (first line) or 1 (second line).
Example:
```c
void main(void) {
    LCDInitialize();
    while (1)
    {
        LCDInstruction(ClearDisplay);
        LCDInstruction(DisplayOn);
        LCDInstruction(EntryModeSet);
        
        LCDSetPos(x, y);
        LCDDataMode();
        LCDWriting();
        LCDEnable();
    
        SendByteToLCD('A');
    
        LCDDisable();
        
        LCDInstruction(ReturnHome);
    }
}
```
Actually we do not need this function to write something with special position. Because the LCDPutChar() and LCDPutString() functions have this function too.

#### LCD Put Character
This function provides to us show character with a special position on LCD.
Example:
```c
void main(void) {
    LCDInitialize();
    while (1)
    {
        LCDInstruction(ClearDisplay);
        LCDInstruction(DisplayOn);
        LCDInstruction(EntryModeSet);
        // This codes sends three characters to the LCD.
        LCDPutChar('N', 5, 0);
        LCDPutChar('e', 6, 0);
        LCDPutChar('o', 7, 0);
        
        LCDInstruction(ReturnHome);
    }
}
```
#### LCD Put String
This function provides to us show string with a special position on LCD.
Example:
```c
void main(void) {
    LCDInitialize();
    char str[15] = "Elektro NEO";
    while (1)
    {
        LCDInstruction(ClearDisplay);
        LCDInstruction(DisplayOn);
        LCDInstruction(EntryModeSet);
        // This codes sends string to LCD.
        // String starts 5th column and second line.
        LCDPutString(str, 5, 1);
        
        LCDInstruction(ReturnHome);
    }
}
```
#### LCD Put Numbers
For string and character the library have functions, but for numbers we do not. To show numbers on LCD you can use `sprintf()` function with `stdio.h` library.
Example:
```c
void main(void) {
    LCDInitialize();
    char str[15];
    double num = 3.14;
    sprintf (str, "%.2f", num);
    
    while (1)
    {
        LCDInstruction(ClearDisplay);
        LCDInstruction(DisplayOn);
        LCDInstruction(EntryModeSet);
        // This codes sends string to LCD.
        // String starts 5th column and second line.
        LCDPutString(str, 5, 1);
        
        LCDInstruction(ReturnHome);
    }
}
```
