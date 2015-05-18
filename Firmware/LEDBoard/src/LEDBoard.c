
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "SPI.h"

#include "PortExpander.h"
#include "LEDBoard.h"

unsigned short peCommand;
unsigned short ssCommand;

void initLEDBoard()
{
    //Initialize the port expander

    //Switches as inputs
    peCommand = getPECommand4(P12_15_CONFIG, INPUT, INPUT, INPUT, INPUT);
    //Test mode off
    ssCommand = getPECommand1(TEST, 0);
    sendCommand2(peCommand, ssCommand);

    peCommand = getPECommand4(P16_19_CONFIG, CONST_CUR, CONST_CUR, CONST_CUR, CONST_CUR);
    ssCommand = getPECommand1(INTENSITY, 8);
    sendCommand2(peCommand, ssCommand);

    peCommand = getPECommand4(P20_23_CONFIG, CONST_CUR, CONST_CUR, CONST_CUR, CONST_CUR);
    ssCommand = getPECommand1(SCANLIMIT, 7);
    sendCommand2(peCommand, ssCommand);

    peCommand = getPECommand4(P24_27_CONFIG, CONST_CUR, CONST_CUR, CONST_CUR, CONST_CUR);
    ssCommand = getPECommand1(DECODE, 0);
    sendCommand2(peCommand, ssCommand);

    peCommand = getPECommand4(P28_31_CONFIG, CONST_CUR, CONST_CUR, CONST_CUR, CONST_CUR);
    ssCommand = getPECommand1(SSSHUTDOWN, 1);
    sendCommand2(peCommand, ssCommand);

    peCommand = getPECommand2(P16_17_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    ssCommand = getPECommand1(TEST, 1);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P18_19_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P20_21_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P22_23_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P24_25_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P26_27_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P28_29_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand2(P30_31_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand1(CONFIGURATION, NORMAL | INDIVIDUAL);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand1(RW_BYTE_16,0xFF);
    sendCommand2(peCommand, LED_NOOP);

    peCommand = getPECommand1(RW_BYTE_24,0xFF);
    sendCommand2(peCommand, LED_NOOP);

    //Clear the hex
    int i;
    for(i=1; i<=NUM_DIGITS; i++){
        writeHex(i, 0);
    }
}

void writeHex(BYTE digit, BYTE value)
{
    ssCommand = getPECommand1(digit, value);
    sendCommand2(NOOP, ssCommand);
}

void sendCommand2(unsigned short PESend, unsigned short SSSend)
{
    unsigned int sendVal;
    sendVal = (PESend << 16) | SSSend;

    while(SPI1STATbits.SPIBUSY);
    putcSPI1(sendVal);
}

void sendCommand4(BYTE PECommand, BYTE PEData, BYTE SSCommand, BYTE SSData)
{
    unsigned short PESend = WRITE | (PECommand << 8) | PEData;
    unsigned short SSSend = SSCommand | SSData;

    sendCommand2(PESend, SSSend);
}

unsigned short getPECommand4(BYTE pins, BYTE b3Value, BYTE b2Value, BYTE b1Value, BYTE b0Value)
{
    unsigned short sendVal = WRITE;
    sendVal |= (pins << 8);
    sendVal |= (b3Value << 6);
    sendVal |= (b2Value << 4);
    sendVal |= (b1Value << 2);
    sendVal |= (b0Value);

    return sendVal;
}

unsigned short getPECommand2(BYTE pins, BYTE b1Value, BYTE b0Value)
{
    unsigned short sendVal = WRITE;
    sendVal |= (pins << 8);
    sendVal |= (b1Value << 4);
    sendVal |= (b0Value);

    return sendVal;
}

unsigned short getPECommand1(BYTE pins, BYTE b0Value)
{
    unsigned short sendVal = WRITE;
    sendVal |= (pins << 8);
    sendVal |= (b0Value);

    return sendVal;
}
