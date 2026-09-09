/*
 * File:   main.c
 * Author: supre
 *
 * Created on 1 December, 2025, 11:54 AM
 */


#include <xc.h>
#include "main.h"
#include<stdlib.h>
#include "can.h"
#include<stdio.h>
#include<string.h>
#include "msg_id.h"
#include "message_handler.h"
unsigned char str[] = "<-           ->";
unsigned char ste[] = "             ->";
unsigned char stf[] = "<-             ";
unsigned char mtr[5];

void init_conf() {
    indicator_conf();
    // init_clcd();
    init_adc();
    init_can();
}

void main(void) {
    init_conf();
    unsigned int once = 0, var = 0;
    unsigned short int val = 0, or = 0;
    while (1) {
        var = read_adc(CHANNEL4);
        val = ((var / 10.23));
        can_transmit(RPM_MSG_ID, &val, 1);
        for (int delay = 100; delay--;);

        if (RC0 == 0) {

            once = 1;
        }
        if (RC1 == 0) {

            once = 2;
        }
        if (RC2 == 0) {

            once = 3;
        }
        if (RC3 == 0) {

            once = 4;
        }
        if (once == 2) {
            or = 2;
            once = 0;
        }
        if (once == 3) {

            or = 3;
            once = 0;
        }
        if (once == 1) {
            or = 1;
            once = 0;
        }
        if (once == 4) {
            or = 4;
            once = 0;
        }

        can_transmit(INDICATOR_MSG_ID, &or, 1);
        for (int delay = 200; delay--;);

    }

}
