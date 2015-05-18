
#include "GenericTypeDefs.h"
#include "KeyboardRoutines.h"
#include "Keyboard.h"

extern BYTE NUM_Lock_Pressed;

void handleKeyPress(BYTE data , BYTE HIDData)
{
    if((HIDData>=0x1E && HIDData<=0x27) || (HIDData>=0x04 && HIDData<=0x1D) ||
            (HIDData>=0x2D && HIDData<=0x38) || ((HIDData>=0x59 && HIDData<=0x62)&&(NUM_Lock_Pressed == 1)))
    {
        //Normal keys
        manipulatePiece(data);
    }
    else if(HIDData == Symbol_LeftArrow || HIDData == Symbol_UpArrow || HIDData == Symbol_RightArrow || HIDData == Symbol_DownArrow)
    {
        manipulatePiece(HIDData);
    }
}
