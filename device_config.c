#include "device_config.h"
#include <xc.h>

void oscillator_init(void)
{
    // Select external oscillator with PLL of 4:1
    OSCCON1 = 0x20;
    //wait until the clock switch has happened
    while (OSCCON3bits.ORDY == 0)  {}
    //if the currently active clock (CON2) isn't the selected clock (CON1)
    if (OSCCON2 != 0x20) {
        // try to wait for the oscillator to come back
        for(uint16_t i = 0; i < 60000; i++) {}
    }
}

// These configuration bits were generating using MCC and then copy/pasted here.
// Everything is default except for the first two lines which set up the external oscillator.


// PIC18LF26K83 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator Selection (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = HFINTOSC_1MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 4 MHz and CDIV = 4:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = OFF     // Multi-vector enable bit (Interrupt contoller does not use vector table to prioritze interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_8// WDT Period selection bits (Divider ratio 1:8192)
#pragma config WDTE = ON        // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_6// WDT Window Select bits (window always open (100%); no software control; keyed access required)
#pragma config WDTCCS = LFINTOSC// WDT input clock selector (WDT reference clock is the 31.0 kHz LFINTOSC)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// CONFIG5H

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
