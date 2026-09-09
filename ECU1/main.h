#ifndef MAIN_H
#define	MAIN_H
#define _XTAL_FREQ  20000000
#define LINE1(x)    (0x80 + (x))
#define LINE2(x)    (0xC0 + (x))
#define RS  RC1
#define RW  RC0
#define EN  RC2
#define CHANNEL4		0x04
#define CHANNEL5		0x05
#define CHANNEL6		0x06
void indicator_conf();
void init_clcd();
void clcd_write(unsigned char byte, unsigned char type);
void clcd_print(const char *data, unsigned char addr);
void init_adc(void);
unsigned short read_adc(unsigned char channel);

#endif	

