//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include <p33Fxxxx.h>
#include "types.h"
#include "uart.h"
#include "crc16.h"
#include "lab3.h"
#include "lcd.h"
#include "timer.h"

// Primary (XT, HS, EC) Oscillator without PLL
_FOSCSEL(FNOSC_PRIPLL);
// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystanl
_FOSC(OSCIOFNC_ON & POSCMD_XT);
// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);
// Disable Code Protection
_FGS(GCP_OFF);

int attempt = 0;
int timeout = 0;

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    CLEARBIT(IFS0bits.T2IF);
    timeout = 1;
}

void main() {

    __C30_UART = 1;
    lcd_initialize();
    lcd_clear();
    lcd_locate(0, 0);

    lcd_printf("Attempt: \n\r");
    lcd_clear();
    uart2_init(9600);
    set_timer2(50000);
    
    uint8_t start_byte, msgg;
    char c;
    uint8_t mlen = 0;
    uint8_t crc_1;
    uint8_t crc_2;


    while (1) {
        timeout = 0;
        char message_buffer[256];
        
        int i,k;
        
        uint16_t calc_crc = 0;
        while (uart2_recv(&start_byte) != 0); 
        //lcd_printf("sb: %d\n\r", start_byte);

        //timer on
        SETBIT(T2CONbits.TON);
        
        while(start_byte!=0){
            while (uart2_recv(&start_byte) != 0 && timeout==0){}

            if(timeout==1){
                break;
        }  
        }
        
//         if(timeout==1){
//            lcd_printf("timeout");
//        }
//        if(timeout==0){
//            lcd_printf("time not out");
//        }

        
        while (uart2_recv(&crc_1) != 0  && timeout==0);
        
        while (uart2_recv(&crc_2) != 0 && timeout==0);
        
        while (uart2_recv(&mlen) != 0 && timeout==0);
     
        
        for (i = 0; i < mlen; i++) {
            while (uart2_recv(&msgg) != 0 && timeout==0);
            message_buffer[i] = msgg; 
            calc_crc = crc_update(calc_crc, message_buffer[i]);
        }
        
          
        uint16_t sent_crc = ((crc_1 << 8) & 0xFF00) | crc_2;
        
        //timer off
        CLEARBIT(T2CONbits.TON);
        
        if (calc_crc != sent_crc || timeout==1) {     
            lcd_clear();
            lcd_locate(0, 0);
            lcd_printf("Failed Attempt: %d\n\r", attempt);
            lcd_printf_d("crc: %x\n\r", calc_crc);
            attempt++;
            
            for(k = 0; k<256; k++){
                message_buffer[k] = '\0';
            }
            uart2_send_8(0);
            
        } else {
            
            lcd_clear();
            lcd_locate(0, 0);
            lcd_printf("Failed Attempt: %d\n\r", attempt);
            lcd_printf_d("crc: %x\n\r", calc_crc);
            lcd_printf_d("%s\n\r", message_buffer);
            
            attempt = 0;
            for(k = 0; k<256; k++){
            message_buffer[k] = '\0';
        }
            uart2_send_8(1);
//            break;
        }

        
    }
}
