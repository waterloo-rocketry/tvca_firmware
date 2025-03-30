#include "sequence.h"
#include "pid_logic.h"
#include <stdint.h>

int16_t tvc_sequence[][3] = {
#include "circle.seq"
};

#define seq_length (sizeof(tvc_sequence) / sizeof(uint16_t) / 3)
uint16_t seq_counter = seq_length;
int16_t delay_counter;

void tvc_seq_set_state(int start) {
    if(!start) {
        seq_counter = seq_length;
    } else if(seq_counter == seq_length) {
        seq_counter = 0;
        delay_counter = 0;
    }
}

void tvc_seq_update(void) {
    if(seq_counter < seq_length) {
        int16_t pos1 = tvc_sequence[seq_counter][0];
        int16_t pos2 = tvc_sequence[seq_counter][1];
        int16_t delay = tvc_sequence[seq_counter][2];

        pid_set_target_count_1(pos1);
        pid_set_target_count_2(pos2);

        if(delay_counter >= delay) {
            seq_counter++;
            delay_counter = 0;
        } else {
            delay_counter++;
        }
    }
}
