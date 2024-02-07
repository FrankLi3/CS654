/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file to implement basic  */
/*                macros and functions to interface */
/*                with the analog joystick.         */
/*                                                  */
/****************************************************/

#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include <p33Fxxxx.h>
#include "types.h"

#define DEBOUNCE_TIME 200

int buttonWasPressed = 0;
uint8_t COUNT = 0;


#define BTN1_PRESSED() PORTEbits.RE8 == 0
    
          
#define BTN1_RELEASED() PORTEbits.RE8 == 1
        

#define BTN2_PRESSED() RTDbits.RD10 == 0

#define BTN2_RELEASED() PORTDbits.RD10 == 1

#define LED_STATE()\
    if(PORTEbits.RE8 ^ PORTDbits.RD10){\
        SETLED(LED3_PORT);\
    }else{\
        CLEARLED(LED3_PORT);\
    }

#define LED_BLINK()\
    int i;\
    for (i=1; i<4;i++){\
        SETLED(LED4_PORT);\
        __delay_ms(10);\
        CLEARLED(LED4_PORT);\
        __delay_ms(1000);\
    }
    

    
#endif /* __JOYSTICK_H */


