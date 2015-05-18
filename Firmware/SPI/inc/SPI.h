/*
 * File:   SPI.h
 * Author: Whiteway
 *
 * Created on September 21, 2014, 12:24 PM
 */

#ifndef SPI_H
#define	SPI_H

#define DataRdySPI1() (SPI1STATbits.SPIRBF)
#define DataRdySPI2() (SPI1STATbits.SPIRBF)

unsigned int ReadSPI1(void);
unsigned int ReadSPI2(void);
unsigned int getcSPI1(void);
unsigned int getcSPI2(void);
void putcSPI1(unsigned int data_out);
void putcSPI2(unsigned int data_out);

#endif	/* SPI_H */

