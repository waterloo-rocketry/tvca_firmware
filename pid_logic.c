/*
 * File:   pid_logic.c
 * Author: kavin
 *
 * Created on November 16, 2024, 11:52 AM
 */


#include <xc.h>
#include "pid_logic.h"
#include <math.h>

int16_t target_count_1 = 0;
int16_t target_count_2 = 0;

float P_1 = 0;
float I_1 = 0;
float D_1 = 0;

float P_2 = 0;
float I_2 = 0;
float D_2 = 0;

float kp = 0.2;
float ki = 0.0;
float kd = 0.01;
float saturation = 1.0;

float compute_pid_1(int16_t encoder_count, float dt) {
    int16_t error = target_count_1 - encoder_count;
    I_1 = fmin(I_1 + error, saturation); // TODO FIXME
    D_1 = error - P_1;
    P_1 = error;
    return kp * P_1 + ki * I_1 + kd * D_1;
}

float compute_pid_2(int16_t encoder_count, float dt) {
    int16_t error = target_count_2 - encoder_count;
    I_2 += error * dt;
    I_2 = fmax(fmin(I_2, saturation), -saturation);
    D_2 = (error - P_2) / dt;
    P_2 = error;
    return kp * P_2 + ki * I_2 + kd * D_2;
}

void pid_set_target_count_1(int16_t val) {
    target_count_1 = val;
}

void pid_set_target_count_2(int16_t val) {
    target_count_2 = val;
}

void pid_set_kp(float k) {
    kp = k;
}

void pid_set_ki(float k) {
    ki = k;
}

void pid_set_kd(float k) {
    kd = k;
}

void pid_set_sat(float sat) {
    saturation = sat;
}
