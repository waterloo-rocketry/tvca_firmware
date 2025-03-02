/* 
 * File:   encoder.h
 * Author: kavin
 *
 * Created on October 19, 2024, 1:19 PM
 */

#ifndef ENCODER_H
#define	ENCODER_H

#include <stdint.h>

void initialize_encoder(void);

int16_t get_encoder_1(void);
int16_t get_encoder_2(void);

void set_encoder_1(int16_t count);
void set_encoder_2(int16_t count);

#endif	/* ENCODER_H */

