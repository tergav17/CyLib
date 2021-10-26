/*
 * adc.c
 *
 *  Created on: Oct 19, 2021
 *      Author: strinco
 */

#include "adc.h"

void adc_init() {
    // Init RCGCGPIO port B
    SYSCTL_RCGCGPIO_R |= 0b10;

    // Set AFSEL for PB4
    GPIO_PORTB_AFSEL_R |= 0b10000;

    // Set direction for PB4
    GPIO_PORTB_DIR_R &= 0xFFFFFFEF;

    // Set digital disable for PB4
    GPIO_PORTB_DEN_R &= 0xFFFFFFEF;

    // Set analog enable for RB4
    GPIO_PORTB_AMSEL_R |= 0b10000;

    // Set ADC control
    GPIO_PORTB_ADCCTL_R = 0x00;


    // GPIO pin setup complete, ADC time!

    // Enable ADC0 clock
    SYSCTL_RCGCADC_R |= 0b01;

    // Wait a little bit
    timer_waitMicros(150);

    // Disable SS1
    ADC0_ACTSS_R &= ~0b0010;

    // Configure SSMUX1
    ADC0_SSMUX1_R = (ADC0_SSMUX1_R & 0xFFFFFFF0) | 10;

    // Configure SS1 sequence
    ADC0_SSCTL1_R = (ADC0_SSCTL1_R & 0xFFFFFFF0) | 0b110;

    // Reset event multiplexor select for SS1
    ADC0_EMUX_R &= 0xFF0F;

    // Set over-sample rate to 32x
    ADC0_SAC_R = 0x5;

    // Enable SS1
    ADC0_ACTSS_R |= 0b0010;
}

int adc_read() {
    adc_read_begin();

    return adc_read_finish();
}

void adc_read_begin() {
    // Tell the ADC to start the read
    ADC0_PSSI_R |= 0b10;
}

int adc_read_finish() {
    // Wait for the ADC to finish
    while (!(ADC0_RIS_R & 0b10));

    // Grab the value from the FIFO
    int out = ADC0_SSFIFO1_R;

    // Reset flag
    ADC0_ISC_R |= 0b10;

    return out;


}
