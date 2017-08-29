/*
 * File:   adc.c
 * Author: ElektroNEO
 *
 * Created on 28 August 2017 Monday, 14:27
 */


#include <xc.h>
#include "adc.h"

void ADCInitialize() {
    
    // GO_nDONE stop; ADON enabled; CHS AN0 (default); 
    ADCON0 = 0x01;
    
    // TRIGSEL CCP5; NVCFG VSS; PVCFG VDD; 
    ADCON1 = 0x00;
    
    // ADFM right; ACQT 12; ADCS FOSC/16;
    ADCON2bits.ACQT = 0b101;
    ADCON2bits.ADCS = 0b101;
    ADCON2bits.ADFM = 1; // right justified
    // ADRESL 0; 
    ADRESL = 0x00;
    
    // ADRESH 0; 
    ADRESH = 0x00;
    
}

void ADCSetIO(channel_t CH) {
    if (CH <= 3) {
        TRISA |= (1 << CH);
    }
    else if (CH == 4) {
        TRISAbits.RA5 = 1;
    }
    else if (CH <= 7) {
        TRISE |= (1 << (CH-5));
    }
    else if (CH <= 9) {
        TRISB |= (1 << (CH-6));
    }
    else if (CH == 10) {
        TRISBbits.RB1 = 1;
    }
    else if (CH == 11) {
        TRISBbits.RB4 = 1;
    }
    else if (CH == 12) {
        TRISBbits.RB0 = 1;
    }
    else if (CH == 13) {
        TRISBbits.RB5 = 1;
    }
    else if (CH <= 19) {
        TRISC |= (1 << (CH-12));
    }
    else {
        TRISD |= (1 << (CH-20));
    }
    
    ANSELA = TRISA;
    ANSELB = TRISB;
    ANSELC = TRISC;
    ANSELD = TRISD;
    ANSELE = TRISE;
}

void ADCSelectChannel(channel_t ch) {
    ADCSetIO(ch);
    // Select the A/D channel
    ADCON0bits.CHS = ch;    
    // Turn on the ADC module
    ADCON0bits.ADON = 1; 
}

void ADCStart() {
    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
}

int ADCIsConversionDone() {
    // Start the conversion
    return ((int)(!ADCON0bits.GO_nDONE));
}

uint16_t ADCGetResult() {
    // Conversion finished, return the result
    return ((uint16_t)((ADRESH << 8) + ADRESL));
}

uint16_t ADCGetConversion(channel_t ch) {
    ADCSetIO(ch);
    // select the A/D channel
    ADCON0bits.CHS = ch;    

    // Turn on the ADC module
    ADCON0bits.ADON = 1;

    // Start the conversion
    ADCON0bits.GO_nDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE);

    // Conversion finished, return the result
    return ((uint16_t)((ADRESH << 8) + ADRESL));
}