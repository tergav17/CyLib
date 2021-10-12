/*
 * cymove.c
 *
 *  Created on: Oct 9, 2021
 *      Author: tergav17
 */

#include "cymove.h"
#include "cyrial.h"


// Ensure the gyro in init before use
void cm_init() {
    cm_sensor_data = oi_alloc();
    oi_init(cm_sensor_data);

    cm_cur_angle = gyro_yaw();
    oi_setWheels(0, 0);
}

void cm_turn(double goal) {
    goal = cm_normalize(goal);

    cm_cur_angle = goal;

    // Control loop

    char cont = 1;
    double last_err;
    char stuck = 1;
    char still_cnt = 0;

    if (fabs(cm_error_rot(gyro_yaw())) < 0.5) cont = 0;

    while (cont) {
        double yaw = gyro_yaw();
        double err = cm_error_rot(yaw);
        double dir = (err < 0) ? -1 : 1;

        if (fabs(err) > 40) {
            // If the error is too big, just run at full blast to reduce error fast
            oi_setWheels((int16_t) (dir * -500), (int16_t) (dir * 500));
        } else if (fabs(err) > 10) {
            // Start to slow down
            oi_setWheels((int16_t) (err * -1), (int16_t) (err * 1));
        } else {
            // Rough correction
            oi_setWheels((int16_t) (dir * -20), (int16_t) (dir * 20));
        }

        if (stuck) {
            oi_setWheels((int16_t) (dir * -150), (int16_t) (dir * 150));
            stuck = 0;
        }

        if (cont == 1) {
            cont = 2;
        } else {
            double deriv = err - last_err;

            if (fabs(deriv) < 0.001) {
                still_cnt++;

                if (still_cnt > 10) {
                    stuck = 1;
                }
            } else {
                still_cnt = 0;
            }

            if (fabs(err) < 3 && fabs(err) > 1.5 && fabs(deriv) < 0.3 && fabs(deriv) > 0.1) {

                oi_setWheels(0, 0);

                //char str[64];

                //sprintf(str, "[CYBOT] Deriv, Err: %f,%f\n", deriv, err);
                //term_puts(str);

                cont = 0;

            }
        }

        last_err = err;
        timer_waitMillis(10);
    }

    cm_correct(goal, 1);
}

void cm_turn_relative(double goal) {
    cm_turn(cm_cur_angle + goal);
}

void cm_correct(double goal, char backtrack) {
    int corr_delay = 21;

    goal = cm_normalize(goal);

    cm_cur_angle = goal;

    char cont = 1;
    char first = 0;
    double gen_dir = 0;
    while (cont) {
        double yaw = gyro_yaw();
        double err = cm_error_rot(yaw);
        double dir = (err < 0) ? -1 : 1;

        // Set the general direction if backtrack is disabled
        if (!first && !backtrack) gen_dir = dir;
        first = 1;

        oi_setWheels((int16_t) (dir * -150), (int16_t) (dir * 150));

        timer_waitMillis(corr_delay);

        oi_setWheels(0, 0);

        timer_waitMillis(100);

        yaw = gyro_yaw();
        double err_last = cm_error_rot(yaw);
        dir = (err_last < 0) ? -1 : 1;

        if ((dir > 0 && gen_dir < 0) || (dir < 0 && gen_dir > 0)) {
            if (!backtrack) cont = 0;
        }

        //char str[64];

        //sprintf(str, "[CYBOT] Delay: %d\n", corr_delay);
        //term_puts(str);


        if (fabs(err_last) < 0.5) {
            cont = 0;
        } else {
            if (err - err_last < 0.001) corr_delay++;
        }
    }
}

cm_report cm_drive(double distance, char bump_stop) {
    // Reset everything
    oi_update(cm_sensor_data);

    char cont = 1;
    double pos = 0;
    double last_dir;
    int switch_cnt = 0;
    char bump = 0;
    while (cont) {
        oi_update(cm_sensor_data);
        pos += cm_sensor_data->distance;

        if (cm_sensor_data->bumpLeft || cm_sensor_data->bumpRight) bump = 1;

        double err = cm_error(pos, distance);
        double dir = (err < 0) ? -1 : 1;

        if (fabs(err) > 40) {
            // If the error is too big, just run at full blast to reduce error fast
            oi_setWheels((int16_t) (dir * 500), (int16_t) (dir * 500));
        } else if (fabs(err) > 10) {
            // Start to slow down
            oi_setWheels((int16_t) (err * 1), (int16_t) (err * 1));
        } else {
            oi_setWheels((int16_t) (dir * 30), (int16_t) (dir * 30));
        }

        if (switch_cnt > 1 || (bump_stop && bump)) {
            oi_setWheels(0, 0);
            cont = 0;
        }

        if (cont == 1) {
            cont = 2;
        } else {
            if ((last_dir > 0 && dir < 0) || (last_dir < 0 && dir > 0)) switch_cnt++;
        }

        last_dir = dir;

        timer_waitMillis(10);
    }

    cm_report rep;
    rep.bumped = bump;
    rep.distance = pos;

    return rep;
}

double cm_normalize(double in) {
    while (in >= 360.0) in -= 360.0;
    while (in < 0) in += 360.0;

    return in;
}

double cm_error_rot(double pos) {
    double err = cm_cur_angle - pos;

    if (err > 180.0) err -= 360.0;
    if (err < -180.0) err += 360.0;

    return err;
}

double cm_error(double pos, double goal) {
    double err = goal - pos;

    return err;
}
