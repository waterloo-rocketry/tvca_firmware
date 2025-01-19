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

float const_p = 1.0;
float const_i = 1.0;
float const_d = 0.0;
float saturation = 1.0;

float compute_pid_1(int encoder_value) {
    int error = encoder_value - desired_angle_1;
    I_1 = fmin(I_1 + error, saturation);
    D_1 = error - P_1;
    P_1 = error;
    return const_p * P_1 + const_i * I_1 + const_d * D_1;
}

float compute_pid_2(int encoder_value) {
    int error = encoder_value - desired_angle_1;
    I_2 = fmin(I_2 + error, saturation);
    D_2 = error - P_2;
    P_2 = error;
    return const_p * P_2 + const_i * I_2 + const_d * D_2;
}

void set_desired_angle_1(int val) {
    desired_angle_1 = val;
}

void set_desired_angle_2(int val) {
    desired_angle_2 = val;
}

void set_p(float kp) {
    const_p = kp;
}

void set_i(float ki) {
    cons_i = ki;
}

void set_d(float kd) {
    const_d = kd;
}

void set_s(float sat) {
    saturation = sat;
}
