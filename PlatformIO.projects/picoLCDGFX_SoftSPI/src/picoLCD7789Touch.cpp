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
// For 1.3", 1.54", and 2.0" TFT with ST7789:
//jmysu:As Waveshare PicoLCD ties LCD_DC/LCD_RST to SPI1_RX, looks like the Hardware SPI1 is not work now; switch to SOFT SPI
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out
//#define TFT_MOSI       PICO_MOSI // Data out
//#define TFT_SCLK       PICO_SCK  // Clock out
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

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
Adafruit_ST7789 tft = Adafruit_ST7789(LCD28_CS_PIN, LCD28_DC_PIN, LCD28_MOSI_PIN, LCD28_CLK_PIN, LCD28_RST_PIN);

#define BOXSIZE 40

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

// the interrupt service routine affects this
volatile bool   isTouched         = false;

void displayUpTime() {

    // calculate seconds, truncated to the nearest whole second
    unsigned long upSeconds = millis() / 1000;

    // calculate days, truncated to nearest whole day
    unsigned long days = upSeconds / 86400;

    // the remaining hhmmss are
    upSeconds = upSeconds % 86400;

    // calculate hours, truncated to the nearest whole hour
    unsigned long hours = upSeconds / 3600;

    // the remaining mmss are
    upSeconds = upSeconds % 3600;

    // calculate minutes, truncated to the nearest whole minute
    unsigned long minutes = upSeconds / 60;

    // the remaining ss are
    upSeconds = upSeconds % 60;

    // allocate a buffer
    char newTimeString[MaxString] = { 0 };

    // construct the string representation
    sprintf(
        newTimeString,
        "%lu %02lu:%02lu:%02lu",
        days, hours, minutes, upSeconds
    );

    // has the time string changed since the last tft update?
    if (strcmp(newTimeString,oldTimeString) != 0) {

        // yes! home the cursor
        tft.setCursor(120-30,32);

        // change the text color to the background color
        tft.setTextColor(Display_Backround_Color);

        // redraw the old value to erase
        tft.print(oldTimeString);

        // home the cursor
        tft.setCursor(120-30,32);
        
        // change the text color to foreground color
        tft.setTextColor(Display_Text_Color);
    
        // draw the new time value
        tft.print(newTimeString);
    
        // and remember the new value
        strcpy(oldTimeString,newTimeString);
    }
}

extern void checkColorChange();
void setupTouch();
void loopTouchRaw();

void setup() {

    #if (SerialDebugging)
    Serial.begin(115200); while (!Serial); Serial.println();
    delay(500);
    Serial.println("Start LCD/Touch test...");
    #endif

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
      tft.setSPISpeed(10000000);


    // initialise the display
    tft.setFont();
    tft.fillScreen(Display_Backround_Color);
    tft.setTextColor(Display_Text_Color);
    tft.setTextSize(1);

    // the display is now on
    isDisplayVisible = true;

    pinMode(LCD28_BKL_PIN, OUTPUT);
    digitalWrite(LCD28_BKL_PIN, HIGH);

    //Touch setup
    tft.setCursor(0,0);

    tft.drawLine(0,     0, 240-1,  320-1, Display_Color_White);
    tft.drawLine(0, 320-1, 240-1,      0, Display_Color_White);

    tft.drawCircle(30,        40,10, Display_Color_White);
    tft.drawCircle(30,    320-40,10, Display_Color_White);
    tft.drawCircle(240-30,    40,10, Display_Color_White);
    tft.drawCircle(240-30,320-40,10, Display_Color_White);
    /*
    //draw 6 color blocks
    tft.fillRect(0,         0, BOXSIZE, BOXSIZE*3/4, Display_Color_Red);
    tft.fillRect(BOXSIZE,   0, BOXSIZE, BOXSIZE*3/4, Display_Color_Yellow);
    tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE*3/4, Display_Color_Green);
    tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE*3/4, Display_Color_Cyan);
    tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE*3/4, Display_Color_Magenta);
    tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE*3/4, Display_Color_Black);
    //
    tft.fillRect(0, 28, 240, 4, Display_Color_Black);
    */
    checkColorChange();

    tft.setCursor(0,0);

    setupTouch();
    delay(500);

}

uint8_t iRot =0;
void loop() {

    // unconditional display, regardless of whether display is visible
    if (millis()%1000==0)
        displayUpTime();
/*
    // has the button been pressed?
    if (isTouched) {
        
        // yes! toggle display visibility
        isDisplayVisible = !isDisplayVisible;

        // apply
        tft.enableDisplay(isDisplayVisible);

        #if (SerialDebugging)
        Serial.print("button pressed @ ");
        Serial.print(millis());
        Serial.print(", display is now ");
        Serial.println((isDisplayVisible ? "ON" : "OFF"));
        #endif

        // confirm button handled
        isTouched = false;
        
        if (isDisplayVisible){
          tft.setRotation(iRot++);
          if (iRot>3) iRot=0;
          }

        delay(500);  
    }
*/
    // no need to be in too much of a hurry
    delay(10);

    loopTouchRaw();
}