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
#include "led.h"
#include "lcd.h"
#include "joystick.h"
#include "types.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

uint16_t cnt = 0;
uint8_t second = 0;
uint8_t minute = 0;
uint16_t ms = 0;


void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt (void)
{
    LED2_PORT ^= 1;
    CLEARBIT(IFS0bits.T1IF);
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt (void)
{
    LED1_PORT ^= 1;
    CLEARBIT(IFS0bits.T2IF);
    ms++;
    if (ms>999) {
        ms = 0;
        second++;
    }
    if (second > 59) {
        lcd_clear();
        minute++;
        second = 0;
    }
}

/*------------------- Joystick external interrupt ----------------------*/
void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt (void)
{
    minute = 0;
    second = 0;
    ms = 0;
    _INT1IF = 0;
}

int main(){
	/* LCD Initialization Sequence */
	__C30_UART=1;	
	lcd_initialize();
    led_initialize();
    lcd_clear();
	lcd_locate(0,0);
	
    
    AD1PCFGHbits.PCFG20 = 1;
    TRISEbits.TRISE8 = 1;
    TRISDbits.TRISD10 = 1;
    
    /*------------------- Setting Joystick external interrupt ----------------------*/
    IEC1bits.INT1IE = 1;    //enable bit
    IPC5bits.INT1IP = 5;    //priority
    INTCON2bits.INT1EP = 1; //external interrupt 1 edge detect
    
    /*------------------- Setting Timer 2 ----------------------*/
    /* Initialize Timer in Normal mode, internal clock*/
    CLEARBIT(T2CONbits.TON);
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);   
    
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
    
    
    /*------------------- Setting Timer 1 ----------------------*/
    /* Select external clock source */
    T1CONbits.TCS = 1; 
    
    __builtin_write_OSCCONL(OSCCONL | 2); 
    
    /* Reset timer counter*/ 
    TMR1 = 0x00;
    
    /* Select 1:8 prescaler*/
    T1CONbits.TCKPS =0b01;
    
    /* Turn off synchronization */
    T1CONbits.TSYNC = 0;    
    
    /* Select reload period*/
    PR1 = 4096;
    
    /* Select Interrupt Priority 1*/ 
    IPC0bits.T1IP = 0x02;   
    
    /* Clear Pending Flag, Enable Interrupt 0*/  
    CLEARBIT(IFS0bits.T1IF);
    SETBIT(IEC0bits.T1IE);
   
    /* Turn on TImer 2*/ 
    SETBIT(T1CONbits.TON);
    
   
    /*------------------- Setting Timer 3 ----------------------*/
   
    /* Use internal clock */
    T3CONbits.TCS = 0;  
    
    /* Disable gated time accumulation */
    T3CONbits.TGATE = 0; 
    
    /* Prescaler value (adjust as needed) */
    T3CONbits.TCKPS = 0b00; 
    
    /* Set period for Timer 3 */
    PR3 = 65535; 
    
    /* Reset Timer 3 counter */
    TMR3 = 0x00;  
    
    /* Start Timer */
    SETBIT(T3CONbits.TON); 
    
    
	while(1){
        TMR3 = 0x00;
        
        SR |= 0b01100000;
        LED4_PORT ^= 1;
        SR &= 0b00011111;
        
        cnt++;
        if (cnt > 25000) {
            cnt=0;
            lcd_locate(0,0);
            lcd_printf("Time: %02d:%02d:%03d\r", minute, second, ms);
            double elapsedMs = (double)TMR3 * 1000 / FCY;
            lcd_printf("TMR3: %.4f ms", elapsedMs);
        }
        
        
        //lcd_locate(5,5);
        
    }
    return 0;
}

