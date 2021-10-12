/*
 * cyrial.h
 *
 *  Created on: Oct 4, 2021
 *      Author: tergav17
 */

#ifndef CYRIAL_H_
#define CYRIAL_H_

#define DIV_MIDDLE 0
#define DIV_TOPLEFT 1
#define DIV_TOP 2
#define DIV_TOPRIGHT 3
#define DIV_RIGHT 4
#define DIV_BOTTOMRIGHT 5
#define DIV_BOTTOM 6
#define DIV_BOTTOMLEFT 7
#define DIV_LEFT 8

#include "uart.h"
#include <stdio.h>
#include <stdlib.h>

void cyrial_init(void);

void uart_sendln(char* str);
void uart_sends(char* str);
void uart_recv(char* str);

void term_putc(char c);
char term_getc(void);
void term_puts(char *str);
void term_gets(char* str);

void dash_init(int x, int y);
void dash_add_label(char* name, char* contents, int x, int y, int width, int height, char centered, char boxed);
void dash_add_button(char* name, char* contents, int x, int y, int width, int height);
void dash_add_input(char* name, char* contents, int x, int y, int width, int height);
void dash_add_divider(char* name, int x1, int y1, int pos1, int x2, int y2, int pos2);
void dash_add_icon(char* name, char* state, int x, int y);
void dash_add_logger(char* name, char* title, int x, int y, int width, int height, char* axis, double max);
void dash_add_plotter(char* name, char* title, int x, int y, int width, int height, char* xaxis, char* yaxis);

void dash_poll(char* name, char* str);

void dash_radial_half(void);
void dash_radial_full(void);
void dash_radial_done(void);
void dash_radial_max(int max);
void dash_radial_log(int in);

void dash_label_update(char* name, char* contents);
void dash_icon_update(char* name, char* status);

void dash_logger_bound(char* name, double min, double max);
void dash_logger_log(char* name, double value, char* color);

void dash_plotter_erase(char* name);
void dash_plotter_linear(char* name);
void dash_plotter_nonlinear(char* name);
void dash_plotter_color(char* name, char* color);
void dash_plotter_bound(char* name, double xmin, double xmax, double ymin, double ymax);
void dash_plotter_plot(char* name, double x, double y);

#endif /* CYRIAL_H_ */
