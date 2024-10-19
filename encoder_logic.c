/*
 * File:   encoder_logic.c
 * Author: kavin
 *
 * Created on October 19, 2024, 1:21 PM
 */

#include "encoder_logic.h"
#include <xc.h>

// Encoder counts
int count1 = 0;
int count2 = 0;

void initialize_encoder() {
    // Enable reading encoder 
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB5 = 1;
    
    // Enable interrupts
    IOCIE = 1;
    
    // enable interrupt on rising edge
    // for RA3, RA4, RB3 and RB5
    IOCAPbits.IOCAP3 = 1;
    IOCAPbits.IOCAP4 = 1;
    IOCBPbits.IOCBP3 = 1;
    IOCBPbits.IOCBP5 = 1;
}

void encoder_interrupt_handler() {
    // Inspired by this
    // https://electronics.stackexchange.com/questions/496759/pic18f-rotary-encoder-code-help-xc8
    if (IOCAFbits.IOCAF3 && !IOCAFbits.IOCAF4) {
        ++count1;
        // Switch edge direction
        IOCAPbits.IOCAP3 ^= 1;
        IOCAPbits.IOCAP4 ^= 1;
        IOCANbits.IOCAN3 ^= 1;
        IOCANbits.IOCAN4 ^= 1;
        IOCAF = 0;
    }
    if (!IOCAFbits.IOCAF3 && IOCAFbits.IOCAF4) {
        --count1;
        IOCAPbits.IOCAP3 ^= 1;
        IOCAPbits.IOCAP4 ^= 1;
        IOCANbits.IOCAN3 ^= 1;
        IOCANbits.IOCAN4 ^= 1;
        IOCAF = 0;
    }
    
    if (IOCBFbits.IOCBF3 && !IOCBFbits.IOCBF5) {
        ++count2;
        IOCBPbits.IOCBP3 ^= 1;
        IOCBPbits.IOCBP5 ^= 1;
        IOCBNbits.IOCBN3 ^= 1;
        IOCBNbits.IOCBN5 ^= 1;
        IOCBF = 0;
    }
    if (!IOCBFbits.IOCBF3 && IOCBFbits.IOCBF5) {
        --count2;
        IOCBPbits.IOCBP3 ^= 1;
        IOCBPbits.IOCBP5 ^= 1;
        IOCBNbits.IOCBN3 ^= 1;
        IOCBNbits.IOCBN5 ^= 1;
        IOCBF = 0;
    }
}

int get_encoder_1() {
    return count1;
}

int get_encoder_2() {
    return count2;
}