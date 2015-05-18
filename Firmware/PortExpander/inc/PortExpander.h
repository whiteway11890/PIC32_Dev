/* 
 * File:   PortExpander.h
 * Author: Whiteway
 *
 * Created on March 18, 2015, 6:59 PM
 */

#ifndef PORTEXPANDER_H
#define	PORTEXPANDER_H

//Read/Write Defines
#define READ  0x8000
#define WRITE 0x0000
#define NOOP  0x00

#define RW_PORT_12 0x2C
#define RW_PORT_13 0x2D
#define RW_PORT_14 0x2E
#define RW_PORT_15 0x2F
#define RW_PORT_16 0x30
#define RW_PORT_17 0x31
#define RW_PORT_18 0x32
#define RW_PORT_19 0x33
#define RW_PORT_20 0x34
#define RW_PORT_21 0x35
#define RW_PORT_22 0x36
#define RW_PORT_23 0x37
#define RW_PORT_24 0x38
#define RW_PORT_25 0x39
#define RW_PORT_26 0x3A
#define RW_PORT_27 0x3B
#define RW_PORT_28 0x3C
#define RW_PORT_29 0x3D
#define RW_PORT_30 0x3E
#define RW_PORT_31 0x3F

#define RW_BYTE_05 0x45
#define RW_BYTE_06 0x46
#define RW_BYTE_07 0x47
#define RW_BYTE_08 0x48
#define RW_BYTE_09 0x49
#define RW_BYTE_10 0x4A
#define RW_BYTE_11 0x4B
#define RW_BYTE_12 0x4C
#define RW_BYTE_13 0x4D
#define RW_BYTE_14 0x4E
#define RW_BYTE_15 0x4F
#define RW_BYTE_16 0x50
#define RW_BYTE_17 0x51
#define RW_BYTE_18 0x52
#define RW_BYTE_19 0x53
#define RW_BYTE_20 0x54
#define RW_BYTE_21 0x55
#define RW_BYTE_22 0x56
#define RW_BYTE_23 0x57
#define RW_BYTE_24 0x58
#define RW_BYTE_25 0x59
#define RW_BYTE_26 0x5A
#define RW_BYTE_27 0x5B
#define RW_BYTE_28 0x5C
#define RW_BYTE_29 0x5D
#define RW_BYTE_30 0x5E
#define RW_BYTE_31 0x5F

#define CONFIGURATION 0x04
#define GLOBAL        0x00
#define INDIVIDUAL    0x40
#define NORMAL        0x01
#define SHUTDOWN      0x00

//Configuration defines
#define P12_15_CONFIG 0x0B
#define P16_19_CONFIG 0x0C
#define P20_23_CONFIG 0x0D
#define P24_27_CONFIG 0x0E
#define P28_31_CONFIG 0x0F

#define CONST_CUR  0x00
#define OUTPUT     0x01
#define INPUT      0x02
#define IN_PULLUP  0x03

//LED current setting
#define P12_13_CURRENT 0x16
#define P14_15_CURRENT 0x17
#define P16_17_CURRENT 0x18
#define P18_19_CURRENT 0x19
#define P20_21_CURRENT 0x1A
#define P22_23_CURRENT 0x1B
#define P24_25_CURRENT 0x1C
#define P26_27_CURRENT 0x1D
#define P28_29_CURRENT 0x1E
#define P30_31_CURRENT 0x1F

#define LED_CURRENT_1  0x00
#define LED_CURRENT_2  0x01
#define LED_CURRENT_3  0x02
#define LED_CURRENT_4  0x03
#define LED_CURRENT_5  0x04
#define LED_CURRENT_6  0x05
#define LED_CURRENT_7  0x06
#define LED_CURRENT_8  0x07
#define LED_CURRENT_9  0x08
#define LED_CURRENT_A  0x09
#define LED_CURRENT_B  0x0A
#define LED_CURRENT_C  0x0B
#define LED_CURRENT_D  0x0C
#define LED_CURRENT_E  0x0D
#define LED_CURRENT_F  0x0E
#define LED_CURRENT_10 0x0F

void configPort(BYTE pins, BYTE b3Value, BYTE b2Value, BYTE b1Value, BYTE b0Value);
void setLEDCurrent(BYTE pins, BYTE b1Value, BYTE b0Value);
void writePort(BYTE pins, BYTE value);
BYTE readPort(BYTE pins);
void initPE(void);


#endif	/* PORTEXPANDER_H */

