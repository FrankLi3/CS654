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
    //lcd_clear();
    lcd_locate(0, 0);

    lcd_printf("Failed: \n");
    uart2_init(9600);
    set_timer1(50000);

    uint8_t start_byte, msg, m1, m2;
    uint8_t mlen = 0;
    //char len;
    uint8_t crc_1;
    uint8_t crc_2;


    while (1) {

        while (uart2_recv(&start_byte) != 0 );
        
        
        while (uart2_recv(&crc_1) != 0  );
        
        while (uart2_recv(&crc_2) != 0 );
        
        while (uart2_recv(&mlen) != 0 );
        



        lcd_printf_d("--len %x\n\r", mlen);
        lcd_clear();

        uint16_t sent_crc = ((crc_1 << 8) & 0xFF00) | crc_2;



        int i;

        uint8_t buffer[mlen + 1];

        uint16_t calc_crc = 0;
        free(buffer);
        for (i = 0; i < mlen; i++) {
            while (uart2_recv(&msg) != 0 );
 
            buffer[i] = msg;

            calc_crc = crc_update(calc_crc, buffer[i]);


        }

        buffer[i] = '\0';

        if (calc_crc != sent_crc) {
            free(buffer);
            attempt++;
            uart2_send_8(0);
        } else {
            free(buffer);
            lcd_clear();
            lcd_locate(0, 0);
            lcd_printf("Failed: %d\n\r", attempt);
lcd_printf_d("calc_crc %x\n\r", calc_crc);

            attempt = 0;
            uart2_send_8(1);
            break;
        }

        
    }
}
