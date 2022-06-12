#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "i2c_master_noint.h"
#include "ssd1306.h"
#include "font.h"

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
    
    //Set Up I2C
    i2c_master_setup();
    //Set Up SSD1036
    ssd1306_setup();
    
    long delay = 1200000;
    char first_line[] = "Ryan King-Shepard";
    char second_line[] = "Adv Mechatronics";
    char thrid_line[25];
    char fourth_line[25];
    unsigned int count_var = 0;
    // Configure PIC Ports
    TRISAbits.TRISA4 = 0;
    __builtin_enable_interrupts();
    
    while(1){
        drawString(0x00, 0x00, first_line);
        drawString(0x00, 0x08, second_line);
        sprintf(thrid_line, "Counter Var: %d", count_var);
        count_var++;
        drawString(0x00, 0x10, thrid_line);
        _CP0_SET_COUNT(0);
        ssd1306_update();
        unsigned long time = _CP0_GET_COUNT();
        double fps = 48000000 / time;
        sprintf(fourth_line, "FPS = %.2f", fps);
        drawString(0x00, 0x18, fourth_line);
        
        //Heartbeat
        LATAbits.LATA4 = 1;
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < delay) {}
        LATAbits.LATA4 = 0;
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < delay) {}
        if (count_var == 50000){
            count_var = 0;
        }
    }

}