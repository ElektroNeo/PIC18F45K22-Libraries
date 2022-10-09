/*
 * File:   main.c
 * Author: Bahtiyar Bayram
 *
 * Created on 04 February 2022, 23:35
 */


#include "main.h"
#include <stdint.h>

#define SEV_SEG_DIGIT_COUNT         4
#define SEV_SEG_SEGMENT_COUNT       8

const uint8_t char_map[] = {
    //  [DP]ABCDEFG  Segments      7-segment map:
    0b01111110, // 0   "0"          AAA
    0b00110000, // 1   "1"         F   B
    0b01101101, // 2   "2"         F   B
    0b01111001, // 3   "3"          GGG
    0b00110011, // 4   "4"         E   C
    0b01011011, // 5   "5"         E   C
    0b01011111, // 6   "6"          DDD
    0b01110000, // 7   "7"
    0b01111111, // 8   "8"
    0b01111011, // 9   "9"
    0b01110111, // 10  "A"
    0b00011111, // 11  "b"
    0b01001110, // 12  "C"
    0b00111101, // 13  "d"
    0b01001111, // 14  "E"
    0b01000111, // 15  "F"
};

typedef struct {
    volatile unsigned char *dir;
    volatile unsigned char *port;
    uint8_t pin;
} pin_t;

typedef struct {
    uint16_t display_num;
    uint16_t display_dig_val;
    uint16_t digit_delay_100us;
    uint16_t current_delay_100us;
    uint8_t current_digit;
    uint8_t current_seg_val;
    uint8_t special_char[SEV_SEG_DIGIT_COUNT];
    pin_t seg[SEV_SEG_SEGMENT_COUNT];
    pin_t digit[SEV_SEG_DIGIT_COUNT];
    uint8_t state;
} sev_seg_display_t;

sev_seg_display_t seven_segment;

void sev_seg_init(sev_seg_display_t * sev_seg) {
    /* Set digit pins as output (to control digit transistor) */
    for (uint8_t i = 0; i < SEV_SEG_DIGIT_COUNT; i++) {
        (*sev_seg->digit[i].dir) &= ~(1 << sev_seg->digit[i].pin);
        (*sev_seg->digit[i].port) |= (1 << sev_seg->digit[i].pin);
    }

    /* Set segment pins as output */
    for (uint8_t i = 0; i < SEV_SEG_SEGMENT_COUNT; i++) {
        (*sev_seg->seg[i].dir) &= ~(1 << sev_seg->seg[i].pin);
        (*sev_seg->seg[i].port) &= ~(1 << sev_seg->seg[i].pin);
    }
}

void sev_seg_scan(sev_seg_display_t * sev_seg) {
    uint8_t digit_val = 0, is_special_char_enabled = 0;

    /* Increase delay_us */
    sev_seg->current_delay_100us++;
    if (sev_seg->current_delay_100us < sev_seg->digit_delay_100us) {
        return;
    }

    
    if (sev_seg->state == 0) {
        sev_seg->state = 1;
        /* Turn off segments */
        for (uint8_t i = 0; i < SEV_SEG_SEGMENT_COUNT; i++) {
            (*sev_seg->seg[i].port) |= (1 << sev_seg->seg[i].pin);
        }

        sev_seg->current_delay_100us = 0;
        return;
    }

    sev_seg->state = 0;

    sev_seg->current_delay_100us = 0;

    /* Turn off digits */
    for (uint8_t i = 0; i < SEV_SEG_DIGIT_COUNT; i++) {
        (*sev_seg->digit[i].port) |= (1 << sev_seg->digit[i].pin);
    }

    /* Turn on current digit */
    (*sev_seg->digit[SEV_SEG_DIGIT_COUNT - sev_seg->current_digit - 1].port) &= ~(1 << sev_seg->digit[SEV_SEG_DIGIT_COUNT - sev_seg->current_digit - 1].pin);

    /* Turn off segments */
    for (uint8_t i = 0; i < SEV_SEG_SEGMENT_COUNT; i++) {
        (*sev_seg->seg[i].port) |= (1 << sev_seg->seg[i].pin);
    }
    
    /* Enable is_special_char_enabled if special_char array is not equal to 0 */
    for (uint8_t i = 0; i < SEV_SEG_DIGIT_COUNT; i++) {
        if (sev_seg->special_char[i] != 0) {
            is_special_char_enabled = 1;
        }
    }

    /* If there is any special character, then, display it */
    if (is_special_char_enabled) {
        digit_val = sev_seg->special_char[sev_seg->current_digit];
    } 
    /* Else display display_num variable */
    else {
        /* Calculate current digit value */
        sev_seg->display_dig_val = sev_seg->display_num;
        for (uint8_t i = 0; i < sev_seg->current_digit; i++) {
            sev_seg->display_dig_val /= 10;
        }
        sev_seg->display_dig_val %= 10;

        /* Get value from char map */
        digit_val = char_map[sev_seg->display_dig_val];
    }

    /* Turn on corresponding segments */
    for (uint8_t i = 0, j = 0b01000000; i < SEV_SEG_SEGMENT_COUNT - 1; i++, j >>= 1) {
        if (digit_val & j) {
            (*sev_seg->seg[i].port) &= ~(1 << sev_seg->seg[i].pin);
        }
    }

    /* Increase current digit */
    if (sev_seg->current_digit++ > SEV_SEG_DIGIT_COUNT) {
        sev_seg->current_digit = 0;
    }
}

void __interrupt() ISR(void) {
    // Timer1 Interrupt - Freq = 10000.00 Hz - Period = 0.000100 seconds
    if (PIR1bits.TMR1IF == 1) {
        /* 100us timer */
        sev_seg_scan(&seven_segment);

        PIR1bits.TMR1IF = 0; // interrupt must be cleared by software
        PIE1bits.TMR1IE = 1; // reenable the interrupt
        TMR1H = 254; // preset for timer1 MSB register
        TMR1L = 112; // preset for timer1 LSB register
    }
}

void main(void) {
    /* Configure oscillator */
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;

    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;

    WPUB = 0x00;

    /* Initialise Timer0 */
    //Timer1 Registers Prescaler= 1 - TMR1 Preset = 65136 - Freq = 10000.00 Hz - Period = 0.000100 seconds
    T1CONbits.T1CKPS1 = 0; // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 0; // bit 4
    T1CONbits.T1OSCEN = 1; // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1; // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0; // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1; // bit 0 enables timer
    TMR1H = 254; // preset for timer1 MSB register
    TMR1L = 112; // preset for timer1 LSB register


    // Interrupt Registers
    INTCON = 0; // clear the interrpt control register
    INTCONbits.TMR0IE = 0; // bit5 TMR0 Overflow Interrupt Enable bit...0 = Disables the TMR0 interrupt
    PIR1bits.TMR1IF = 0; // clear timer1 interupt flag TMR1IF
    PIE1bits.TMR1IE = 1; // enable Timer1 interrupts
    INTCONbits.TMR0IF = 0; // bit2 clear timer 0 interrupt flag
    INTCONbits.GIE = 1; // bit7 global interrupt enable
    INTCONbits.PEIE = 1; // bit6 Peripheral Interrupt Enable bit...1 = Enables all unmasked peripheral interrupts


    /*seven_segment.digit[0].port = &PORTB;
    seven_segment.digit[0].dir = &TRISB;
    seven_segment.digit[0].pin = 0;
    
    seven_segment.digit[1].port = &PORTB;
    seven_segment.digit[1].dir = &TRISB;
    seven_segment.digit[1].pin = 1;
    
    seven_segment.digit[2].port = &PORTB;
    seven_segment.digit[2].dir = &TRISB;
    seven_segment.digit[2].pin = 2;
    
    seven_segment.digit[3].port = &PORTB;
    seven_segment.digit[3].dir = &TRISB;
    seven_segment.digit[3].pin = 3;
    
    seven_segment.seg[0].port = &PORTD;
    seven_segment.seg[0].dir = &TRISD;
    seven_segment.seg[0].pin = 0;
    
    seven_segment.seg[1].port = &PORTD;
    seven_segment.seg[1].dir = &TRISD;
    seven_segment.seg[1].pin = 1;
    
    seven_segment.seg[2].port = &PORTD;
    seven_segment.seg[2].dir = &TRISD;
    seven_segment.seg[2].pin = 2;
    
    seven_segment.seg[3].port = &PORTD;
    seven_segment.seg[3].dir = &TRISD;
    seven_segment.seg[3].pin = 3;
    
    seven_segment.seg[4].port = &PORTD;
    seven_segment.seg[4].dir = &TRISD;
    seven_segment.seg[4].pin = 4;
    
    seven_segment.seg[5].port = &PORTD;
    seven_segment.seg[5].dir = &TRISD;
    seven_segment.seg[5].pin = 5;
    
    seven_segment.seg[6].port = &PORTD;
    seven_segment.seg[6].dir = &TRISD;
    seven_segment.seg[6].pin = 6;
    
    seven_segment.seg[7].port = &PORTD;
    seven_segment.seg[7].dir = &TRISD;
    seven_segment.seg[7].pin = 7;*/

    for (uint8_t i = 0; i < SEV_SEG_DIGIT_COUNT; i++) {
        seven_segment.digit[i].port = &PORTB;
        seven_segment.digit[i].dir = &TRISB;
        seven_segment.digit[i].pin = i;
    }

    for (uint8_t i = 0; i < SEV_SEG_SEGMENT_COUNT; i++) {
        seven_segment.seg[i].port = &PORTD;
        seven_segment.seg[i].dir = &TRISD;
        seven_segment.seg[i].pin = i;
    }

    seven_segment.digit_delay_100us = 10;

    sev_seg_init(&seven_segment);

    TRISBbits.TRISB4 = 1;

    while (1) {
        if (PORTBbits.RB4 == 0) {
            //            for (uint8_t i = 0; i < SEV_SEG_DIGIT_COUNT; i++) {
            //                seven_segment.special_char[i] >>= 1;
            //                if (seven_segment.special_char[i] == 1) {
            //                    seven_segment.special_char[i] = 0b01000000;
            //                } else if (seven_segment.special_char[i] == 0) {
            //                    seven_segment.special_char[i] = 0b01000000 >> i;
            //                }
            //            }

            seven_segment.special_char[3] = 0b1110111;
            seven_segment.special_char[2] = 0b0011111;
            seven_segment.special_char[1] = 0b1001110;
            seven_segment.special_char[0] = 0b0111101;
        } else {
            for (uint8_t i = 0; i < SEV_SEG_DIGIT_COUNT; i++) {
                seven_segment.special_char[i] = 0;
            }
            seven_segment.display_num++;
        }

        __delay_ms(50);
    }
    return;
}
