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
#include "motor_logic.h"
#include "adc_logic.h"
#include "pid_logic.h"

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

    // initialize ADC
    ADCC_Initialize();

    // initialize can
    initialize_can(tx_pool, sizeof(tx_pool));

    // initialize PWM
    initialize_pwm();
    
    // initialize motors
    initialize_motors();

    // Set RA1 to an output
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 0;
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

    uint32_t last_millis = millis();
    while(1) {
        CLRWDT();
        
        throttle_motor_1(compute_pid_1(get_encoder_1()));
        throttle_motor_2(compute_pid_2(get_encoder_2()));

        uint32_t now = millis();
        if (now - last_millis > 1000) {
            last_millis = now;
            
            uint16_t cur_amp = ADCC_GetSingleConversion(channel_CUR_AMP);
            uint16_t cur_1 = ADCC_GetSingleConversion(channel_CUR_1);
            uint16_t cur_2 = ADCC_GetSingleConversion(channel_CUR_2);
            uint16_t vbat_1 = ADCC_GetSingleConversion(channel_VBAT_1);
            uint16_t vbat_2 = ADCC_GetSingleConversion(channel_VBAT_2);

            can_msg_t msg;
            build_board_stat_msg(millis(), E_NOMINAL, NULL, 0, &msg);
            txb_enqueue(&msg);

            char text[8];
            snprintf(text, sizeof(text), "%d", cur_amp);
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);

            snprintf(text, sizeof(text), "%d", cur_1);
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);

            snprintf(text, sizeof(text), "%d", cur_2);
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);

            snprintf(text, sizeof(text), "%d", vbat_1);
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);

            snprintf(text, sizeof(text), "%d", vbat_2);
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);

            snprintf(text, sizeof(text), "---");
            build_printf_can_message(text, &msg);
            txb_enqueue(&msg);
        }

        txb_heartbeat();
    }
    return;
}
