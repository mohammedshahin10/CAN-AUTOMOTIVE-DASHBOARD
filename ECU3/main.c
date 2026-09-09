#include <xc.h>
#include<stdio.h>
#include<stdlib.h>
#include "can.h"
#include "clcd.h"
#include "message_handler.h"
#include "msg_id.h"

char rpm[5];
char speed[4];
char mtr[4];
unsigned char blink = 0;

void int_to_str1(unsigned int n, char *s, int size) {
    for (int i = 0; i < size; i++) {
        s[size - i - 1] = n % 10 + 48;
        n = n / 10;
    }
    s[size] = '\0';
}

void int_to_str(unsigned int n, char *s) {
    s[4] = 0;
    s[3] = (n % 10) + '0';
    n /= 10;
    s[2] = (n % 10) + '0';
    n /= 10;
    s[1] = (n % 10) + '0';
    n /= 10;
    s[0] = (n % 10) + '0';
}

void can_bus(void) {

    unsigned int mes_id;
    unsigned char data;
    unsigned char len;

    can_receive(&mes_id, &data, &len);

    switch (mes_id) {
        case SPEED_MSG_ID:
        {
            unsigned int spd = data;
            //            spd = spd / 10.23;
            int_to_str1(spd, speed, 3);
            clcd_print(speed, LINE2(10));
        }
            break;

        case RPM_MSG_ID:
        {
            uint16_t rpm_val;
            rpm_val = data * 60;
            int_to_str(rpm_val, rpm);
            clcd_print(rpm, LINE2(0));
        }
            break;

        case GEAR_MSG_ID:
            if(data > 0 && data <6)
            {
                clcd_print("  ", LINE2(14));
                 sprintf(mtr, "%u", data);
                clcd_print(mtr, LINE2(14));
            }
            else if(data == 0)
            {
               clcd_print("  ", LINE2(14));
                 
                clcd_print("N ", LINE2(14)); 
            }
           else if(data == 6)
            {
               clcd_print("  ", LINE2(14));
                 
                clcd_print("R ", LINE2(14)); 
            }
            else if(data == 7)
            {
               clcd_print("   ", LINE2(14));
                 
                clcd_print("_c", LINE2(14)); 
            }
            break;

        case INDICATOR_MSG_ID:
            if (data == 1) {

                clcd_print("  ", LINE2(7));
                clcd_print("<-", LINE2(5));

            } else if (data == 2) {

                clcd_print("  ", LINE2(5));
                clcd_print("->", LINE2(7));

            } else if (data == 3) {

                clcd_print("<-->", LINE2(5));

            } else if (data == 4) {
                clcd_print("    ", LINE2(5));
            }
            break;

        default:
            break;
    }
}

void init_config() {

    init_can();
    init_clcd();
}

void main(void) {
    init_config();
    while (1) {
        clcd_print("RPM  IND  SPD GR",LINE1(0));
        can_bus();
    }
    return;
}