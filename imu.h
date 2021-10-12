/*
 * imu.h
 *
 *  Created on: Oct 6, 2021
 *      Author: Gavin Tersteeg
 */

#ifndef IMU_H_
#define IMU_H_

#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>
#include "Timer.h"
#include "driverlib/interrupt.h"

short yaw_zero_point;

void imu_init();
int i2c_wait();
char i2c_write(int addr, char reg, int cnt, char *data);
char i2c_read(char addr, char reg, int cnt, char *data);

void gyro_init();
void gyro_cali_wait();
short gyro_yaw_raw();
short gyro_roll_raw();
short gyro_pitch_raw();
short gyro_ang_vel_raw();
short gyro_yaw_raw_norm(short in);
void gyro_yaw_zero();
double gyro_yaw();
double gyro_roll();
double gyro_pitch();



#endif /* IMU_H_ */
