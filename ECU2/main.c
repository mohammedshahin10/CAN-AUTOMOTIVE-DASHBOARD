/*
 * File:   main.c
 * Author: supre
 *
 * Created on 1 December, 2025, 6:31 PM
 */
#include <xc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"main.h"
#include "can.h"
#include"msg_id.h"
#include"message_handler.h"
unsigned char str[] = "Gear : ";
unsigned char atr[] = "_C";
unsigned char mtr[6];
unsigned char mst[6];

void init_conf() {
    init_gear();
    // init_clcd();
    init_adc();

    init_can();

}

void main(void) {
    init_conf();
    unsigned int sw1 = 1, sw2 = 1, sw3 = 1, var = 0;
    unsigned short int val = 0;
    unsigned int gear = 0, or = 0;
    while (1) {
        var = read_adc(CHANNEL4);
        val = ((var / 10.23));
        can_transmit(SPEED_MSG_ID, &val, 1);
        for (int delay = 300; delay--;);

        if ((gear == 7) &&(RC0 == 0 && sw1 == 1)) {
            gear = 0;
            sw1 = 0;
        } 
        else if ((gear == 7) &&(RC1 == 0 && sw2 == 1)) {
            gear = 0;
            sw2 = 0;
        } else {


            if (RC0 == 0 && sw1 == 1) {

                gear++;
                sw1 = 0;
            } else if (RC0 == 1) {

                sw1 = 1;
            }
            if (RC1 == 0 && sw2 == 1) {

                gear--;
                sw2 = 0;
            } else if (RC1 == 1) {
                sw2 = 1;
            }
            if (RC2 == 0 && sw3 == 1) {
                gear = 7;

                sw3 = 0;
            } else if (RC2 == 1) {
                sw3 = 1;
            }
        }

        can_transmit(GEAR_MSG_ID, &gear, 1);
        for (int delay = 100; delay--;);
    }
}
