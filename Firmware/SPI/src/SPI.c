
#include <xc.h>
#include <p32xxxx.h>

#include "SPI.h"

unsigned int ReadSPI1()
{
    /* Check for Receive buffer full status bit of status register*/
    if (SPI1STATbits.SPIRBF)
    {
        SPI1STATbits.SPIROV = 0;

        if (SPI1CONbits.MODE16)
            return (SPI1BUF);           /* return word read */
        else
            return (SPI1BUF & 0xff);    /* return byte read */
    }
    return -1;                          /* RBF bit is not set return error*/
}

unsigned int ReadSPI2()
{
    /* Check for Receive buffer full status bit of status register*/
    if (SPI2STATbits.SPIRBF)
    {
        SPI2STATbits.SPIROV = 0;

        if (SPI2CONbits.MODE16)
            return (SPI2BUF);           /* return word read */
        else
            return (SPI2BUF & 0xff);    /* return byte read */
    }
    return -1;                          /* RBF bit is not set return error*/
}

unsigned int getcSPI1(void)
{
    while (!DataRdySPI1()); // wait receive data available
    return ReadSPI1();
}

unsigned int getcSPI2(void)
{
    while (!DataRdySPI2()); // wait receive data available
    return ReadSPI2();
}

void putcSPI1(unsigned int data_out)
{
    while(!SPI1STATbits.SPITBE);
    SPI1BUF = data_out;
}

void putcSPI2(unsigned int data_out)
{
    while(!SPI2STATbits.SPITBE);
    SPI2BUF = data_out;
    //while(!(SPI1STATbits.SPIRBF));
    //getcSPI2();
}

