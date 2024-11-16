/*
 * File:   pid_logic.c
 * Author: kavin
 *
 * Created on November 16, 2024, 11:52 AM
 */


#include <xc.h>
#include "pid_logic.h"

int desired_angle_1 = 0;
int desired_angle_2 = 0;

float P_1 = 0;
float I_1 = 0;
float D_1 = 0;

float P_2 = 0;
float I_2 = 0;
float D_2 = 0;

const float const_p = 0.0;
const float const_i = 0.0;
const float const_d = 0.0;

float compute_pid_1(int encoder_value) {
    int error = encoder_value - desired_angle_1;
    I_1 += error;
    D_1 = P_1 - error;
    P_1 = error;
    return const_p * P_1 + const_i * I_1 + const_d * D_1;
}

float compute_pid_2(int encoder_value) {
    int error = encoder_value - desired_angle_1;
    I_2 += error;
    D_2 = P_1 - error;
    P_2 = error;
    return const_p * P_2 + const_i * I_2 + const_d * D_2;
}

void set_desired_angle_1(int val) {
    desired_angle_1 = val;
}

void set_desired_angle_2(int val) {
    desired_angle_2 = val;
}
