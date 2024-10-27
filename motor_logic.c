/* 
 * File:   motor_logic.c
 * Author: kavin
 *
 * Created on October 26, 2024, 12:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "pwm_logic.h"
#include "motor_logic.h"


void initialize_motor_1() {
    // Set RB1 to and RB2
    // to outputs
    // these control direction
    // CWL and CCW respectively
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    
    // Set RC6 to an output
    // SEL_1
    TRISCbits.TRISC6 = 0;
    LATCbits.LATC6 = 1;
    
    // Set RB0 (CUR1) and 
    // RB4 (VBAT1) to analog
    // inputs
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB4 = 1;
    ANSELBbits.ANSELB0 = 1;
    ANSELBbits.ANSELB4 = 1;
}

void initialize_motor_2() {
    // Set RA2 to and RA5
    // to outputs
    // these control direction
    // CWL and CCW respectively
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA5 = 0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA5 = 0;
    
    // Set RC6 to an output
    // SEL_2
    TRISCbits.TRISC3 = 0;
    LATCbits.LATC3 = 1;
    
    // Set RC7 (CUR2) and 
    // RC2 (VBAT1) to analog
    // inputs
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC2 = 1;
    ANSELCbits.ANSELC7 = 1;
    ANSELCbits.ANSELC2 = 1;
}

inline void initialize_motors() {
    initialize_motor_1();
    initialize_motor_2();
}

void throttle_motor_1(float value) {
    if (value > 0) {
        // clockwise
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 0;
    } else if (value < 0) {
        // counter clockwise
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 1;
    } else {
        // break
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
    }
    value = (value >= 0) ? value : -value; // absolute value
    pwm_throttle_1(value);
}

void throttle_motor_2(float value) {
    if (value > 0) {
        // clockwise
        LATAbits.LATA2 = 1;
        LATAbits.LATA5 = 0;
    } else if (value < 0) {
        // counter clockwise
        LATAbits.LATA2 = 0;
        LATAbits.LATA5 = 1;
    } else {
        // break
        LATAbits.LATA2 = 1;
        LATAbits.LATA5 = 1;
    }
    value = (value >= 0) ? value : -value; // absolute value
    pwm_throttle_2(value);
}