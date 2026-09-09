/*
 * File:   gear.c
 * Author: supre
 *
 * Created on 1 December, 2025, 6:33 PM
 */


#include <xc.h>
#include"main.h"
void  init_gear()
{
    TRISC = (TRISC & 0X0F);
    PORTC = 0x00;
  //  TRISB = ((TRISB & 0X01)|0X1E);
  //  RBPU = 0;
    
}
void clcd_write(unsigned char byte, unsigned char type) {   //function
    RS = type;
    PORTD = byte;


    EN = 1;  
    EN = 0;

    TRISD7 = 0xff;
    RW = 1;
    RS = 0;

    do {
        EN = 1;
        EN = 0;
    } while (RD7);

    RW = 0;
    TRISD7 = 0x00;
}

void init_clcd(void) {
    TRISD = 0x00;
    TRISC = TRISC & 0xF8;
    RW = 0;
    __delay_ms(30);
    clcd_write(0x33, 0);   //function call
    __delay_us(4100);       //delay
    clcd_write(0x33, 0);   //function call
    __delay_us(100);       //delay
    clcd_write(0x33, 0);   //function call
    __delay_us(1);


    clcd_write(0x02, 0);   //function call
    __delay_us(100);        //delay
    clcd_write(0x38, 0);     //function call
    __delay_us(100);        //delay
    clcd_write(0x01, 0);      //function call
    __delay_us(500);            //delay
    clcd_write(0x0C, 0);       //function call
    __delay_us(100);            //delay
}
void clcd_print(const char *data, unsigned char addr) {  //function
    clcd_write(addr, 0);
    while (*data != '\0')
    {
        clcd_write(*data++, 1);
    }
}
void init_adc(void)
{
	/* Selecting right justified ADRES Registers order */
	ADFM = 1;

	/* 
	 * Acqusition time selection bits 
	 * Set for 4 Tad
	 */
	ACQT2 = 0;
	ACQT1 = 1;
	ACQT0 = 0;

	/*
	 * Selecting the conversion clock of Fosc / 32 -> 1.6usecs -> 1Tad
	 * Our device frequency is 20 MHz
	 */
	ADCS0 = 0;
	ADCS1 = 1;
	ADCS2 = 0;

	/* Stop the conversion to start with */
	GODONE = 0;

	

	/* Voltage reference bit as VSS */
	VCFG1 = 0;
	/* Voltage reference bit as VDD */
	VCFG0 = 0;

	/* Just clearing the ADRESH & ADRESL registers, for time pass */
	ADRESH = 0;
	ADRESL = 0;

	/* Turn ON the ADC module */
	ADON = 1;
}

unsigned short read_adc(unsigned char channel)
{
	unsigned short reg_val;

	/*select the channel*/
	ADCON0 = (ADCON0 & 0xC3) | (channel << 2);

	/* Start the conversion */
	GO = 1;
	while (GO);
	reg_val = (ADRESH << 8) | ADRESL; 

	return reg_val;
}

