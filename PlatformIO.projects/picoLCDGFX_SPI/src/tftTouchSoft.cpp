/* FILE:    TFT_Touch_Example1
   DATE:    10/06/14
   VERSION: 0.1
   ORIGINAL AUTHOR:  Andrew Davies

This is an example of how to use the TFT_Touch library. It does not display
anything on the screen!

Open up the Serial Monitor Window from the IDE and set to 38400 baud to see the
information from screen contacts

By using a stylus at the screen extremes or touch zone limits this sketch can be
used to check tha calibration values.

IMPORTANT: The 2.4" screen orientation used for calibration is with the SD Card slot on the left!

The library provides the ability to read the current status of the touch screen.
It does not provide any graphics drawing functions for controlling the TFT screen itself
it is intended for providing touch screen support to any graphics sketches and libraries.
However the library will work standalone as this example sketch will demonstrate.

The example will read the current state of the touch screen and output it to the serial
port as raw X and Y coordinates.

No information will be displayed on the TFT screen itself whilst this example is running.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.

Adapted by Rowboteer 13/5/15
*/
#include <Arduino.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#define SPI_CLK_PIN	    10
#define SPI_MOSI_PIN	11
#define SPI_MISO_PIN	12
#define TP2046_CS_PIN	16
#define TP2046_IRQ_PIN	17
// These are the pins used to interface between the 2046 touch controller and Arduino Mega
#define DOUT SPI_MOSI_PIN  /* Data out pin (T_DO) of touch screen */
#define DIN  SPI_MISO_PIN  /* Data in pin (T_DIN) of touch screen */
#define DCS  TP2046_CS_PIN /* Chip select pin (T_CS) of touch screen */
#define DCLK SPI_CLK_PIN   /* Clock pin (T_CLK) of touch screen */


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

#include "TFT_Touch.h"
 
/* Create an instance of the touch screen library */
TFT_Touch touch = TFT_Touch(DCS, DCLK, DOUT, DIN);


void setupTouch()
{
  //Serial.begin(38400);
  touch.setCal(HMIN, HMAX, VMIN, VMAX, HRES, VRES, XYSWAP); // Raw xmin, xmax, ymin, ymax, width, height
  //touch.setCal(200, 3800, 400, 3900, 320, 240, 0); // Raw xmin, xmax, ymin, ymax, width, height
  touch.setRotation(1);
}

extern Adafruit_ST7789 tft;
extern bool isTouched;
extern void checkColorChange();
extern int lastX, lastY, lastColor;

/* Main program */
void loopTouchRaw()
{
  unsigned int X_Raw;
  unsigned int Y_Raw;
  int X_Coord;
  int Y_Coord;

  // Check if the touch screen is currently pressed
  // Raw and coordinate values are stored within library at this instant
  // for later retrieval by GetRaw and GetCoord functions.

  if (touch.Pressed()) // Note this function updates coordinates stored within library variables
  {
    /* Check if the touch screen is currently pressed*/
    // Raw and coordinate values are stored within library at this instant
    // for later retrieval by GetRaw and GetCoord functions.
    // This avoids getting duff values returned
 
    /* Read the current X and Y axis as raw co-ordinates at the last touch time*/
    // The values returned were captured when Pressed() was called!
    
    X_Raw = touch.RawX();
    Y_Raw = touch.RawY();
    //map raw to coordinate
    //X_Coord = touch.X();
    X_Coord = map(touch.RawX(), 200, 3800, 0,240); //mapping rawX to 0~240
    //Y_Coord = touch.Y();
    Y_Coord = map(touch.RawY(), 400, 3900, 0,320); //mapping rawY to 0~320
    
    if (Y_Coord <40) {
      lastX = X_Coord;
      checkColorChange();
      }
    else  {  
      /* Output the results to the serial port */
      Serial.print("Raw x,y = ");
      Serial.print(X_Raw);
      Serial.print(",");
      Serial.print(Y_Raw);
      Serial.print("\t => ");
      Serial.print(X_Coord);
      Serial.print(",");
      Serial.println(Y_Coord);
    
      tft.fillCircle(X_Coord, Y_Coord, 3, lastColor);
      }
    //delay(100);//debounce
    isTouched = true;
  }
  else 
    isTouched = false;

}

