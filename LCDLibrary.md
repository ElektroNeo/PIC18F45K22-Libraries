
# LCD Library for PIC18F45K22

With this library you can use 2x16 LCD easly on pic18f45k22 microcontroller. Also you can use same algorithm to make library for other microcontroller.


## Features

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

#### LCD Instraction



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
