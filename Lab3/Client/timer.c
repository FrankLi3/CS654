/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 timer initialization func.   */
/*                                                  */
/****************************************************/


#include "timer.h"

//period in ms
void set_timer1(uint16_t period){
	/* Implement me please. */
        /* Initialize Timer in Normal mode, internal clock*/
    CLEARBIT(T2CONbits.TON);
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);   
    
    /* Reset timer counter*/ 
    TMR2 = 0x00;   
    
    /* Select 1:256 prescaler*/
    T2CONbits.TCKPS =0b11;  

    /* Select reload period*/
    PR2 = period;    

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
    
}

