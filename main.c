/*
 * File:   main.c
 * Author: Kavin Satheeskumar
 *
 * Created on October 4, 2024, 9:51 PM
 */


#include <xc.h>
#include <canlib.h>

#include "device_config.h"
#include "can_logic.h"
#include "encoder_logic.h"
#include "pwm_logic.h"

// Memory pool for CAN transmit buffer
uint8_t tx_pool[500];

inline void init() {
    // initialize the external oscillator
    oscillator_init();

    // init our millis() function
    timer0_init();

    // Enable global interrupts
    INTCON0bits.GIE = 1;
    
    // initialize encoder
    initialize_encoder();
    
    // initialize can
    initialize_can(tx_pool);
    
    // initialize PWM
    initialize_pwm();

    // Set RA1 to an output
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 1;
}

static void __interrupt() interrupt_handler(void) {
    // Can interrupt
    if(PIR5) {
        can_handle_interrupt();
    }

    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
    
    encoder_interrupt_handler();
}

void main(void) {
    init();
    
    uint32_t last_millis = millis();
    while(1) {
        CLRWDT();

        uint32_t now = millis();
        if (now - last_millis > 1000) {
            last_millis = now;

            can_msg_t board_stat_msg;
            build_board_stat_msg(millis(), E_NOMINAL, NULL, 0, &board_stat_msg);
            txb_enqueue(&board_stat_msg);
        }

        txb_heartbeat();
    }
    return;
}
