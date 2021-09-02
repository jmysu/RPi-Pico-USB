#include <Arduino.h>
#include "st7789_lcd.pio.h"

extern "C" {
void pioLCDsetup();
void pioLCDloop(float t);
};

#include <TFT_Touch.h>
extern TFT_Touch touch;

extern bool isTouchIRQ;
extern void setupTouchIRQ();
extern void loopTouch();

void setup()
{
    pinMode(25, OUTPUT); //LED
    pinMode(13, OUTPUT); //LCD Back Light
    digitalWrite(13, HIGH);
    delay(100);
    //setup the pio state machine
    pioLCDsetup();
    setupTouchIRQ();
}

extern float theta;
int dir=1;
void loop()
{
    if (isTouchIRQ) {   //As the XPT2046 share spi pins with LCD, might need more works to figure out position reading w/ LCD state machine running
        //theta = 0.0f;
        dir = dir*-1;   //Change rotation direction
        delay(50);      //debounce
        isTouchIRQ = false;    
        } 
    
    theta += 0.05f*dir;
    pioLCDloop(theta);
}

