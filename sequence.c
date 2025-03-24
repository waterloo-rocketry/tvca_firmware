#include "sequence.h"
#include "pid_logic.h"
#include <stdint.h>

int16_t tvc_sequence[][2] = {
#include "circle.seq"
};

#define divider 2
#define seq_length (sizeof(tvc_sequence) / sizeof(uint16_t) / 2)
uint16_t seq_counter = seq_length * divider;

void tvc_seq_set_state(int start) {
    if(start)
        seq_counter = 0;
    else
        seq_counter = seq_length * divider;
}

void tvc_seq_update(void) {
    if(seq_counter < seq_length * divider) {
        int16_t pos1 = tvc_sequence[seq_counter/divider][0];
        int16_t pos2 = tvc_sequence[seq_counter/divider][1];

        pid_set_target_count_1(pos1);
        pid_set_target_count_2(pos2);

        seq_counter = (seq_counter + 1) % (seq_length * divider);
    }
}
