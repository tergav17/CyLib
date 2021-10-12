/*
 * imu.c
 *
 *  Created on: Oct 6, 2021
 *      Author: Gavin Tersteeg
 */

#include "imu.h"

void imu_init() {
    // Turn on clocks for port A and port B
    SYSCTL_RCGCGPIO_R |= 0b11;

    // Turn on I2C1
    SYSCTL_RCGCI2C_R |= 0b10;

    // Enable I2C lines on port A
    GPIO_PORTA_DEN_R |= 0b11000000;

    // Configure alternate functions for said lines
    GPIO_PORTA_AFSEL_R |= 0b11000000;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0x00FFFFFF) | 0x33000000;

    // Set SDA pin as open drain
    GPIO_PORTA_ODR_R |= 0b10000000;

    // Enable I2C1 master function
    I2C1_MCR_R = 0x0010;

    // Configure I2C1 clock speed (assumed 100Khz @ 16Mhz system clock)
    I2C1_MTPR_R = 0x07;

    // Enable RSTN and ADDR lines on port B
    // INT will be ignored because we do not care about interrupts for now
    GPIO_PORTB_DEN_R |= 0b11000000;

    // Set directions for these lines
    GPIO_PORTB_DIR_R |= 0b11000000;

    // Ensure that ADDR line is low for I2C address of 0x28
    // Also set RSTN low to start a reset
    GPIO_PORTB_DATA_R &= 0b00111111;

    // Bad delay, but I don't care
    int i = 0;
    for (i = 0; i < 1000; i++);

    // Set RSTN to high
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0xFFFFFF3F) | 0b01000000;
    //GPIO_PORTB_DATA_R &= 0b00111111;


}

int i2c_wait() {
    // Wait till bus not busy...
    while (I2C1_MCS_R & 0b1);

    // Return an error (if one)
    return I2C1_MCS_R & 0xE;
}

char i2c_write(int addr, char reg, int cnt, char *data) {
    char error;

    // Sanity error checking
    if (cnt <= 0) return 1;

    // Send slave address and register
    I2C1_MSA_R = addr << 1;
    I2C1_MDR_R = reg;
    I2C1_MCS_R = 3;

    // Wait and record error
   error = i2c_wait();
   if (error) return error;

   // Send data
   while (cnt > 1) {
       I2C1_MDR_R = *(data++);
       I2C1_MCS_R = 1;
       error = i2c_wait();
       if (error) return error;
       cnt--;
   }

   I2C1_MDR_R = *data;
   I2C1_MCS_R = 5;

   // Wait and record error
   error = i2c_wait();
   while (I2C1_MCS_R & 0x40);
   if (error) return error;
   return 0;

}

char i2c_read(char addr, char reg, int cnt, char *data) {
    char error;

    // Sanity error checking
    if (cnt <= 0) return 1;

    // Send slave address and register
    I2C1_MSA_R = addr << 1;
    I2C1_MDR_R = reg;
    I2C1_MCS_R = 3;

    // Wait and record error
    error = i2c_wait();
    if (error) return error;

    // Change from write to read, send restart with slave addr
    I2C1_MSA_R = (addr << 1) | 0b1;

    // If last byte, don't ack
    // Otherwise do
    if (cnt == 1) I2C1_MCS_R = 7;
    else I2C1_MCS_R = 0xB;

    // Wait and record error
    error = i2c_wait();
    if (error) return error;

    // Record data
    *(data++) = I2C1_MDR_R;

    // If single byte read, be done
    if (--cnt == 0) {
        while (I2C1_MCS_R & 0x40);
        return 0;
    }

    // Read the rest of the bytes
    while (cnt > 1) {
        I2C1_MCS_R = 9;
        error = i2c_wait();
        if (error) return error;
        cnt--;
        *(data++) = I2C1_MDR_R;
    }

    // NACK
    I2C1_MCS_R = 5;
    i2c_wait();
    *data = I2C1_MDR_R;

    // Wait for bus not busy
    while (I2C1_MCS_R & 0x40);

    // Return no error
    return 0;


}

void gyro_init() {
    // Set to fusion mode
    timer_waitMillis(100);

    char out = 0x0C;
    i2c_write(0x29, 0x3D, 1, &out);

    timer_waitMillis(100);

    // Set units

    out = 0xC0;
    i2c_write(0x29, 0x3B, 1, &out);

}

void gyro_cali_wait() {
    while (1) {
        timer_waitMillis(10);

        char out;
        i2c_read(0x29, 0x35, 1, &out);

        if ((out & 0b00110000) == 0b00110000) {
            gyro_yaw_zero();
            return;
        }
    }


}

short gyro_yaw_raw() {
    short out;
    i2c_read(0x29, 0x1A, 2, (char*) &out);

    return out;
}

short gyro_roll_raw() {
    short out;
    i2c_read(0x29, 0x1C, 2, (char*) &out);

    return out;
}

short gyro_pitch_raw() {
    short out;
    i2c_read(0x29, 0x1E, 2, (char*) &out);

    return out;
}


short gyro_ang_vel_raw() {
    short out;
    i2c_read(0x29, 0x18, 2, (char*) &out);

    return out;
}

short gyro_yaw_raw_norm(short in) {
    while (in < 0) in += 5760;
    while (in >= 5760) in -= 5760;

    return in;
}

void gyro_yaw_zero() {
    yaw_zero_point = gyro_yaw_raw();
}

double gyro_yaw() {
    short yaw = gyro_yaw_raw() - yaw_zero_point;

    return gyro_yaw_raw_norm(yaw) / 16.0;
}

double gyro_roll() {
    return gyro_roll_raw() / 16.0;
}

double gyro_pitch() {
    return gyro_pitch_raw() / 16.0;
}

