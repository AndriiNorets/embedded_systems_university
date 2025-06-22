 
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
 
int gray(int binary) {
    return binary ^ (binary >> 1);
}
 
int bcd(int decimal) {
    return ((decimal / 10) << 4) | (decimal % 10);
}
 
uint8_t lfsr_step(uint8_t lfsr) {
    uint8_t bit = ((lfsr >> 0) ^ (lfsr >> 1) ^ (lfsr >> 2) ^ (lfsr >> 5) ^ (lfsr >> 6)) & 1;
    return (lfsr >> 1) | (bit << 5);
}
 
void fun0() {
    int i = 0;
    while (i < 256) {
        LATA = i++;
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun1() {
    int i = 255;
    while (i >= 0) {
        LATA = i--;
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun2() {
    for (int i = 0; i < 256; ++i) {
        LATA = gray(i);
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun3() {
    for (int i = 255; i >= 0; --i) {
        LATA = gray(i);
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun4() {
    for (int i = 0; i < 100; ++i) {
        LATA = bcd(i);
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun5() {
    for (int i = 99; i >= 0; --i) {
        LATA = bcd(i);
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun6() {
    int pos = 0;
    int dir = 1;
    while (1) {
        LATA = 0b00000111 << pos;
        __delay32(1000000);
 
        if (pos == 5 && dir == 1) dir = -1;
        else if (pos == 0 && dir == -1) dir = 1;
 
        pos += dir;
 
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun7() {
    int filled = 0;
    for (int i = 0; i < 8; i++) {
        int bit = 1 << (7 - i);
        for (int j = 0; j <= i; j++) {
            LATA = filled | bit;
            __delay32(1000000);
        }
        filled |= bit;
        __delay32(2000000);
 
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
void fun8() {
    uint8_t lfsr = 0b1110011;
    __delay32(1000000);
    while (1) {
        lfsr = lfsr_step(lfsr);
        LATA = lfsr;
        __delay32(1000000);
        if (BUTTON_IsPressed(BUTTON_S3) || BUTTON_IsPressed(BUTTON_S4)) break;
    }
}
 
int main(void) {
    AD1PCFG = 0xFFFF;  // cyfrowe wej?cia
    TRISA = 0x0000;    // port A jako wyj?cie
 
    int mode = 0;
 
    while (1) {
        if (BUTTON_IsPressed(BUTTON_S3)) {
            mode = (mode + 1) % 9;
            __delay32(2000000);
        } else if (BUTTON_IsPressed(BUTTON_S4)) {
            mode = (mode == 0) ? 8 : mode - 1;
            __delay32(2000000);
        }
 
        switch (mode) {
            case 0: 
                fun0(); 
                break;
                
            case 1: 
                fun1(); 
                break;
            
            case 2: 
                fun2(); 
                break;
            
            case 3: 
                fun3(); 
                break;
            
            case 4: 
                fun4(); 
                break;
            
            case 5: 
                fun5(); 
                break;
            
            case 6: 
                fun6(); 
                break;
            
            case 7: 
                fun7(); 
                break;
            
            case 8: 
                fun8(); 
                break;
        }
    }
 
    return 0;
}
 