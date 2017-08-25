/*
 * File:   lcd.c
 * Author: ElektroNEO
 *
 * Created on 24 August 2017 Thursday, 08:49
 */

#include "lcd.h"

void LCDInitialize() {
    __delay_ms(300);
    LCDInstruction(FunctionSet8Bit);
    __delay_ms(100);
    LCDInstruction(FunctionSet8Bit);
    __delay_us(150);
    LCDInstruction(FunctionSet8Bit);
    LCDInstruction(DisplayOff);
    LCDInstruction(ClearDisplay);
    LCDInstruction(EntryModeSet);
    
}

void LCDDelay(uint16_t num) {
    for(num; num > 0; num--);
}

void LCDInstructionMode() {
    LCDRS = 0;
}

void LCDDataMode() {
    LCDRS = 1;
}

void LCDWriting() {
    LCDDelay(DelayTime);
    LCDRW = 0;
    LCDDelay(DelayTime*3);
}

void LCDReading() {
    LCDDelay(DelayTime);
    LCDRW = 1;
    LCDDelay(DelayTime*3);
}

void LCDEnable() {
    LCDDelay(DelayTime*3);
    LCDE = 1;
    LCDDelay(DelayTime*6);
}

void LCDDisable() {
    LCDDelay(DelayTime*6);
    LCDE = 0;
    LCDDelay(DelayTime);
}

void SendByteToLCD(char byte) {
    LCDD0 = (unsigned)((byte & 0b00000001) >> 0);
    LCDD1 = (unsigned)((byte & 0b00000010) >> 1);
    LCDD2 = (unsigned)((byte & 0b00000100) >> 2);
    LCDD3 = (unsigned)((byte & 0b00001000) >> 3);
    LCDD4 = (unsigned)((byte & 0b00010000) >> 4);
    LCDD5 = (unsigned)((byte & 0b00100000) >> 5);
    LCDD6 = (unsigned)((byte & 0b01000000) >> 6);
    LCDD7 = (unsigned)((byte & 0b10000000) >> 7);
}

void LCDInstruction(char byte) {
    LCDInstructionMode();
    LCDWriting();
    LCDEnable();
    
    SendByteToLCD(byte);
    
    LCDDisable();
}

/*
************* Display Position ***************
1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
-----------------------------------------------
00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F

*/

void LCDSetPos(uint8_t x, uint8_t y) {
    uint8_t Pos = (unsigned)(0x80 + y*64 + x);
    LCDInstruction(Pos);
}

void LCDPutChar(uint8_t byte, uint8_t x, uint8_t y) {
    LCDSetPos(x, y);
    LCDDataMode();
    LCDWriting();
    LCDEnable();
    
    SendByteToLCD(byte);
    
    LCDDisable();
}

void LCDPutString(char string[], uint8_t x, uint8_t y) {
    LCDSetPos(x, y);
    for(uint8_t i = 0; i < 17; i++) {
        
        if(string[i] == '\0')
            break;
        
        LCDDataMode();
        LCDWriting();
        LCDEnable();

        SendByteToLCD(string[i]);

        LCDDisable();
    }
}