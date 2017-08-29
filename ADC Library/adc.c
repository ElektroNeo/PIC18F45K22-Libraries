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
    if (CH == 0) {
        TRISAbits.RA0 = 1;
        ANSELAbits.ANSA0 = 1;
    }
    else if (CH == 1) {
        TRISAbits.RA1 = 1;
        ANSELAbits.ANSA1 = 1;
    }
    else if (CH == 2) {
        TRISAbits.RA2 = 1;
        ANSELAbits.ANSA2 = 1;
    }
    else if (CH == 3) {
        TRISAbits.RA3 = 1;
        ANSELAbits.ANSA3 = 1;
    }
    else if (CH == 4) {
        TRISAbits.RA5 = 1;
        ANSELAbits.ANSA5 = 1;
    }
    else if (CH == 5) {
        TRISEbits.RE0 = 1;
        ANSELEbits.ANSE0 = 1;
    }
    else if (CH == 6) {
        TRISEbits.RE1 = 1;
        ANSELEbits.ANSE1 = 1;
    }
    else if (CH == 7) {
        TRISEbits.RE2 = 1;
        ANSELEbits.ANSE2 = 1;
    }
    else if (CH == 8) {
        TRISBbits.RB2 = 1;
        ANSELBbits.ANSB2 = 1;
    }
    else if (CH == 9) {
        TRISBbits.RB3 = 1;
        ANSELBbits.ANSB3 = 1;
    }
    else if (CH == 10) {
        TRISBbits.RB1 = 1;
        ANSELBbits.ANSB1 = 1;
    }
    else if (CH == 11) {
        TRISBbits.RB4 = 1;
        ANSELBbits.ANSB4 = 1;
    }
    else if (CH == 12) {
        TRISBbits.RB0 = 1;
        ANSELBbits.ANSB0 = 1;
    }
    else if (CH == 13) {
        TRISBbits.RB5 = 1;
        ANSELBbits.ANSB5 = 1;
    }
    else if (CH == 14) {
        TRISCbits.RC2 = 1;
        ANSELCbits.ANSC2 = 1;
    }
    else if (CH == 15) {
        TRISCbits.RC3 = 1;
        ANSELCbits.ANSC3 = 1;
    }
    else if (CH == 16) {
        TRISCbits.RC4 = 1;
        ANSELCbits.ANSC4 = 1;
    }
    else if (CH == 17) {
        TRISCbits.RC5 = 1;
        ANSELCbits.ANSC5 = 1;
    }
    else if (CH == 18) {
        TRISCbits.RC6 = 1;
        ANSELCbits.ANSC6 = 1;
    }
    else if (CH == 19) {
        TRISCbits.RC7 = 1;
        ANSELCbits.ANSC7 = 1;
    }
    else if (CH == 20) {
        TRISDbits.RD0 = 1;
        ANSELDbits.ANSD0 = 1;
    }
    else if (CH == 21) {
        TRISDbits.RD1 = 1;
        ANSELDbits.ANSD1 = 1;
    }
    else if (CH == 22) {
        TRISDbits.RD2 = 1;
        ANSELDbits.ANSD2 = 1;
    }
    else if (CH == 23) {
        TRISDbits.RD3 = 1;
        ANSELDbits.ANSD3 = 1;
    }
    else if (CH == 24) {
        TRISDbits.RD4 = 1;
        ANSELDbits.ANSD4 = 1;
    }
    else if (CH == 25) {
        TRISDbits.RD5 = 1;
        ANSELDbits.ANSD5 = 1;
    }
    else if (CH == 26) {
        TRISDbits.RD6 = 1;
        ANSELDbits.ANSD6 = 1;
    }
    else {
        TRISDbits.RD7 = 1;
        ANSELDbits.ANSD7 = 1;
    }
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