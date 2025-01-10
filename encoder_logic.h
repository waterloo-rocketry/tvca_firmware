/* 
 * File:   encoder.h
 * Author: kavin
 *
 * Created on October 19, 2024, 1:19 PM
 */

#ifndef ENCODER_H
#define	ENCODER_H

void initialize_encoder(void);

void encoder_interrupt_handler(void);

int get_encoder_1(void);

int get_encoder_2(void);

#endif	/* ENCODER_H */

