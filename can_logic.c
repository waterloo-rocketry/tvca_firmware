/* 
 * File:   can_logic.c
 * Author: kavin
 *
 * Created on October 17, 2024, 10:13 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <canlib.h>
#include <xc.h>
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

void initialize_can(uint8_t *tx_pool, size_t tx_pool_size) {
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
    txb_init(tx_pool, tx_pool_size, can_send, can_send_rdy);
    /* ============= init CAN ============= */
}
