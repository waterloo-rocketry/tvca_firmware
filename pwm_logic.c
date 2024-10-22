/* 
 * File:   pwm_logic.c
 * Author: kavin
 *
 * Created on October 21, 2024, 9:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "pwm_logic.h"
#include <xc.h>

void initialize_pwm() {
    // Use PPS to set RC4 as the output of 
    // PWM5 and RC5 as the output of PWM6
    // refer to pages 
    // 262, 266 and 267 of the data sheet
    // to learn how PPS works
    RC4PPS = 0b001101;
    RC5PPS = 0b001110;
    
    // Everything here is based off of page
    // 343 of the PIC18F26K83 data sheet
    // (AKA the bible)
    
    // Set RC4 and RC5 to outputs
    // and initialize them to zero
    TRISCbits.TRISC4 = 0;
    LATCbits.LATC4 = 0;
    TRISCbits.TRISC5 = 0;
    LATCbits.LATC5 = 0;
    
    // clear control registers
    PWM5CON = 0;
    PWM6CON = 0;
    
    // Period value,
    // Period = [255 + 1] * 4 / 12,000,000
    // = 85 microseconds
    T2PR = 0xFF;
    
    // Set the duty cycle. 
    // This indicates the fraction
    // of the cycle where the signal is
    // high. Set it to zero for now.
    PWM5DCH = 0;
    PWM5DCL = 0;
    PWM6DCH = 0;
    PWM6DCL = 0;
    
    // We don't care about emitting a perfect
    // PWM cycle the first time around so we skip
    // a few steps in the data sheet
    
    // enable PWM
    PWM5CONbits.PWM5EN = 1;
    PWM6CONbits.PWM6EN = 1;
}

void pwm_throttle_1(float value) {
    if (value < 0) {
        value = 0;
    } else if (value > 1) {
        value = 1;
    }
    
    // For non-negative value, the cast 
    // acts like a floor
    uint16_t bits = (uint16_t)(value * 1023);
    PWM5DCLbits.PWM5DC0 = bits & 1;
    PWM5DCLbits.PWM5DC1 = (bits >> 1) & 1;
    PWM5DCH = (uint8_t) (bits >> 2);
}

void pwm_throttle_2(float value) {
    if (value < 0) {
        value = 0;
    } else if (value > 1) {
        value = 1;
    }
    
    // For non-negative value, the cast 
    // acts like a floor
    uint16_t bits = (uint16_t)(value * 1023);
    PWM6DCLbits.PWM6DC0 = bits & 1;
    PWM6DCLbits.PWM6DC1 = (bits >> 1) & 1;
    PWM6DCH = (uint8_t) (bits >> 2);
}