/*
 * cymove.h
 *
 *  Created on: Oct 9, 2021
 *      Author: tergav17
 */

#ifndef CYMOVE_H_
#define CYMOVE_H_

#include "imu.h"
#include "open_interface.h"
#include "math.h"

oi_t *cm_sensor_data;
double cm_cur_angle;

typedef struct {
    char bumped;
    double distance;
} cm_report;

void cm_init();
void cm_turn(double goal);
void cm_turn_relative(double goal);
cm_report cm_drive(double distance, char bump_stop);
double cm_normalize(double in);
void cm_correct(double goal, char backtrack);

double cm_error_rot(double pos);
double cm_error(double pos, double goal);
#endif /* CYMOVE_H_ */
