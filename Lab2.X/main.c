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

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

uint8_t cnt =0;


void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt (void)
{
    PORTAbits.RA5 ^= 1;
    CLEARBIT(IFS0bits.T1IF);
}

void __attribute__((__interrupt__,no_auto_psv)) _T2Interrupt (void)
{
    PORTAbits.RA4 ^= 1;
    CLEARBIT(IFS0bits.T2IF);
}

int main(){
	/* LCD Initialization Sequence */
	__C30_UART=1;	
	lcd_initialize();
    
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
   
    CLEARBIT(T2CONbits.TON);
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);   
    
    //TIMER 2
    /* Reset timer counter*/ 
    TMR2 = 0x00;   
    
    /* Select 1:256 prescaler*/
    T2CONbits.TCKPS =0b11;  

    /* Select reload period*/
    PR2 = 50;    
    
    /* Select Interrupt Priority 1*/   
    IPC1bits.T2IP = 0x01;    
    
    /* Clear Pending Flag, Enable Interrupt 0*/    
    CLEARBIT(IFS0bits.T2IF);   
    SETBIT(IEC0bits.T2IE);
    
    /* Turn on TImer 2*/   
    SETBIT(T2CONbits.TON);
    
    
    /* Initialize Timer in Normal mode, internal clock*/
    CLEARBIT(T1CONbits.TON);
    CLEARBIT(T1CONbits.TGATE);
    
    T1CONbits.TCS = 1; // Select external clock source
    
    __builtin_write_OSCCONL(OSCCONL | 2); 
    TMR1 = 0x00;
    /* Select 1:8 prescaler*/
    T1CONbits.TCKPS =0b01;
    /* Turn off synchronization */
    T1CONbits.TSYNC = 0;    
    PR1 = 4096;
    IPC0bits.T1IP = 0x02;   
    CLEARBIT(IFS0bits.T1IF);
    SETBIT(IEC0bits.T1IE);
    SETBIT(T1CONbits.TON);
    
    
   
    T3CONbits.TCS = 0;  // Use internal clock
    T3CONbits.TGATE = 0; // Disable gated time accumulation
    T3CONbits.TCKPS = 0b00; // Prescaler value (adjust as needed)
    PR3 = 25000; // Set period for Timer 3
    TMR3 = 0x00;  // Reset Timer 3 counter
    SETBIT(T3CONbits.TON);  // Start Timer 3
	while(1){
        if (TMR3 >= 25) {
                lcd_clear();
                lcd_locate(0,0);
                lcd_printf("Count %d", cnt);
                TMR3 = 0;  // Reset Timer 3 counter
                ++cnt;  // Increment count
    }
    
    return 0;
}


