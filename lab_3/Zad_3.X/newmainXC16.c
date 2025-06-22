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
#pragma config JTAGEN = OFF   

#include <xc.h>
#include "libpic30.h"
#include "adc.h"
#include "buttons.h"
 
#define ALARM_THRESHOLD 512  
 
void init(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    TRISA = 0x0000;
    BUTTON_Enable(BUTTON_S3);
}
 
void alarm(void) {
    int alarm_active = 0;
 
    while (1) {
        unsigned int adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
 
        if (!alarm_active && adc_value > ALARM_THRESHOLD) {
            alarm_active = 1;
 
            for (int i = 0; i < 25; i++) {  
                if (BUTTON_IsPressed(BUTTON_S3)) {
                    alarm_active = 0;
                    LATA = 0x00;
                    break;
                }
 
                adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
                if (adc_value < ALARM_THRESHOLD) {
                    alarm_active = 0;
                    LATA = 0x00;
                    break;
                    
                }
 
                LATA ^= 0x01; 
                __delay32(500000); 
            }
 
            if (alarm_active) {
                LATA = 0xFF;
            }
        }
 
        if (alarm_active) {
            if (adc_value < ALARM_THRESHOLD || BUTTON_IsPressed(BUTTON_S3)) {
                alarm_active = 0;
                LATA = 0x00;
            }
        }
 
        __delay32(100000); 
    }
}
 
int main(void) {
    init();
    alarm();
    return 0;
}

// Andrii Norets BOSS, to go laba