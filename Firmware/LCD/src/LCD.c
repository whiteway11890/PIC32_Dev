
#include "HardwareProfile.h"
#include "Fonts.h"
#include "LCD.h"

int count;
int spiDummy;

void writeData(BYTE data, int command) {
    //Set D/C bit
    LCD_DC_IO = command;

    //Write to the SPI
    LCD_CS_IO = 0;
    putcSPI2(data);
    spiDummy = getcSPI2();
    LCD_CS_IO = 1;
}

BYTE readData(BYTE command)
{
    BYTE val;
    LCD_DC_IO = COMMAND;

    //Write to the SPI
    LCD_CS_IO = 0;
    putcSPI2(command);
    val = getcSPI2();

    LCD_DC_IO = DATA;
    putcSPI2(0x00);
    val = getcSPI2();
    LCD_CS_IO = 1;

    return val;
}

void setPixel(int x, int y, int color)
{
    if((x < 0) ||(x >= ILI9340_TFTWIDTH) || (y < 0) || (y >= ILI9340_TFTHEIGHT)) return;

    setAddrWindow(x,y,x+1,y+1);

    LCD_DC_IO = DATA;
    LCD_CS_IO = 0;

    SPI2CONbits.MODE16 = 1;
    putcSPI2(color);
    putcSPI2(color);
    SPI2CONbits.MODE16 = 0;

    LCD_CS_IO = 1;
}

void drawString(char * s, int x, int y, WORD color)
{
    int numChars = 0;
    int width, height, charNum;
    char c;
    BYTE column;

    while(s[numChars] != '\0'){
        numChars++;
    }

    width = 7*numChars;
    height = 8;

    if ((x + width - 1) >= ILI9340_TFTWIDTH) width = ILI9340_TFTWIDTH - x;
    if ((y + height - 1) >= ILI9340_TFTHEIGHT) height = ILI9340_TFTHEIGHT - y;

    setAddrWindow(x, y, x + width - 1, y + height - 1);

    LCD_DC_IO = DATA;
    LCD_CS_IO = 0;

    SPI2CONbits.MODE16 = 1;
    for (y = height-1; y >= 0; y--) {
        for(charNum = 0; charNum < numChars; charNum++){
            c = s[charNum];

            for (x = 0; x < 5; x++) {
                column = ASCII_5_7[c - 0x20][x];

                if (column & (0x01 << y)){ // test bits to set pixels
                    putcSPI2(color);
                } else {
                    putcSPI2(ILI9340_BLACK);
                }
            }
            //Two spaces between characters
            putcSPI2(ILI9340_BLACK);
            putcSPI2(ILI9340_BLACK);
        }
    }
    putcSPI2(ILI9340_BLACK);
    SPI2CONbits.MODE16 = 0;

    LCD_CS_IO = 1;
}

void drawChar(char c, int x, int y, WORD color)
{
    BYTE column;
    int i, j;

    for(i=0; i<5; i++){
        column = ASCII_5_7[c - 0x20][i];

        for (j=0; j<8; j++) // 8 rows (y) per character
        {
            if (column & (0x01 << j)){ // test bits to set pixels
                setPixel(x+i, y+7-j, color);
            } else {
                setPixel(x+i, y+7-j, ILI9340_BLACK);
            }
        }
    }
}

void fillScreen(WORD color) {
    fillRect(0, 0, ILI9340_TFTWIDTH, ILI9340_TFTHEIGHT, color);
}

// fill a rectangle
void fillRect(SHORT x, SHORT y, SHORT w, SHORT h, WORD color) {

    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= ILI9340_TFTWIDTH) || (y >= ILI9340_TFTHEIGHT)) return;
    if ((x + w - 1) >= ILI9340_TFTWIDTH) w = ILI9340_TFTWIDTH - x;
    if ((y + h - 1) >= ILI9340_TFTHEIGHT) h = ILI9340_TFTHEIGHT - y;

    setAddrWindow(x, y, x + w - 1, y + h - 1);

    LCD_DC_IO = DATA;
    LCD_CS_IO = 0;

    SPI2CONbits.MODE16 = 1;
    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            putcSPI2(color);
        }
    }
    putcSPI2(color);
    SPI2CONbits.MODE16 = 0;

    LCD_CS_IO = 1;
}

void drawBitmap(int x, int y, int width, int height, const WORD * ptr)
{
    int pixX = 0, pixY = 0;

    if ((x >= ILI9340_TFTWIDTH) || (y >= ILI9340_TFTHEIGHT)) return;
    if ((x + width - 1) >= ILI9340_TFTWIDTH) width = ILI9340_TFTWIDTH - x;
    if ((y + height - 1) >= ILI9340_TFTHEIGHT) height = ILI9340_TFTHEIGHT - y;

    setAddrWindow(x, y, x + width - 1, y + height - 1);

    LCD_DC_IO = DATA;
    LCD_CS_IO = 0;

    SPI2CONbits.MODE16 = 1;
    for (pixY = 0; pixY < height; pixY++) {
        for (pixX = 0; pixX < width; pixX++) {
            putcSPI2(ptr[pixY * width + pixX]);
        }
    }
    putcSPI2(ptr[pixY * width + pixX]);

    //putcSPI2(color);
    SPI2CONbits.MODE16 = 0;

    LCD_CS_IO = 1;
}

void setAddrWindow(WORD x0, WORD y0, WORD x1, WORD y1) {
    writeData(ILI9340_CASET, COMMAND); // Column addr set
    writeData(x0 >> 8, DATA);
    writeData(x0 & 0xFF, DATA); // XSTART
    writeData(x1 >> 8, DATA);
    writeData(x1 & 0xFF, DATA); // XEND

    writeData(ILI9340_PASET, COMMAND); // Row addr set
    writeData(y0 >> 8, DATA);
    writeData(y0, DATA); // YSTART
    writeData(y1 >> 8, DATA);
    writeData(y1, DATA); // YEND

    writeData(ILI9340_RAMWR, COMMAND); // write to RAM
}

void initLCD() {
    //Reset high
    LCD_CS_IO = 1;
    LCD_RST_IO = 1;
    for (count = 0; count < 10000; count++);

    //Turn reset low
    LCD_RST_IO = 0;
    for (count = 0; count < 10000; count++);

    //Reset high
    LCD_RST_IO = 1;
    for (count = 0; count < 100000; count++);

    writeData(ILI9340_DISPOFF, COMMAND); //Display off

    writeData(ILI9340_SLPOUT, COMMAND); //Exit sleep
    writeData(ILI9340_NOP, DATA);

    writeData(ILI9340_PWCTR1, COMMAND); //Power Control 1
    writeData(0x26, DATA);
    writeData(0x04, DATA);

    writeData(ILI9340_PWCTR2, COMMAND); //Power Control 2
    writeData(0x04, DATA);

    writeData(ILI9340_VMCTR1, COMMAND); //VCM 1
    writeData(0x34, DATA);
    writeData(0x40, DATA);

    writeData(ILI9340_MADCTL, COMMAND); //BGR Data
    writeData(0x88, DATA);

    writeData(ILI9340_FRMCTR1, COMMAND); //Frame rate control
    writeData(0x00, DATA);
    writeData(0x18, DATA);

    writeData(ILI9340_DFUNCTR, COMMAND); //Display function control
    writeData(0x0A, DATA);
    writeData(0xA2, DATA);

    writeData(ILI9340_VMCTR2, COMMAND); //VCM data control
    writeData(0xC0, DATA);

    writeData(ILI9340_PIXFMT, COMMAND); //Pixel Format
    writeData(0x55, DATA);

    writeData(ILI9340_GAMMASET, COMMAND); //Gamma curve selected
    writeData(0x01, DATA);

    writeData(ILI9340_GMCTRP1, COMMAND); //Pos gamma correction
    writeData(0x1F, DATA);
    writeData(0x1B, DATA);
    writeData(0x18, DATA);
    writeData(0x0B, DATA);
    writeData(0x0F, DATA);
    writeData(0x09, DATA);
    writeData(0x46, DATA);
    writeData(0xB5, DATA);
    writeData(0x37, DATA);
    writeData(0x0A, DATA);
    writeData(0x0C, DATA);
    writeData(0x07, DATA);
    writeData(0x07, DATA);
    writeData(0x05, DATA);
    writeData(0x00, DATA);

    writeData(ILI9340_GMCTRN1, COMMAND); //Neg Gamma correction
    writeData(0x00, DATA);
    writeData(0x24, DATA);
    writeData(0x27, DATA);
    writeData(0x04, DATA);
    writeData(0x10, DATA);
    writeData(0x06, DATA);
    writeData(0x39, DATA);
    writeData(0x74, DATA);
    writeData(0x48, DATA);
    writeData(0x05, DATA);
    writeData(0x13, DATA);
    writeData(0x38, DATA);
    writeData(0x38, DATA);
    writeData(0x3A, DATA);
    writeData(0x1F, DATA);

    writeData(ILI9340_CASET, COMMAND); //Column address set
    writeData(0x00, DATA);
    writeData(0x00, DATA);
    writeData(0x00, DATA);
    writeData(0xEF, DATA);

    writeData(ILI9340_PASET, COMMAND); //Page address set
    writeData(0x00, DATA);
    writeData(0x00, DATA);
    writeData(0x01, DATA);
    writeData(0x3F, DATA);

    writeData(ILI9340_SLPOUT, COMMAND); //Exit Sleep
    for (count = 0; count < 100000; count++);

    writeData(ILI9340_DISPON, COMMAND); //Display on

    //Set the screen black
    fillScreen(ILI9340_BLACK);
}
