/* 
 * File:   LEDBoard.h
 * Author: Whiteway
 *
 * Created on March 20, 2015, 11:42 PM
 */

#ifndef LEDBOARD_H
#define	LEDBOARD_H

#define LED_NOOP  0x0
#define DIG0      0x1
#define DIG1      0x2
#define DIG2      0x3
#define DIG3      0x4
#define DIG4      0x5
#define DIG5      0x6
#define DIG6      0x7
#define DIG7      0x8
#define DECODE    0x9
#define INTENSITY 0xA
#define SCANLIMIT 0xB
#define SSSHUTDOWN 0xC
#define TEST      0xF

#define NUM_DIGITS 8

void initLEDBoard(void);
void writeHex(BYTE digit, BYTE value);
void sendCommand2(unsigned short PESend, unsigned short SSSend);
void sendCommand4(BYTE PECommand, BYTE PEData, BYTE SSCommand, BYTE SSData);
unsigned short getPECommand4(BYTE pins, BYTE b3Value, BYTE b2Value, BYTE b1Value, BYTE b0Value);
unsigned short getPECommand2(BYTE pins, BYTE b1Value, BYTE b0Value);
unsigned short getPECommand1(BYTE pins, BYTE b0Value);


#endif	/* LEDBOARD_H */

