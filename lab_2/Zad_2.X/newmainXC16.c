 
// PIC24FJ128GA010 Configuration Bit Settings
 
// 'C' source line config statements
 
// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)
 
// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
 
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
 
#include <xc.h>
#include "libpic30.h"
#include "adc.h"
#include "buttons.h"
 
char modeState = 0;
 
int convertToGray(int value) {
    return value ^ (value >> 1);
}
 
void systemInit(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000;
    BUTTON_Enable(BUTTON_S3);
    BUTTON_Enable(BUTTON_S4);
}
 
unsigned long calculateDelayFromADC() {
    unsigned long potVal = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
 
    if (potVal < 205) return 40000;
    else if (potVal < 410) return 80000;
    else if (potVal < 615) return 120000;
    else if (potVal < 820) return 160000;
    else return 200000;
}
 
void updateModeBasedOnInput() {
    if (BUTTON_IsPressed(BUTTON_S3)) {
        modeState = (modeState + 1) % 2;
        __delay32(2000000);
    }
 
    if (BUTTON_IsPressed(BUTTON_S4)) {
        modeState = (modeState == 0) ? 1 : 0;
        __delay32(2000000);
    }
}
 
void modeDescending(void) {
    for (int val = 255; val >= 0; val--) {
        LATA = val;
        unsigned long waitTime = calculateDelayFromADC();
 
        for (int rep = 0; rep < 100; rep++) {
            __delay32(waitTime / 10);
            updateModeBasedOnInput();
            if (modeState != 0) return;
        }
    }
}
 
void modeChase(unsigned long baseDelay) {
    int ledIndex = 0;
    int direction = 1;
 
    while (1) {
        LATA = (7 << ledIndex);
        __delay32(baseDelay);
 
        if ((direction == 1 && ledIndex >= 5) || (direction == -1 && ledIndex <= 0)) {
            direction *= -1;
        }
 
        ledIndex += direction;
 
        updateModeBasedOnInput();
        if (modeState != 1) break;
 
        baseDelay = calculateDelayFromADC();
    }
}
 
int main(void) {
    systemInit();
 
    while (1) {
        unsigned long currentDelay = calculateDelayFromADC();
 
        if (modeState == 0) {
            modeDescending();
        } else if (modeState == 1) {
            modeChase(currentDelay);
        }
    }
}
 