/* 
 * File:   adc_logic.c
 * Author: kavin
 *
 * Created on November 1, 2024, 8:23 PM
 */

#include <xc.h>
#include "adc_logic.h"

void ADCC_Initialize(void)
{
    // Setup pins
    
    // RA0 - Cur Amp
    // RB0 - Cur 1
    // RC7 - Cur 2
    // RC2 - Vbat 2
    // RB4 - Vbat 1
    
    TRISAbits.TRISA0 = 1;
    TRISBbits.TRISB0 = 1;
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC2 = 1;
    TRISBbits.TRISB4 = 1;
    
    ANSELAbits.ANSELA0 = 1; 
    ANSELBbits.ANSELB0 = 1;
    ANSELCbits.ANSELC7 = 1;
    ANSELCbits.ANSELC2 = 1;
    ANSELBbits.ANSELB4 = 1;

    // b[7] enables FVR
    // b[1:0] sets the reference to 4.096
    FVRCON = 0b10000011;
    
    
    // set the ADCC to the options selected in the User Interface
    //ADLTHL 0; 
    ADLTHL = 0x0;
    //ADLTHH 0; 
    ADLTHH = 0x0;
    //ADUTHL 0; 
    ADUTHL = 0x0;
    //ADUTHH 0; 
    ADUTHH = 0x0;
    //ADSTPTL 0; 
    ADSTPTL = 0x0;
    //ADSTPTH 0; 
    ADSTPTH = 0x0;
    //ADACCU 0x0; 
    ADACCU = 0x0;
    //ADRPT 0; 
    ADRPT = 0x0;
    //ADCHS ANA0; 
    ADPCH = 0x0;
    //ADACQL 1; 
    ADACQL = 0x1;
    //ADACQH 0; 
    ADACQH = 0x0;
    //ADCAP Additional uC disabled; 
    ADCAP = 0x0;
    //ADPREL 0; 
    ADPREL = 0x0;
    //ADPREH 0; 
    ADPREH = 0x0;
    //ADDSEN disabled; ADGPOL digital_low; ADIPEN disabled; ADPPOL Vss; 
    ADCON1 = 0x0;
    //ADCRS 1; ADMD Basic_mode; ADACLR disabled; ADPSIS RES; 
    ADCON2 = 0x10;
    //ADCALC First derivative of Single measurement; ADTMD disabled; ADSOI ADGO not cleared; 
    ADCON3 = 0x0;
    //ADMATH registers not updated; 
    ADSTAT = 0x0;
    //ADNREF VSS; ADPREF FVR; 
    ADREF = 0x3;
    //ADACT disabled; 
    ADACT = 0x0;
    //ADCCS FOSC/64; 
    ADCLK = 0x1F;
    //GO_nDONE undefined; ADFM right; ADON enabled; ADCS FOSC; ADCONT disabled; 
    ADCON0 = 0x84;
    
    //Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;

    //Clear the ADC Threshold interrupt flag
    PIR1bits.ADTIF = 0;
}

adc_result_t ADCC_GetSingleConversion(adcc_channel_t channel)
{
    //Selects the A/D channel
    ADPCH = channel;  
    
    //Disables the continuous mode
    ADCON0bits.ADCONT = 0;

    //Starts the conversion
    ADCON0bits.ADGO = 1;


    //Waits for the conversion to finish
    while (ADCON0bits.ADGO)
    {
    }
    
    //Conversion finished, returns the result
    return ((adc_result_t)((ADRESH << 8) + ADRESL));
}

// return in mA or mV
uint16_t adc_read_channel(adcc_channel_t channel) {
    adc_result_t value = ADCC_GetSingleConversion(channel);
    float mvolt = value * 4096.0 / 4095.0;

    switch(channel) {
        case channel_CUR_AMP:
            return mvolt / 866.0 * 1900.0; // volt / R * K1

        case channel_CUR_1:
        case channel_CUR_2:
            return mvolt / 0.2 / 20.0; // V / R / INA180 Gain

        case channel_VBAT_1:
        case channel_VBAT_2:
            return mvolt * (10.0 + 21.5) / 10.0; // V / divider
    }

    return value;
}
