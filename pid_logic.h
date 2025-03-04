/* 
 * File:   pid_logic.h
 * Author: kavin
 *
 * Created on November 16, 2024, 11:59 AM
 */

#ifndef PID_LOGIC_H
#define	PID_LOGIC_H

float compute_pid_1(int16_t encoder_count, float dt);
float compute_pid_2(int16_t encoder_count, float dt);

void pid_set_target_count_1(int16_t val);
void pid_set_target_count_2(int16_t val);

void pid_set_kp(float kp);
void pid_set_ki(float ki);
void pid_set_kd(float kd);
void pid_set_sat(float sat);

#endif	/* PID_LOGIC_H */

