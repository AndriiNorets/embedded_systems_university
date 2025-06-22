// PIC24FJ128GA010 Configuration Bit Settings
// For more on Configuration Bits
// consult your device data sheet
// CONFIG2
#pragma config POSCMOD = XT // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON // Int Ext Switch Over Mode enabled
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "lcd.h"
#include "buttons.h"
#include "string.h"
 
#define DOWN_ARROW 0
#define HEART 1
 
const uint8_t char_data_heart[8] = {
    0b01010, 
    0b11111, 
    0b11111,
    0b01110, 
    0b00100, 
    0b00000,
    0b00000,
    0b00000
};
 
const uint8_t char_data_down_arrow[8] = {
    0b00100, 
    0b00100, 
    0b11111, 
    0b01110, 
    0b00100, 
    0b00000,
    0b00000,
    0b00000
};
 
void LCD_LoadCustomChars(void) {
    LCD_CreateChar(DOWN_ARROW, (uint8_t*)char_data_down_arrow);
    LCD_CreateChar(HEART, (uint8_t*)char_data_heart);
}
 
void system_init(void){
    LCD_Initialize();
    LCD_LoadCustomChars(); 
    BUTTON_Enable(BUTTON_S3); 
}
 
void runAd(void){
    const char* adText = "\x01 ELEKTROTECH \x02 - najlepsze ceny\x01! ";
    int len = strlen(adText);
 
    AD1PCFG = 0xFFFF;    
    TRISA = 0x0000;      
 
    LCD_ClearScreen();
    LCD_PutString("Reklama...", 10);
    __delay32(100000); 
    
    while (1) {
        for (int i = 0; i < len - 15; i++) {
            LCD_ClearScreen();
 
            for (int j = 0; j < 16; j++) {
                LCD_PutChar(adText[i + j]);
            }
 
            LATA = 1 << (i % 8);
 
            __delay32(1200000);  
 
            if (BUTTON_IsPressed(BUTTON_S3)) {
                LCD_ClearScreen();
                LCD_PutString("Koniec reklamy", 14);
                LATA = 0x0000;
                __delay32(1000000);
                return;
            }
        }
    }
}
 
int main(void) {
    system_init();
    runAd(); 
    while (1); 
    return 0;
}

// Andrii Norets kozak, to go laba