
#include <Arduino.h>

#include "PluggableUSBHID.h"
#include "USBMouse.h"


//USBKeyboard Keyboard;
USBMouse mouse;

void setup()
{

}

int16_t x = 0;
int16_t y = 0;
int32_t radius = 10;
int32_t angle = 0;

void loop()
{
   for (int i=0;i<100;i++) {
      mouse.move(1,0);
      delay(100);
      }
   for (int i=0;i<100;i++) {
      mouse.move(-1,0);
      delay(100);
      }
      
   //will cause mouse to move in a circle
   for (angle=0; angle<=360; angle+=3) {
      x = cos((double)angle * 3.14 / 180.0) * radius;
      y = sin((double)angle * 3.14 / 180.0) * radius;
      mouse.move(x,y);
      delay(10);
      }

}