# PIC18F45K22-Libraries
These libraries are useful to develop aplications on pic18f45k22 microcontroller.

# LCD Library for PIC18F45K22

With this library you can use 2x16 LCD easly on pic18f45k22 microcontroller. Also you can use same algorithm to make library for other microcontroller.


# Features

  - Write character to LCD
  - Write string to LCD
  - Set position of cursor

You can also:
  - Send instructions to LCD.

## Functions
There are 13 functions in this library, but we mainly use 5 of them. Other functions are used in these 5 functions. These functions are;
- void LCDInitialize ();
- void LCDInstruction (uint8_t byte);
- void LCDSetPos (uint8_t x, uint8_t y);
- void LCDPutChar (uint8_t byte, uint8_t x, uint8_t y);
- void LCDPutString (char string [], uint8_t x, uint8_t y);

For more information go [Github Wiki](https://github.com/ElektroNeo/PIC18F45K22-Libraries/wiki/LCD-Library).
