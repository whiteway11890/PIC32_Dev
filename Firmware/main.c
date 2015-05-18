/*
 * File:   main.c
 * Author: Whiteway
 *
 * Created on September 10, 2014, 10:27 PM
 */

#define _SUPPRESS_PLIB_WARNING

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <xc.h>
#include <peripheral/int.h>

#include "GenericTypeDefs.h"
#include "Config.h"
#include "Defines.h"
#include "HardwareProfile.h"

#include "SPI.h"
#include "LEDBoard.h"
#include "PortExpander.h"
#include "main.h"

void init() {

    int tmp;

    //Default all to inputs
    TRISA = 0xFFFFFFFF;
    TRISB = 0xFFFFFFFF;
    LATA = 0;
    LATB = 0;

    //Unlock
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    //Set oscillator
    OSCCONbits.PBDIV = 3;
    OSCCONbits.COSC = 1; //Internal FRC Oscillator
    OSCCONbits.PLLMULT = 0;

    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR); //configure multi vector
    INTEnableSystemMultiVectoredInt(); //enable interrupts

    ANSELA = 0; //Turn off Port A Analog Channe,ls
    ANSELB = 0; //Turn off Port B Analog Channels

    //Set the SPI pins
    PE_SDO_TRIS = TRIS_OUTPUT;
    PE_SCK_TRIS = TRIS_OUTPUT;
    PE_CS_TRIS  = TRIS_OUTPUT;
    PE_SDI_TRIS = TRIS_INPUT;

    LED_SDO_TRIS = TRIS_OUTPUT;
    LED_SCK_TRIS = TRIS_OUTPUT;
    LED_CS_TRIS  = TRIS_OUTPUT;
    LED_SDI_TRIS = TRIS_INPUT;

    //Unlock PPS pins
    CFGCONbits.IOLOCK = 0;
    //PE
    SDI2R = RPB2;
    RPB1R = SDO2;
    RPB0R = SS2;

    //LED
    SDI1R = RPB8;
    RPA4R = SDO1;
    RPB7R = SS1;

    //Relock PPS pins
    CFGCONbits.IOLOCK = 1;

    //Enable SPI1 for the Port Expander
    IEC1bits.SPI1EIE = 0; //Disable  Interrupts
    IEC1bits.SPI1RXIE = 0; //Disable Interrupts
    IEC1bits.SPI1TXIE = 0; //Disable Interrupts
    SPI1CONbits.ON = 0; //Make sure module is off.
    tmp = SPI1BUF; //Dummy read to clear data
    SPI1CONbits.MSSEN = 1; //SPI controls SS pin
    SPI1CONbits.MSTEN = 1; //Master Mode
    SPI1CONbits.CKP = 0;
    SPI1CONbits.CKE = 1;
    SPI1STAT = 0; //Clear out status
    SPI1BRG = 0x1; //Slow down the Master Clock
    SPI1CONbits.MODE32 = 1; //32 bit writes.
    SPI1CONbits.ON = 1; //SPI2 on

    //Enable SPI2 for the Port Expander
    IEC1bits.SPI2EIE = 0; //Disable  Interrupts
    IEC1bits.SPI2RXIE = 0; //Disable Interrupts
    IEC1bits.SPI2TXIE = 0; //Disable Interrupts
    SPI2CONbits.ON = 0; //Make sure module is off.
    tmp = SPI2BUF; //Dummy read to clear data
    SPI2CONbits.MSSEN = 1; //SPI controls SS pin
    SPI2CONbits.MSTEN = 1; //Master Mode
    SPI2CONbits.CKP = 0;
    SPI2CONbits.CKE = 1;
    SPI2STAT = 0; //Clear out status
    SPI2BRG = 0x0; //Slow down the Master Clock
    SPI2CONbits.MODE16 = 1; //16 bit writes.
    SPI2CONbits.ON = 1; //SPI2 on

    //Set up interrupts
    INTCON = 0; //Disable overflow errors
    INTCONbits.MVEC = 1; //Each interrupt has its own vector

    //Relock
    SYSKEY = 0x33333333;
}

int main(void) {

    init();
    initPE();
    initLEDBoard();

    //Main loop
    while(1){
        
    }

    return (EXIT_SUCCESS);
}

