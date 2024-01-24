/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: simple HelloWorld application     */
/*                for Amazing Ball platform         */
/*                                                  */
/****************************************************/

#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>
#include <uart.h>

#include "lcd.h"
#include "joystick.h"
#include "led.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  


int main() {
    /* LCD Initialization Sequence */
    __C30_UART = 1; 
    lcd_initialize();
    lcd_clear();
    lcd_locate(0, 0);
    
    lcd_printf("Qinfeng Li\rChinKuan Lin\rChaoJen Chiu\rYifei Zhou");
    
    AD1PCFGHbits.PCFG20 = 1;
    TRISEbits.TRISE8 = 1;
    TRISDbits.TRISD10 = 1;
    
    // Initialize LED (assuming led_initialize() sets TRIS for LED)
    led_initialize();
    LED_BLINK();
    // Main loop
    while(1) {
        // Check if button is pressed
        BTN1_PRESSED();
        BTN1_RELEASED();
        BTN2_PRESSED();
        BTN2_RELEASED();
        LED_STATE();
       
        
    }
    
    
    return 0;
}


