/*
 * File:   main.c
 * Author: Kavin Satheeskumar
 *
 * Created on October 4, 2024, 9:51 PM
 */


#include <xc.h>
#include <can_common.h>
#include <pic18f26k83/pic18f26k83_can.h>
#include <pic18f26k83/pic18f26k83_timer.h>

void main(void) {
    timer0_init();
    
    // Set RA1 to an output
    TRISAbits.TRISA1 = 0;
    while(1) {
        uint32_t cur_millis = millis();
        
        if (millis() > cur_millis + 1000) {
            LATAbits.LATA1 = !LATAbits.LATA1;
            cur_millis = millis();
        }
    }
    return;
}
