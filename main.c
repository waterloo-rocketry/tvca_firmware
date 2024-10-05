/*
 * File:   main.c
 * Author: Kavin Satheeskumar
 *
 * Created on October 4, 2024, 9:51 PM
 */


#include <xc.h>
#include <canlib.h>

#include "device_config.h"

void main(void) {
    // initialize the external oscillator
    oscillator_init();

    // init our millis() function
    timer0_init();
    
    // Set RA1 to an output
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 1;

    uint32_t last_millis = millis();
    while(1) {
        CLRWDT();

        uint32_t now = millis();
        if (now - last_millis > 1000) {
            LATAbits.LATA1 ^= 1;
            last_millis = now;
        }
    }
    return;
}

static void __interrupt() interrupt_handler(void) {
    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
}
