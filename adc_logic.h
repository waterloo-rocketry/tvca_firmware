/* 
 * File:   adc_logic.h
 * Author: kavin
 *
 * Created on November 1, 2024, 8:22 PM
 */

#ifndef ADC_LOGIC_H
#define	ADC_LOGIC_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint16_t adc_result_t;
#ifndef uint24_t
typedef __uint24 uint24_t;
#endif


typedef enum
{
    channel_CUR_AMP =  0x00,
    channel_CUR_1 =  0x08,
    channel_CUR_2 =  0x17,
    channel_VBAT_2 =  0x12,
    channel_VBAT_1 =  0x0C,
} adcc_channel_t;

/**
  Section: ADCC Module APIs
*/

/**
 * @ingroup adcc
 * @brief Initializes the ADC module. This routine is called before any other ADC routine.
 * @param None.
 * @return None.
*/
void ADCC_Initialize(void);

/**
 * @ingroup adcc
 * @brief Retrieves the result of a single ADC conversion on any given channel.
 * @pre Enable ADC using the ADCC_Initialize() function. Also ADCC_DisableContinuousConversion() function should have been called before calling this function
 * @param channel - Analog channel number on which ADC conversion will be applied.
 *                  Refer to adcc_channel_t  for available channels
 * @return The result of a single ADC conversion. Refer to the adc_result_t.
*/
adc_result_t ADCC_GetSingleConversion(adcc_channel_t channel);


double convert_adc_to_voltage(adc_result_t value);

double convert_adc_to_motor_current(adc_result_t value);

double convert_adc_to_cur_amp(adc_result_t value);
#endif	/* ADC_LOGIC_H */

