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
    //put breakpoint here, examine the contents of these variables yourself
    uint16_t x = msg->sid;
    msg->data_len;
    msg->data[0];
    msg->data[1];
    msg->data[2];
    msg->data[3];
}

// Memory pool for CAN transmit buffer
uint8_t tx_pool[500];


void main(void) {
    // initialize the external oscillator
    oscillator_init();

    // init our millis() function
    timer0_init();
    
    /* ============= init CAN ============= */
    // init gpio pins
    // tx
    TRISC0 = 0; // out
    LATC0 = 1;
    ODCC0 = 1;
    ANSELC0 = 0;

    // rx
    TRISC1 = 1; // in
    ANSELC1 = 0;

    can_timing_t timing;
    timing.brp = 0;
    timing.btlmode = 0;
    timing.sjw = 0b11;
    timing.sam = 0;
    timing.seg1ph = 0b100;
    timing.seg2ph = 0b100;
    timing.prseg = 0;
    
    can_init(&timing, can_receive_callback);
    txb_init(tx_pool, sizeof(tx_pool), can_send, can_send_rdy);
    INTCON0 = 0b10000000;
    /* ============= init CAN ============= */
    
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
        
        can_msg_t board_stat_msg;
        build_board_stat_msg(millis(), E_NOMINAL, NULL, 0, &board_stat_msg);       
        // send it off at low priority
        txb_enqueue(&board_stat_msg);
        
        txb_heartbeat();
    }
    return;
}

static void __interrupt() interrupt_handler(void) {
    // Can interrupt
    if(PIR5) {
        can_handle_interrupt();
        return;
    }
    
    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
}