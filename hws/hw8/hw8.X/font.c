#include "font.h"

#define OFFSET 32
#define X_LIMIT 122
#define Y_LIMIT 26
#define LET_WIDTH 5
#define LET_HEIGHT 8
#define SPACE 1

void drawChar(unsigned char x, unsigned char y, unsigned char letter) {
    //Check that X and Y are in range. If not do nothing
    if ((x > X_LIMIT) || (y > Y_LIMIT)) {
        return;
    }    
    else {
        // X and Y are in range; lets make a letter
        unsigned char idx = letter - OFFSET;
        const char* symbol = ASCII[idx];
        for( unsigned char i = 0; i < LET_WIDTH; i++ ) {
            unsigned char col = symbol[i];
            
            for ( unsigned char j = 0; j < LET_HEIGHT; j++ ) {
                unsigned char bitflag = 1 << j;
                unsigned char powerbit = (col & bitflag) >> j;
                ssd1306_drawPixel(x+i, y+j, powerbit);
            }
        }
    }
}

void drawString(unsigned char x, unsigned char y, char* message) {
    //Check that X and Y are in range. If not do nothing
    if ((x > X_LIMIT) || (y > Y_LIMIT)) {
        return;
    } 
    else { 
        unsigned char i = 0;
        while (message[i] != 0x00){
            drawChar(x, y, message[i]);
            x = x + LET_WIDTH + SPACE; // Letter length plus space
            if (x > X_LIMIT){
                x = 0;
                y = y + LET_HEIGHT;
            }
            if (y > Y_LIMIT) {
                return; // Hit the end of the screen
            }
            i++;
        }
    }
}
