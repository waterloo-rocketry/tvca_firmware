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

int16_t error_1 = 0;
int16_t error_2 = 0;

float P_1 = 0;
float I_1 = 0;
float D_1 = 0;

float P_2 = 0;
float I_2 = 0;
float D_2 = 0;

float kp = 0.02;
float ki = 0.03;
float kd = 0.0;
float saturation = 2;
int16_t deadzone = 0;

float compute_pid_1(int16_t encoder_count) {
    error_1 = target_count_1 - encoder_count;
    if(error_1 > -deadzone && error_1 < deadzone) {
        error_1 = 0;
    }
    I_1 += error_1;
    I_1 = fmax(fmin(I_1, saturation), -saturation);
    D_1 = error_1 - P_1;
    P_1 = error_1;
    return kp * P_1 + ki * I_1 + kd * D_1;
}

float compute_pid_2(int16_t encoder_count) {
    error_2 = target_count_2 - encoder_count;
    if(error_2 > -deadzone && error_2 < deadzone) {
        error_2 = 0;
    }
    I_2 += error_2;
    I_2 = fmax(fmin(I_2, saturation), -saturation);
    D_2 = error_2 - P_2;
    P_2 = error_2;
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

void pid_set_dead(int16_t dead) {
    deadzone = dead;
}
