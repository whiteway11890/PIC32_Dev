
#include "usb_config.h"
#include "usb.h"
#include "usb_host_hid.h"
#include "usb_host_hid_parser.h"
#include "../KeyboardRoutines.h"
#include "../USBFunctions.h"

extern BOOL ReportBufferUpdated;

extern BYTE ErrorCounter;
extern BYTE ErrorDriver;
extern BYTE NumOfBytesRcvd;

extern HID_DATA_DETAILS Appl_LED_Indicator;
extern HID_DATA_DETAILS Appl_ModifierKeysDetails;
extern HID_DATA_DETAILS Appl_NormalKeysDetails;
extern HID_DATA_DETAILS Appl_ModifierKeysDetails;
extern HID_DATA_DETAILS Appl_NormalKeysDetails;

extern HID_USER_DATA_SIZE Appl_BufferModifierKeys[8];
extern HID_USER_DATA_SIZE Appl_BufferNormalKeys[6];
extern HID_USER_DATA_SIZE Appl_ShadowBuffer1[6];

extern HID_REPORT_BUFFER     Appl_raw_report_buffer;
extern HID_LED_REPORT_BUFFER Appl_led_report_buffer;

extern APP_STATE App_State_Keyboard;

void USBStateMachine(void) {

    int i;

    USBTasks();
    App_Detect_Device();
    switch (App_State_Keyboard) {
        case DEVICE_NOT_CONNECTED:
            USBTasks();
            if (USBHostHID_ApiDeviceDetect()){ // True if report descriptor is parsed with no error
                App_State_Keyboard = DEVICE_CONNECTED;
            }
            break;
        case DEVICE_CONNECTED:
            App_State_Keyboard = READY_TO_TX_RX_REPORT;
            InitializeTimerFour(); // start 10ms timer to schedule input reports

            break;
        case READY_TO_TX_RX_REPORT:
            if (!USBHostHID_ApiDeviceDetect()) {
                App_State_Keyboard = DEVICE_NOT_CONNECTED;
                //                                DisplayOnce = FALSE;
            }
            break;
        case GET_INPUT_REPORT:
            if (USBHostHID_ApiGetReport(Appl_raw_report_buffer.Report_ID, Appl_ModifierKeysDetails.interfaceNum,
                    Appl_raw_report_buffer.ReportSize, Appl_raw_report_buffer.ReportData)) {
                ;
            } else {
                App_State_Keyboard = INPUT_REPORT_PENDING;
            }
            USBTasks();
            break;
        case INPUT_REPORT_PENDING:
            if (USBHostHID_ApiTransferIsComplete(&ErrorDriver, &NumOfBytesRcvd)) {
                if (ErrorDriver || (NumOfBytesRcvd != Appl_raw_report_buffer.ReportSize)) {
                    ErrorCounter++;
                    if (MAX_ERROR_COUNTER <= ErrorDriver)
                        App_State_Keyboard = ERROR_REPORTED;
                    else
                        App_State_Keyboard = READY_TO_TX_RX_REPORT;
                } else {
                    ErrorCounter = 0;
                    ReportBufferUpdated = TRUE;
                    App_State_Keyboard = READY_TO_TX_RX_REPORT;

                    App_ProcessInputReport();
                    App_PrepareOutputReport();
                }
            }
            break;

        case SEND_OUTPUT_REPORT: // Will be done while implementing Keyboard
            if (USBHostHID_ApiSendReport(Appl_LED_Indicator.reportID, \
                        Appl_LED_Indicator.interfaceNum, Appl_LED_Indicator.reportLength, \
                        (BYTE*) & Appl_led_report_buffer)) {
                // Host may be busy/error -- keep trying
            } else {
                App_State_Keyboard = OUTPUT_REPORT_PENDING;
            }
            USBTasks();

            break;
        case OUTPUT_REPORT_PENDING:
            if (USBHostHID_ApiTransferIsComplete(&ErrorDriver, &NumOfBytesRcvd)) {
                if (ErrorDriver) {
                    ErrorCounter++;
                    if (MAX_ERROR_COUNTER <= ErrorDriver)
                        App_State_Keyboard = ERROR_REPORTED;

                    //                                App_State_Keyboard = READY_TO_TX_RX_REPORT;
                } else {
                    ErrorCounter = 0;
                    App_State_Keyboard = READY_TO_TX_RX_REPORT;
                }
            }
            break;

        case ERROR_REPORTED:
            break;
        default:
            break;

    }//*/

}

void InitializeTimerFour( void )
{

    T4CON = 0x0;            //Stop and Init Timer
    T4CON = 0x0060;         //prescaler=1:64,
                            //internal clock
    TMR4 = 0;               //Clear timer register
    PR4 = 0x7FFF;           //Load period register

    IPC4SET = 0x00000004;   // Set priority level=1 and
    IPC4SET = 0x00000001;   // Set subpriority level=1

    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;

    T4CONSET = 0x8000;      //Start Timer

    return;
}
