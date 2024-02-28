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

    lcd_printf("Failed: \n");
    uart2_init(9600);
    //set_timer1(500);

    uint8_t start_byte, msg, m1, m2;
    uint8_t mlen = 0;
        //char len;
    uint8_t crc_1;
    uint8_t crc_2;


    while (1) {
        //int8_t rec_sb = uart2_recv(&start_bit);
        //int8_t rec_crc1 = uart2_recv(&crc_1);
        //int8_t rec_crc2 = uart2_recv(&crc_2);
        lcd_clear();
        while (uart2_recv(&start_byte) != 0);
        while (uart2_recv(&crc_1) != 0);
        while (uart2_recv(&crc_2) != 0);
        while (uart2_recv(&mlen) != 0);
        //while (uart2_recv(&m1) != 0);
        //while (uart2_recv(&m2) != 0);

        lcd_printf_d("--c1 value %u\n\r", crc_1);
        lcd_printf_d("--c2 value %u\n\r", crc_2);
        lcd_printf_d("--len %x\n\r", mlen);
        //lcd_printf_d("--m1 value %u\n\r", m1);
        //lcd_printf_d("--m2 value %u\n\r", m2);
        uint16_t sent_crc = ((crc_1 << 8) & 0xFF00) | crc_2;
        
//        while(1);
        
        int i;
        char * buffer = malloc(mlen + 1);
        uint16_t calc_crc = 0;

        for (i = 0; i < mlen; i++) {
            //uint16_t b = uart2_recv(&msg);
            uint8_t ba;
            uart2_recv(&ba);
//            if (b != 0) {
//                break;
//            }
            //buffer[i] = b;
            //buffer[i]=uart2_recv(&msg);
            
            buffer[i]=ba;
            lcd_printf_d("--ba: %u\n\r", ba);
            //buffer[i]=msg;
            lcd_printf_d("--char: %u\n\r", buffer[i]);
            calc_crc = crc_update(sent_crc, ba);
            
            
        }
        
        //lcd_printf("%s\r",buffer);
//                for (i = 0; i < mlen; i++) {
//            //uint16_t b = uart2_recv(&msg);
//            //uint8_t b = uart2_recv(&msg);
////            if (b != 0) {
////                break;
////            }
//            //buffer[i] = b;
//            //buffer[i]=uart2_recv(&msg);
//            uart2_recv(&buffer[i]);
//            //buffer[i]=msg;
//            lcd_printf_d("--char: %u\n\r", buffer[i]);
//            calc_crc = crc_update(sent_crc, buffer[i]);
//            
//            
//        }
        //T1CONbits.TON = 0;
        //buffer[i] = '\0';
        
        //////////////////////////////////////////////
        
        lcd_printf_d("sent_crc %x\n\r", sent_crc);
        lcd_printf_d("calc_crc %x\n\r", calc_crc);
        while(1);
        
        if (calc_crc != sent_crc) {
            free(buffer);
            attempt++;
            uart2_send_8(0);
        } else {
            lcd_clear();
            lcd_locate(0, 0);
            lcd_printf("Failed: %d\n", attempt);
            lcd_printf("%s\n", buffer);
            lcd_printf_d("start bit %u\n", start_byte);
            lcd_printf_d("--c1 value %u\n", crc_1);
            lcd_printf_d("--c2 value %u\n", crc_2);
            lcd_printf_d("--len value %u\n", mlen);
            free(buffer);
            attempt = 0;
            uart2_send_8(1);
        }

        while (1) {
        }
        //        while(uart2_recv(&crc_1)!=0 && timeout == 0);


        //        if (rec_sb != 0){
        //            attempt++;
        //            uart2_send_8(0);
        //            continue;
        //        }


        //        lcd_printf_d("start bit %u\n",start_bit);
        //        lcd_printf_d("c1 value %u\n, c2 %u\n",crc_1,crc_2);
        //        uint16_t sent_crc = ((crc_1 << 8) & 0xFF00) | crc_2;
        uart2_send_8(1);

        //if (c == 0xFF00){
        //    continue;
        //}
        //            uint16_t crc1 = uart2_recv();
        //            uint16_t crc2 = uart2_recv();
        //            uint16_t sent_crc = ((crc1 << 8) & 0xFF00) | crc2;
        //            
        //            lcd_printf_d("c1 value %u\n, c2 %u\n",crc1,crc2);
        //            if (crc1 == 0xFF00 || crc2==0xFF00){
        //                attempt++;
        //                uart2_send_8(0);
        //                continue;
        //            }


        //            uint16_t len = uart2_recv(msg);
        //            if (len == 0xFF00){
        //                attempt++;
        //                uart2_send_8(0);
        //                continue;
        //            }
        //        
        //            int i;
        //            char * buffer = malloc(len+1);
        //            uint16_t calc_crc = 0;
        //            int asdf = 0;
        //            for(i = 0; i<len;i++){
        //                uint16_t b = uart2_recv(msg);
        //                if (b == 1){
        //                    asdf = 1;
        //                    break;
        //                }
        //                buffer[i] = b;
        //                calc_crc = crc_update(calc_crc, buffer[i]);
        //            }
        //            T1CONbits.TON = 0;
        //            buffer[i] = '\0';
        //            
        //            if (calc_crc != sent_crc){
        //                free(buffer);
        //                attempt++;
        //                uart2_send_8(0);
        //            }else{
        //                lcd_clear();
        //                lcd_locate(0,0);
        //                lcd_printf("Failed: %d\n", attempt);
        //                lcd_printf("%s\n",buffer);
        //                free(buffer);
        //                attempt = 0;
        //                uart2_send_8(1);
        //            }

        //}
    }
}
