/*
 *      picoLCD7789TouchSPI.cpp
 *
 *      WaveShare Pico-ResTouch2.8" ST7789 test, w/ links2004/XPT2046 touch control.
 * 
 *      Both LCD/Touch use hardware SPI.
 * 
 *      Dependancy:     
 *              lib_deps = 
 *	                Wire
 *	                SPI
 *	                adafruit/Adafruit BusIO
 *	                adafruit/Adafruit GFX Library
 *	                adafruit/Adafruit ST7735 and ST7789 Library
 *	                Links2004/XPT2046
 */
#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define SerialDebugging true

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

// For 1.44" and 1.8" TFT with ST7735 (including HalloWing) use:
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//#define SPI_INTERFACES_COUNT 2
// 2.8" TFT with Waveshare Touch2.8" ST7789:
#define TFT_CS    9
#define TFT_RST  15 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC    8
#define TFT_BL   13

#define SPI_MOSI 11
#define SPI_MISO 12
#define SPI_SCK  10
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out
//#define TFT_MOSI       PICO_MOSI // Data out
//#define TFT_SCLK       PICO_SCK  // Clock out
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
/*
//https://www.waveshare.com/pico-restouch-lcd-2.8.htm
#define LCD28_RST_PIN	15	
#define LCD28_DC_PIN	 8
#define LCD28_CS_PIN	 9
#define LCD28_CLK_PIN	10
#define LCD28_BKL_PIN	13
#define LCD28_MOSI_PIN	11
#define LCD28_MISO_PIN	12
#define TP2046_CS_PIN	16
#define TP2046_IRQ_PIN	17
//Adafruit_ST7789 tft = Adafruit_ST7789(LCD28_CS_PIN, LCD28_DC_PIN, LCD28_MOSI_PIN, LCD28_CLK_PIN, LCD28_RST_PIN);
*/
// color definitions
const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;


// The colors we actually want to use
uint16_t        Display_Text_Color         = Display_Color_Yellow;
uint16_t        Display_Backround_Color    = Display_Color_Blue;

// assume the display is off until configured in setup()
bool            isDisplayVisible        = false;

// declare size of working string buffers. Basic strlen("d hh:mm:ss") = 10
const size_t    MaxString               = 16;

// the string being displayed on the SSD1331 (initially empty)
char oldTimeString[MaxString]           = { 0 };

#include "XPT2046.h"
// connect the SPI pins (MISO, MOSI, SCK) to the hardware SPI
#define TOUCH_IRQ 17
#define TOUCH_CS  16
XPT2046 touchXPT = XPT2046(TOUCH_CS, TOUCH_IRQ);
 
int lastX, lastY, lastColor=ST77XX_BLACK;
volatile bool   isTouched = false;
void touchXPTevent(bool press, uint16_t x, uint16_t y, uint16_t z) {
    Serial.print("[Touch] ");
    Serial.print(press);
    Serial.print(" X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" Z: ");
    Serial.print(z);
    if (press) {
        lastX = y; lastY = x; //swap x/y to compatiple will old(soft) version
        isTouched = true;
        }

    if (1) { //Turn on raw prints for calibration
        //print raw readings
        touchXPT.readRaw(&x, &y, &z);
        Serial.print("\t[RAW]");
        Serial.print(" X: ");
        Serial.print(x);
        Serial.print(" Y: ");
        Serial.print(y);
        Serial.print(" Z: ");
        Serial.print(z);
        }
    Serial.println();    
}




#define BOXSIZE 40
void fillColorS() {
    //draw 6 color blocks
    tft.fillRect(0,         0, BOXSIZE, BOXSIZE*3/4, ST77XX_RED);
    tft.fillRect(BOXSIZE,   0, BOXSIZE, BOXSIZE*3/4, ST77XX_YELLOW);
    tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE*3/4, ST77XX_GREEN);
    tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE*3/4, ST77XX_CYAN);
    tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE*3/4, ST77XX_MAGENTA);
    tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE*3/4, ST77XX_BLUE);
    //
    tft.fillRect(0, 28, 240, 4, ST77XX_BLACK);
}
void checkColorChange()
{
    int c = 1+(lastX/BOXSIZE);
    //int c = 1+(lastY/BOXSIZE);
    
    Serial.print("C=");
    Serial.println(c);

    switch (c) {
      default:
      case 1: lastColor=ST77XX_RED;
              break;
      case 2: lastColor=ST77XX_YELLOW;
              break;
      case 3: lastColor=ST77XX_GREEN;
              break;
      case 4: lastColor=ST77XX_CYAN;
              break;
      case 5: lastColor=ST77XX_MAGENTA;
              break;
      case 6: lastColor=ST77XX_RED;  //Reset 
              tft.fillScreen(ST77XX_BLUE);
              tft.drawLine(0,     0, 240-1,  320-1, ST77XX_WHITE);
              tft.drawLine(0, 320-1, 240-1,      0, ST77XX_WHITE);

              tft.drawCircle(30,        40,10, ST77XX_WHITE);
              tft.drawCircle(30,    320-40,10, ST77XX_WHITE);
              tft.drawCircle(240-30,    40,10, ST77XX_WHITE);
              tft.drawCircle(240-30,320-40,10, ST77XX_WHITE);
              c=1;
              break;       
      }
    fillColorS();  
    tft.drawCircle(BOXSIZE*c-20, 15, 4, ST77XX_BLACK);
    tft.drawCircle(BOXSIZE*c-20, 15, 5, ST77XX_BLACK);
}

//void setupTouch();
//void loopTouchRaw();

void setup() {

    #if (SerialDebugging)
    Serial.begin(115200); while (!Serial); Serial.println();
    delay(500);
    Serial.println("Start LCD7789/Touch test...");
    #endif

    //Setup SPI pin, Hardware SPI only
    SPI = MbedSPI(SPI_MISO, SPI_MOSI, SPI_SCK);
    
    // settling time
    delay(250);

    // ignore any power-on-reboot garbage
    isTouched = false;
 
      // Use this initializer if using a 1.8" TFT screen:
      //tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
    
      // OR use this initializer (uncomment) if using a 1.44" TFT:
      //tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
    
      // OR use this initializer (uncomment) if using a 0.96" 180x60 TFT:
      //tft.initR(INITR_MINI160x80);  // Init ST7735S mini display
    
      // OR use this initializer (uncomment) if using a 1.54" 240x240 TFT:
      //tft.init(135, 240);           // Init ST7789 135x240
      //tft.init(240, 240);           // Init ST7789 240x240
    tft.init(240, 320);           // Init ST7789 240x320
      // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
      // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
      // may end up with a black screen some times, or all the time.
      // HardwareSPI speed
    tft.setSPISpeed(40000000); //40MHz


    // initialise the display
    tft.setFont();
    tft.fillScreen(Display_Backround_Color);
    tft.setTextColor(Display_Text_Color);
    tft.setTextSize(1);

    // the display is now on
    isDisplayVisible = true;

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    //Touch setup
    tft.setCursor(0,0);

    tft.drawLine(0,     0, 240-1,  320-1, Display_Color_White);
    tft.drawLine(0, 320-1, 240-1,      0, Display_Color_White);

    tft.drawCircle(30,        40,10, Display_Color_White);
    tft.drawCircle(30,    320-40,10, Display_Color_White);
    tft.drawCircle(240-30,    40,10, Display_Color_White);
    tft.drawCircle(240-30,320-40,10, Display_Color_White);

    checkColorChange();

    tft.setCursor(0,0);

    //setupTouch();
    touchXPT.begin(320, 240); // screen resolution
    touchXPT.setRotation(0); // match the cords to screen
    touchXPT.setCalibration(300, 400, 3800, 3900); // Calibrat screen
    // call back for change
    // parameter:
    // 1 - minimum pixel move for new event
    // 2 - minimum z for a event
    // 3 - callback
    touchXPT.onChange(3, 200, touchXPTevent);


}

void loop() {
    touchXPT.loop();
    if (isTouched) {
        if (lastY<32) {
            checkColorChange();
            isTouched = false;
            }
        else    
            tft.fillCircle(lastX, lastY-32, 3, lastColor);
        }
}