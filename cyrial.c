/*
 * cyrial.c
 *
 *  Created on: Oct 4, 2021
 *      Author: tergav17
 */

#include "cyrial.h"

void cyrial_init() {
    uart_init();
}

void uart_sendln(char* str) {
    uart_sends(str);
    uart_sends("\n");
}

void uart_sends(char* str) {
    while (*str != 0) uart_sendChar(*(str++));
}

void uart_recv(char* str) {
    char in = 1;
    while (in) {
        in = uart_receive();
        if (in == '\n') in = '\0';

        if (in != 13) *(str++) = in;
    }
}

void term_putc(char c) {
    char buf[8];

    sprintf(buf, "C%d", (int) c);
    uart_sendln(buf);
}

char term_getc() {
    char buf[8];

    uart_sendln("R");
    uart_recv(buf);

    return (char) atoi(buf);
}

void term_puts(char *str) {
    while (*str != 0) term_putc(*(str++));
}

void term_gets(char *str) {
    char *start = str;

    char in = 1;
    while (in != '\n') {
        in = term_getc();
        if (in != 0) {

            if (in != 8) {
                term_putc(in);
                *(str++) = in;
            } else {
                if (str != start) {
                    term_putc(in);
                    str--;
                }
            }
        }
    }
    *str = 0;
}

void dash_init(int x, int y) {
    while (term_getc() != 0);

    char buf[64];

    sprintf(buf, "I,%d,%d", x, y);
    uart_sendln(buf);
}

void dash_add_label(char* name, char* contents, int x, int y, int width, int height, char centered, char boxed) {
    char buf[128];

    sprintf(buf, "A,T,%s,%s,%d,%d,%d,%d,%d,%d", name, contents, x, y, width, height, (int) centered, (int) boxed);
    uart_sendln(buf);
}

void dash_add_button(char* name, char* contents, int x, int y, int width, int height) {
    char buf[64];

    sprintf(buf, "A,B,%s,%s,%d,%d,%d,%d", name, contents, x, y, width, height);
    uart_sendln(buf);
}

void dash_add_input(char* name, char* contents, int x, int y, int width, int height) {
    char buf[64];

    sprintf(buf, "A,I,%s,%s,%d,%d,%d,%d", name, contents, x, y, width, height);
    uart_sendln(buf);
}

void dash_add_divider(char* name, int x1, int y1, int pos1, int x2, int y2, int pos2) {
    char buf[64];

    sprintf(buf, "A,D,%s,%d,%d,%d,%d,%d,%d", name, x1, y2, pos1, x2, y2, pos2);
    uart_sendln(buf);
}

void dash_add_icon(char* name, char* state, int x, int y) {
    char buf[64];

    sprintf(buf, "A,S,%s,%s,%d,%d", name, state, x, y);
    uart_sendln(buf);
}

void dash_add_logger(char* name, char* title, int x, int y, int width, int height, char* axis, double max) {
    char buf[64];

    sprintf(buf, "A,L,%s,%s,%d,%d,%d,%d,%s,%.3f", name, title, x, y, width, height, axis, max);
    uart_sendln(buf);
}

void dash_add_plotter(char* name, char* title, int x, int y, int width, int height, char* xaxis, char* yaxis) {
    char buf[64];

    sprintf(buf, "A,P,%s,%s,%d,%d,%d,%d,%s,%s", name, title, x, y, width, height, xaxis, yaxis);
    uart_sendln(buf);
}

void dash_poll(char* name, char* str) {
    char buf[32];

    sprintf(buf, "E,%s", name);
    uart_sendln(buf);
    uart_recv(str);
}

void dash_radial_half(void) {
    uart_sendln("S,H");
}

void dash_radial_full(void) {
    uart_sendln("S,F");
}

void dash_radial_done(void) {
    uart_sendln("S,X");
}

void dash_radial_max(int max) {
    char buf[32];

    sprintf(buf, "S,M,%d", max);
    uart_sendln(buf);
}

void dash_radial_log(int in) {
    char buf[32];

    sprintf(buf, "S,%d", in);
    uart_sendln(buf);
}

void dash_label_update(char* name, char* contents) {
    char buf[128];

    sprintf(buf, "U,%s,%s", name, contents);
    uart_sendln(buf);
}

void dash_icon_update(char* name, char* status) {
    dash_label_update(name, status);
}

void dash_logger_bound(char* name, double min, double max) {
    char buf[64];

    sprintf(buf, "U,%s,B,%.3f,%.3f", name, min, max);
    uart_sendln(buf);
}

void dash_logger_log(char* name, double value, char* color) {
    char buf[64];

    sprintf(buf, "U,%s,%.3f,%s", name, value, color);
    uart_sendln(buf);
}

void dash_plotter_erase(char* name) {
    char buf[32];

    sprintf(buf, "U,%s,E", name);
    uart_sendln(buf);
}

void dash_plotter_linear(char* name) {
    char buf[32];

    sprintf(buf, "U,%s,L", name);
    uart_sendln(buf);
}

void dash_plotter_nonlinear(char* name) {
    char buf[32];

    sprintf(buf, "U,%s,N", name);
    uart_sendln(buf);
}

void dash_plotter_color(char* name, char* color) {
    char buf[64];

    sprintf(buf, "U,%s,C,%s", name, color);
    uart_sendln(buf);
}

void dash_plotter_bound(char* name, double xmin, double xmax, double ymin, double ymax) {
    char buf[64];

    sprintf(buf, "U,%s,B,%.3f,%.3f,%.3f,%.3f", name, xmin, xmax, ymin, ymax);
    uart_sendln(buf);
}

void dash_plotter_plot(char* name, double x, double y) {
    char buf[64];

    sprintf(buf, "U,%s,%.3f,%.3f", name, x, y);
    uart_sendln(buf);
}
