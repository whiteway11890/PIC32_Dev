/* 
 * File:   main.h
 * Author: Whiteway
 *
 * Created on September 21, 2014, 6:26 PM
 */

#ifndef MAIN_H
#define	MAIN_H

int ethernetMain(void);
void LCDWrite(int type, BYTE data);
void LCDMoveCursor(int x, int y);
void LCDSetContrast(BYTE contrast);

#endif	/* MAIN_H */

