#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "spi.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use fast frc oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // primary osc disabled
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt value
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz fast rc internal oscillator
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

int main() {
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    //Set Up SPI
    initSPI();
    long delay = 600000;
    unsigned char tri_wave[] = {0, 13, 26, 39, 51, 
            65, 77, 89, 102, 115,
            128, 140, 153, 166, 179, 
            191, 204, 217, 230, 242, 
            255, 242, 230, 217, 204, 
            191, 179, 166, 153, 140,
            128, 115, 102, 89, 77, 
            65, 51, 39, 26, 13};
    unsigned char tri_idx = 0;
    unsigned char sin_wave[] = {128, 169, 205, 233, 251,
            255, 248, 227, 196, 159,
            118, 78, 43, 17, 3,
            1, 12, 36, 69, 107, 
            149, 187, 220, 244, 255,
            253, 239, 213, 178, 138,
            97, 60, 29, 8, 0, 
            5, 23, 51, 87, 128};
    unsigned char sin_idx = 0;
    __builtin_enable_interrupts();
    
    
    while(1){
        // write bytes over SPI
        //Write bytes to ref A
        LATAbits.LATA0 = 0; // Bring Chip select low
        //Write bytes to ref A
        unsigned char upper_tri = (tri_wave[tri_idx] >> 4);
        unsigned char A_byte_two = (tri_wave[tri_idx] << 4);
        unsigned char A_byte_one =  0b01110000;
        A_byte_one = A_byte_one | upper_tri;
        
        spi_io(A_byte_one);
        spi_io(A_byte_two);
        
        LATAbits.LATA0 = 1;
        
        //Write bytes to ref B
        LATAbits.LATA0 = 0;
        
        unsigned char upper_sin = (sin_wave[sin_idx] >> 4);
        unsigned char B_byte_two = (sin_wave[sin_idx] << 4);
        unsigned char B_byte_one = 0b11110000;
        B_byte_one = B_byte_one | upper_sin;
        
        spi_io(B_byte_one);
        spi_io(B_byte_two);
        
        LATAbits.LATA0 = 1; // Bring Chip select high
        
        tri_idx++;
        if (tri_idx == 40) {
            tri_idx = 0;
        }
        
        sin_idx++;
        if (sin_idx == 20) {
            sin_idx = 0;
        }
        
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < delay) {
        
        }
    }

}