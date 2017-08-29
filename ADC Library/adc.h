
/* 
 * File:   adc.h
 * Author: ElektroNEO
 * Comments: 
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
    CH0,  CH1,  CH2,  CH3,  CH4,  CH5,  CH6,  CH7,  CH8, 
    CH9,  CH10, CH11, CH12, CH13, CH14, CH15, CH16, CH17, 
    CH18, CH19, CH20, CH21, CH22, CH23, CH24, CH25, CH26, 
    CH27
} channel_t;

void ADCInitialize();
void ADCSelectChannel(channel_t ch);
void ADCStart();
int ADCIsConversionDone();
uint16_t ADCGetResult();
uint16_t ADCGetConversion(channel_t ch);
void ADCSetIO(channel_t CH);

#endif	/* XC_HEADER_TEMPLATE_H */

