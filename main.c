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
}

void main(void) {
    init();

    uint32_t last_millis = millis();
    int counter = 0;
    while(1) {
        CLRWDT();

        int enc1 = get_encoder_1();
        int enc2 = get_encoder_2();
        
        uint32_t now = millis();
        if (now - last_millis > 10) {
            throttle_motor_1(compute_pid_1(enc1));
            throttle_motor_2(compute_pid_2(enc2));

            if(counter % 100 == 0) {
                can_send_status();
            }

            last_millis = now;
            counter = (counter + 1) % 100;
        }

        txb_heartbeat();
    }
    return;
}
