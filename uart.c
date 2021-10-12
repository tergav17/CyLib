/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"

void uart_init(void) {
    SYSCTL_RCGCGPIO_R |= 0b10;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b10;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R = 0b11;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) | 0x11;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   = 0b11;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R  &= 0xFFFFFFFE;
    GPIO_PORTB_DIR_R  |= 0b10;        // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz]
    int    fbrd;
    int    ibrd;

    fbrd = 44; // page 903
    ibrd = 8;

    UART1_CTL_R &= 0xFFFFFFFE;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = (UART1_LCRH_R & 0xFFFFFF00) | 0b01100000;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0b1;        // enable UART1
}

void uart_sendChar(char data) {
    while (!(UART1_FR_R & 0x80));
	UART1_DR_R = data;
}

char uart_receive(void) {
    while (UART1_FR_R & 0x10);
	return UART1_DR_R;
}

void uart_sendStr(const char *data) {
	while (*data != 0) uart_sendChar(*(data++));
}

void uart_interrupt_init() {
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0b10000; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0b1000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler() {
// STEP1: Check the Masked Interrupt Status
    if (UART1_MIS_R & 0b10000) {

        //STEP2:  Copy the data
        uart_sendChar(uart_receive());

        //STEP3:  Clear the interrupt
        UART1_ICR_R |= 0b10000;
    }

}
