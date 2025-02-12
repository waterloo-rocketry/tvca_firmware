/* 
 * File:   pid_logic.h
 * Author: kavin
 *
 * Created on November 16, 2024, 11:59 AM
 */

#ifndef PID_LOGIC_H
#define	PID_LOGIC_H

float compute_pid_1(int encoder_value);
float compute_pid_2(int encoder_value);

void set_desired_angle_1(int val);
void set_desired_angle_2(int val);

void set_p(float kp);
void set_i(float ki);
void set_d(float kd);
void set_s(float sat);

#endif	/* PID_LOGIC_H */

