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

void main() {
    
    __C30_UART = 1; 
    lcd_initialize();
    lcd_clear();
    lcd_locate(0, 0);
    
    lcd_printf("Failed: \n");
    uart2_init(9600);
        
    while(1) {
        uint16_t c = uart2_recv();
        if (c == 0xFF00){
            continue;
        }else{
            uint16_t crc1 = uart2_recv();
            uint16_t crc2 = uart2_recv();
            uint16_t sent_crc = ((crc1 << 8) & 0xFF00) | crc2;
            
            lcd_clear();
            lcd_printf("%u\n", sent_crc);
            
            
            if (crc1 == 0xFF00 || crc2==0xFF00){
                attempt++;
                uart2_send_8(0);
                continue;
            }
            
            uint16_t len = uart2_recv();
            if (len == 0xFF00){
                attempt++;
                uart2_send_8(0);
                continue;
            }
        
            int i;
            char * buffer = malloc(len+1);
            uint16_t calc_crc = 0;
            int asdf = 0;
            for(i = 0; i<len;i++){
                uint16_t b = uart2_recv();
                if (b == 0xFF00){
                    asdf = 1;
                    break;
                }
                buffer[i] = b;
                calc_crc = crc_update(calc_crc, buffer[i]);
            }
            T1CONbits.TON = 0;
            buffer[i] = '\0';
            
            if (calc_crc != sent_crc){
                free(buffer);
                attempt++;
                uart2_send_8(0);
            }else{
                lcd_clear();
                lcd_locate(0,0);
                lcd_printf("Failed: %d\n", attempt);
                lcd_printf("%s\n",buffer);
                free(buffer);
                attempt = 0;
                uart2_send_8(1);
            }
            
    }
}
}
