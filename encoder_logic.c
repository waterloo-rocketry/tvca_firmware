/*
 * File:   encoder_logic.c
 * Author: kavin
 *
 * Created on October 19, 2024, 1:21 PM
 */

#include "encoder_logic.h"
#include <xc.h>

/*
 * Quadrature decoder using CLC 1,2,3,4, TMR 1,3 and SMT 1,2
 *
 * REV-11-1271 has 2048 cycles per revolution requiring 11 bits, so the 8-bit
 * TMR 2,4,6 can't be used. Since each encoder requires 2 timers (one to count
 * up, one to count down), we need 4 timers in total for 2 encoders. Together
 * each encoder uses 2 CLC blocks, one of TMR 1,3,5, and one SMT.
 *
 * The configuation is shown as below, where the encoder count is the
 * difference between TMR and SMT counters.
 *
 *              D1S
 *  ┌─────┐   ┌─────┐     ┌────────────┐
 *  │ENC A├─┬─┤ OR  ├────►│>           │
 *  └─────┘ │ └─────┘     │            │
 *          │   D2S       │            │
 *          │ ┌─────┐     │            │     ┌───┐
 *          │ │ NOT ├────►│D          Q├────►│TMR│
 *          │ └─────┘     │            │     └───┘
 * ┌────────┘   D3S       │            │
 * │          ┌─────┐     │            │
 * │        ┌─┤ OR  ├────►│R           │
 * │        │ └─────┘     └────────────┘
 * │        │   D1S
 * │┌─────┐ │ ┌─────┐     ┌────────────┐
 * ││ENC B├─┴─┤ OR  ├────►│>           │
 * │└─────┘   └─────┘     │            │
 * │            D2S       │            │
 * │          ┌─────┐     │            │     ┌───┐
 * │          │ NOT ├────►│D          Q├────►│SMT│
 * │          └─────┘     │            │     └───┘
 * │            D3S       │            │
 * │          ┌─────┐     │            │
 * └──────────┤ OR  ├────►│R           │
 *            └─────┘     └────────────┘
 *
 * While TMR 1,3,5 has a 16-bit read mode that buffers TMRxH when TMRxL is
 * read, SMT does not have an automatic buffer like this. Therefore an internal
 * pin RB7 is used as SMT window input to manually buffer its count.
 */

static int16_t offset_1;
static int16_t offset_2;

void initialize_encoder() {
    offset_1 = 0;
    offset_2 = 0;

    // PIC18(L)F25/26K83 datasheet section 21.0

    // Select CLC as timer inputs
    T1CLK = 0b01101; // CLC1
    T3CLK = 0b01111; // CLC3

    // Enable 16-bit read mode
    T1CONbits.RD16 = 1;
    T3CONbits.RD16 = 1;

    // Enable timers
    T1CONbits.ON = 1;
    T3CONbits.ON = 1;

    // ==========================================================
    // PIC18(L)F25/26K83 datasheet section 25.0

    // Enable SMT timers
    SMT1CON0bits.EN = 1;
    SMT2CON0bits.EN = 1;

    // Configure SMT to counter mode
    SMT1CON1bits.MODE = 0b1000;
    SMT2CON1bits.MODE = 0b1000;

    // Select SMT input signal
    SMT1SIG = 0b10111; // CLC2_out
    SMT2SIG = 0b11001; // CLC4_out

    // Use RB7 to trigger SMT buffer
    ANSELBbits.ANSELB7 = 0;
    TRISBbits.TRISB7 = 0;
    SMT1WINPPS = 0b001111;
    SMT2WINPPS = 0b001111;

    // Start SMT
    SMT1CON1bits.GO = 1;
    SMT2CON1bits.GO = 1;

    // ==========================================================
    // PIC18(L)F25/26K83 datasheet section 27.0

    // Disable CLC
    CLC1CONbits.EN = 0;
    CLC2CONbits.EN = 0;
    CLC3CONbits.EN = 0;
    CLC4CONbits.EN = 0;

    // Set pins to digital mode
    ANSELAbits.ANSELA3 = 0;
    ANSELAbits.ANSELA4 = 0;
    ANSELBbits.ANSELB3 = 0;
    ANSELBbits.ANSELB5 = 0;

    // Disable pin drivers
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB5 = 1;

    // Connect pins to CLC PPS
    CLCIN0PPS = 0b000011; // A3
    CLCIN1PPS = 0b000100; // A4
    CLCIN2PPS = 0b001011; // B3
    CLCIN3PPS = 0b001101; // B5

    // Select CLCINxPPS in DyS mux
    CLC1SEL0 = 0; // CLC1D1S to CLCIN0PPS
    CLC1SEL1 = 1; // CLC1D2S to CLCIN1PPS
    CLC2SEL0 = 0; // CLC2D1S to CLCIN0PPS
    CLC2SEL1 = 1; // CLC2D2S to CLCIN1PPS
    CLC3SEL0 = 2; // CLC3D1S to CLCIN2PPS
    CLC3SEL1 = 3; // CLC3D2S to CLCIN3PPS
    CLC4SEL0 = 2; // CLC4D1S to CLCIN2PPS
    CLC4SEL1 = 3; // CLC5D2S to CLCIN3PPS

    // Connect inputs to programmable logic
    CLC1GLS0 = 0b00000010; // CLC1D1S to CLK
    CLC1GLS1 = 0b00000000; // 1 to D (inverted)
    CLC1GLS2 = 0b00001000; // CLC1D2S to R
    CLC1GLS3 = 0b00000000; // 0 to S
    CLC1POL  = 0b0010;     // invert D

    CLC2GLS0 = 0b00001000; // CLC2D2S to CLK
    CLC2GLS1 = 0b00000000; // 1 to D (inverted)
    CLC2GLS2 = 0b00000010; // CLC2D1S to R
    CLC2GLS3 = 0b00000000; // 0 to S
    CLC2POL  = 0b0010;     // invert D

    CLC3GLS0 = 0b00000010; // CLC3D1S to CLK
    CLC3GLS1 = 0b00000000; // 1 to D (inverted)
    CLC3GLS2 = 0b00001000; // CLC3D2S to R
    CLC3GLS3 = 0b00000000; // 0 to S
    CLC3POL  = 0b0010;     // invert D

    CLC4GLS0 = 0b00001000; // CLC4D2S to CLK
    CLC4GLS1 = 0b00000000; // 1 to D (inverted)
    CLC4GLS2 = 0b00000010; // CLC4D1S to R
    CLC4GLS3 = 0b00000000; // 0 to S
    CLC4POL  = 0b0010;     // invert D

    // Set CLC logic functions to 1-input D flip-flop with S and R
    CLC1CONbits.MODE = 0b100;
    CLC2CONbits.MODE = 0b100;
    CLC3CONbits.MODE = 0b100;
    CLC4CONbits.MODE = 0b100;

    // Enable CLCx
    CLC1CONbits.EN = 1;
    CLC2CONbits.EN = 1;
    CLC3CONbits.EN = 1;
    CLC4CONbits.EN = 1;
}

int16_t get_encoder_2() {
    // toggle SMTxWIN to latch SMTxTMR to SMTxCPW
    PORTBbits.RB7 = 1;
    PORTBbits.RB7 = 0;

    uint16_t count1 = 0;
    count1 |= (uint32_t) TMR1L; // make sure TMRxL is loaded first for 16-bit read mode to take affect
    count1 |= (uint32_t) TMR1H << 8;

    uint16_t count2 = 0;
    count2 |= (uint32_t) SMT1CPWL;
    count2 |= (uint32_t) SMT1CPWH << 8;
    //count2 |= (uint32_t) SMT1CPWU << 16;
    
    int16_t value = (int16_t)(count1 - count2) + offset_2;
    return value;
}

int16_t get_encoder_1() {
    // toggle SMTxWIN to latch SMTxTMR to SMTxCPW
    PORTBbits.RB7 = 1;
    PORTBbits.RB7 = 0;

    uint16_t count1 = 0;
    count1 |= (uint32_t) TMR3L; // make sure TMRxL is loaded first for 16-bit read mode to take affect
    count1 |= (uint32_t) TMR3H << 8;

    uint16_t count2 = 0;
    count2 |= (uint32_t) SMT2CPWL;
    count2 |= (uint32_t) SMT2CPWH << 8;
    //count2 |= (uint32_t) SMT2CPWU << 16;
    
    int16_t value = (int16_t)(count1 - count2) + offset_1;
    return value;
}

void set_encoder_2(int16_t count) {
    offset_2 += count - get_encoder_2();
}

void set_encoder_1(int16_t count) {
    offset_1 += count - get_encoder_1();
}
