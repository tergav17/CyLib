/*
 * servo.h
 *
 *  Created on: Nov 2, 2021
 *      Author: strinco
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"

#define MS20 320000



void servo_init(void);
void servo_turn(int pos);


#endif /* SERVO_H_ */
