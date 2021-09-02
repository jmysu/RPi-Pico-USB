/*
 * testIRQ
 */

#include <Arduino.h>

#define XPT2046_SCK 10
#define XPT2046_SDO 11
#define XPT2046_SDI 12
#define XPT2046_CS  16
#define XPT2046_IRQ 17
// These are the pins used to interface between the 2046 touch controller and Arduino Mega
#define DOUT XPT2046_SDO  /* Data out pin (T_DO) of touch screen */
#define DIN  XPT2046_SDI  /* Data in pin (T_DIN) of touch screen */
#define DCS  XPT2046_CS   /* Chip select pin (T_CS) of touch screen */
#define DCLK XPT2046_SCK  /* Clock pin (T_CLK) of touch screen */

// These are the default min and maximum values, set to 0 and 4095 to test the screen
#define HMIN 0
#define HMAX 4095
#define VMIN 0
#define VMAX 4095
#define XYSWAP 0 // 0 or 1

// This is the screen size for the raw to coordinate transformation
// width and height specified for landscape orientation
#define HRES 240 /* Default screen resulution for X axis */
#define VRES 320 /* Default screen resulution for Y axis */

#include <TFT_Touch.h>
/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DOUT, DIN);


void setupTouch()
{
  //Serial.begin(38400);
  touch.setCal(HMIN, HMAX, VMIN, VMAX, HRES, VRES, XYSWAP); // Raw xmin, xmax, ymin, ymax, width, height
  //touch.setCal(200, 3800, 400, 3900, 320, 240, 0); // Raw xmin, xmax, ymin, ymax, width, height
  touch.setRotation(1);
}


bool isTouchIRQ = false;
int x,y;

void touchIRQ()
{
    isTouchIRQ = true;
}

void setupTouchIRQ()
{   /*
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial);
    delay(500);
    Serial.println("IRQ test...");
    */

    setupTouch();
    pinMode(XPT2046_IRQ, INPUT_PULLUP);
    attachInterrupt(XPT2046_IRQ, touchIRQ, CHANGE);
}

 
void loopTouch()
{
    if (isTouchIRQ) {
        delay(50); 
        touch.Pressed();
        if (touch.Pressed()) {
            Serial.print("Down ");
            x = touch.X();
            y = touch.Y();
            Serial.print(x);
            Serial.print(", ");
            Serial.println(y);
            }
        else
            Serial.println("Rise");    

        //delay(30);    
        isTouchIRQ = false;
        }

}