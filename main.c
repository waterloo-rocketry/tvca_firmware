/*
 * File:   main.c
 * Author: Kavin Satheeskumar
 *
 * Created on October 4, 2024, 9:51 PM
 */


#include <xc.h>
#include <canlib.h>

#include "device_config.h"

void can_receive_callback(const can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);
    int dest_id = -1;

    switch (msg_type) {
        case MSG_LEDS_ON:
            LATAbits.LATA1 = 0;
            break;
        case MSG_LEDS_OFF:
            LATAbits.LATA1 = 1;
            break;
        case MSG_RESET_CMD:
            RESET();
            break;
        default:
            // all the other ones - do nothing
            break;
    }
}

// Memory pool for CAN transmit buffer
uint8_t tx_pool[500];

void main(void) {
    // initialize the external oscillator
    oscillator_init();

    // init our millis() function
    timer0_init();

    // Enable global interrupts
    INTCON0bits.GIE = 1;

    /* ============= init CAN ============= */
    // init gpio pins
    // tx
    TRISC1 = 0; // out
    ANSELC1 = 0;
    RC1PPS = 0x33; // make C1 transmit CAN TX (page 267)

    // rx
    TRISC0 = 1; // in
    ANSELC0 = 0;
    CANRXPPS = 0x10; // make CAN read from C0 (page 264-265)

    // init can module
    can_timing_t can_setup;
    can_generate_timing_params(_XTAL_FREQ * 4, &can_setup);
    can_init(&can_setup, can_receive_callback);
    txb_init(tx_pool, sizeof(tx_pool), can_send, can_send_rdy);
    /* ============= init CAN ============= */

    // Set RA1 to an output
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 1;

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
}
