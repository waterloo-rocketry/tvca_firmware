/*
 * File:   main.c
 * Author: Kavin Satheeskumar
 *
 * Created on October 4, 2024, 9:51 PM
 */


#include <xc.h>
#include <canlib.h>
#include <stdio.h>

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
    initialize_can(tx_pool, sizeof(tx_pool));

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

    if(PIE0bits.IOCIE == 1 && PIR0bits.IOCIF == 1) {
        encoder_interrupt_handler();
    }
}

void main(void) {
    init();

    pwm_throttle_1(0.5);
    pwm_throttle_2(0.25);

    uint32_t last_millis = millis();
    while(1) {
        CLRWDT();

        uint32_t now = millis();
        if (now - last_millis > 1000) {
            last_millis = now;

            int encoder_pos[2] = {get_encoder_1(), get_encoder_2()};

            can_msg_t msg;
            build_board_stat_msg(millis(), E_NOMINAL, NULL, 0, &msg);
            txb_enqueue(&msg);

            char text[64];
            snprintf(text, sizeof(msg), "%d %d", encoder_pos[0], encoder_pos[1]);
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);
        }

        txb_heartbeat();
    }
    return;
}
