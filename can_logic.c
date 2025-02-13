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
#include "canlib/can_common.h"
#include "pid_logic.h"
#include "adc_logic.h"

void can_receive_callback(const can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);
    int dest_id = -1;
    int id;
    int value;

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
        case MSG_ACTUATOR_CMD:
            id = get_actuator_id(msg);
            value = get_req_actuator_state(msg);
            switch(id) {
                case 1:
                    set_desired_angle_1(value);
                case 2:
                    set_desired_angle_2(value);
            }
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

int counter = 0;
void can_send_status(void) {
    uint16_t cur_amp = adc_read_channel(channel_CUR_AMP);
    uint16_t cur_1 = adc_read_channel(channel_CUR_1);
    uint16_t cur_2 = adc_read_channel(channel_CUR_2);
    uint16_t vbat_1 = adc_read_channel(channel_VBAT_1);
    uint16_t vbat_2 = adc_read_channel(channel_VBAT_2);
    int enc1 = get_encoder_1();
    int enc2 = get_encoder_2();

    can_msg_t msg;

    // using counter to limit number of message per loop
    // workaround for usb debug dropping message when sent in burst

    if(counter % 3 == 0) {
        build_analog_data_msg(millis(), SENSOR_5V_CURR, cur_amp, &msg);
        txb_enqueue(&msg);

        build_analog_data_msg(millis(), SENSOR_9V_BATT_CURR1, cur_1, &msg);
        txb_enqueue(&msg);

        build_analog_data_msg(millis(), SENSOR_9V_BATT_CURR2, cur_2, &msg);
        txb_enqueue(&msg);
    } else if(counter % 3 == 1) {
        build_analog_data_msg(millis(), SENSOR_ARM_BATT_1, vbat_1, &msg);
        txb_enqueue(&msg);

        build_analog_data_msg(millis(), SENSOR_ARM_BATT_2, vbat_2, &msg);
        txb_enqueue(&msg);
    } else if(counter % 3 == 0) {
        build_analog_data_msg(millis(), SENSOR_MAG_1, enc1, &msg);
        txb_enqueue(&msg);

        build_analog_data_msg(millis(), SENSOR_MAG_2, enc2, &msg);
        txb_enqueue(&msg);
    }

    counter++;
}
