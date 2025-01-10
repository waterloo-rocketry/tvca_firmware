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
    // Enable IO interrupts
    PIE0bits.IOCIE = 1;

    // Disable pin drivers
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB5 = 1;

    // Set pins to digital mode
    ANSELAbits.ANSELA3 = 0;
    ANSELAbits.ANSELA4 = 0;
    ANSELBbits.ANSELB3 = 0;
    ANSELBbits.ANSELB5 = 0;
    
    // Enable interrupt on both edges
    IOCAPbits.IOCAP3 = IOCANbits.IOCAN3 = 1;
    IOCAPbits.IOCAP4 = IOCANbits.IOCAN4 = 1;
    IOCBPbits.IOCBP3 = IOCBNbits.IOCBN3 = 1;
    IOCBPbits.IOCBP5 = IOCBNbits.IOCBN5 = 1;
}

void encoder_interrupt_handler() {
    if(IOCAFbits.IOCAF3) {
        if(PORTAbits.RA4 != PORTAbits.RA3) {
            count1++;
        } else {
            count1--;
        }
    }

    if(IOCAFbits.IOCAF4) {
        if(PORTAbits.RA4 != PORTAbits.RA3) {
            count1--;
        } else {
            count1++;
        }
    }

    if(IOCBFbits.IOCBF3) {
        if(PORTBbits.RB5 != PORTBbits.RB3) {
            count2++;
        } else {
            count2--;
        }
    }

    if(IOCBFbits.IOCBF5) {
        if(PORTBbits.RB5 != PORTBbits.RB3) {
            count2--;
        } else {
            count2++;
        }
    }

    IOCAF &= IOCAF ^ 0xFF;
    IOCBF &= IOCBF ^ 0xFF;
}

int get_encoder_1() {
    return count1;
}

int get_encoder_2() {
    return count2;
}
