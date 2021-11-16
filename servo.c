/*
 * servo.c
 *
 *  Created on: Nov 2, 2021
 *      Author: strinco
 */
#include "servo.h"

void servo_init() {
    // Turn on clock for port B
    SYSCTL_RCGCGPIO_R |= 0b10;

    // Turn on digital enable for port BD5
    GPIO_PORTB_DEN_R |= 0b100000;

    // Set pin direction as output
    GPIO_PORTB_DIR_R |= 0b100000;

    // Configure PCTL function for T1CCP1
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0xF00000) | 0x700000;

    // Enable AFSEL operation
    GPIO_PORTB_AFSEL_R |= 0b100000;

    // Enable timer clock
    SYSCTL_RCGCTIMER_R |= 0b10;

    // Timer disable
    TIMER1_CTL_R = (TIMER1_CTL_R & ~0xFFF) | 0b000000000000;

    // Configure timer bit size
    TIMER1_CFG_R = 0x4;

    // Configure timer settings
    TIMER1_TBMR_R = (TIMER1_TBMR_R & ~0x8FF) | 0b100000001010;

    // Preset
    servo_turn(20000);

    // Timer enable
    TIMER1_CTL_R = (TIMER1_CTL_R & ~0xFFF) | 0b000100000000;
}

void servo_turn(int pos) {

    int start_time = MS20 + pos;

    TIMER1_TBILR_R = start_time;
    TIMER1_TBPR_R = (start_time >> 16);

    TIMER1_TBMATCHR_R = MS20;
    TIMER1_TBPMR_R = (MS20 >> 16);


}
