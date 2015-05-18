
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "SPI.h"

#include "PortExpander.h"

void initPE()
{
    configPort(P12_15_CONFIG, CONST_CUR, CONST_CUR, CONST_CUR, CONST_CUR);
    configPort(P16_19_CONFIG, OUTPUT, OUTPUT, OUTPUT, OUTPUT);
    configPort(P20_23_CONFIG, OUTPUT, OUTPUT, OUTPUT, OUTPUT);
    configPort(P24_27_CONFIG, OUTPUT, OUTPUT, OUTPUT, OUTPUT);
    configPort(P28_31_CONFIG, OUTPUT, OUTPUT, OUTPUT, OUTPUT);

    writePort(RW_BYTE_12, 0xFF);
    writePort(RW_BYTE_20, 0xFF);
    writePort(RW_BYTE_28, 0xFF);

    setLEDCurrent(P12_13_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    setLEDCurrent(P14_15_CURRENT, LED_CURRENT_5, LED_CURRENT_5);
    writePort(CONFIGURATION, NORMAL | INDIVIDUAL);
}

void configPort(BYTE pins, BYTE b3Value, BYTE b2Value, BYTE b1Value, BYTE b0Value)
{
    int sendVal = WRITE;
    sendVal |= (pins << 8);
    sendVal |= (b3Value << 6);
    sendVal |= (b2Value << 4);
    sendVal |= (b1Value << 2);
    sendVal |= (b0Value);

    while(SPI2STATbits.SPIBUSY);
    putcSPI2(sendVal);
}

void setLEDCurrent(BYTE pins, BYTE b1Value, BYTE b0Value)
{
    int sendVal = WRITE;
    sendVal |= (pins << 8);
    sendVal |= (b1Value << 4);
    sendVal |= (b0Value);

    while(SPI2STATbits.SPIBUSY);
    putcSPI2(sendVal);
}

void writePort(BYTE pins, BYTE value)
{
    int sendVal = WRITE;
    sendVal |= (pins << 8);
    sendVal |= value;

    while(SPI2STATbits.SPIBUSY);
    putcSPI2(sendVal);
}

BYTE readPort(BYTE pins)
{
    BYTE returnVal = 0;
    int sendVal = READ;
    sendVal |= (pins << 8);

    while(SPI2STATbits.SPIBUSY);
    putcSPI2(sendVal);

    sendVal = READ;

    while(SPI2STATbits.SPIBUSY);
    putcSPI2(sendVal);
    returnVal = getcSPI2();

    return returnVal;
}
