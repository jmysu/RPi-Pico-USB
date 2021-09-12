#include <Arduino.h>
#include "BitBang_I2C.h"
// PyBase SDA4 SCL5
#define SDA_PIN 4
#define SCL_PIN 5
BBI2C bbi2c;
#include "bitbangTCS34725.h"
TCS34725 tcs;
#define TCS_LED 0
#include "Adafruit_NeoPixel.h"
//pyBase AD2:pin28, 8 pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, 28, NEO_GRB + NEO_KHZ800);

void setup(void)
{
  Serial.begin(115200);
  memset(&bbi2c, 0, sizeof(bbi2c));
  bbi2c.bWire = 0; // use bit bang, not wire library
  bbi2c.iSDA = SDA_PIN;
  bbi2c.iSCL = SCL_PIN;
  I2CInit(&bbi2c, 440000L);
  delay(100); // allow devices to power up

  pinMode(TCS_LED, OUTPUT);
  digitalWrite(TCS_LED, LOW); //Turn Off


  if (!tcs.attach(&bbi2c)){
        Serial.println("ERROR: TCS34725 NOT FOUND !!!");
        }
  else {        
    tcs.integrationTime(500); // ms
    tcs.gain(TCS34725::Gain::X01);
    }
    // set LEDs...
    digitalWrite(TCS_LED, HIGH);
    
    // This initializes the NeoPixel with RED
    pixels.begin();
    pixels.setBrightness(16);
    pixels.fill(0x880000);
    pixels.show();
}

void loop(void)
{
    if (tcs.available())
    {
        static uint32_t prev_ms = millis();
        char buf[255];
   

        TCS34725::Color color = tcs.color();
        /*
        Serial.print("Interval   : "); Serial.println(millis() - prev_ms);
        Serial.print("Color Temp : "); Serial.println(tcs.colorTemperature());
        Serial.print("Lux        : "); Serial.println(tcs.lux());
        Serial.print("R          : "); Serial.println(color.r);
        Serial.print("G          : "); Serial.println(color.g);
        Serial.print("B          : "); Serial.println(color.b);
        */
        TCS34725::RawData raw = tcs.raw();
        /*
        Serial.print("Raw R      : "); Serial.println(raw.r);
        Serial.print("Raw G      : "); Serial.println(raw.g);
        Serial.print("Raw B      : "); Serial.println(raw.b);
        Serial.print("Raw C      : "); Serial.println(raw.c);
        */
       
        sprintf(buf, "Int:% 3dms Temp:% 4.0f° Lux:%4.0f RGB(%3.0f,%3.0f,%3.0f)  RGBC:%3d/%3d/%3d/%3d",
            millis() - prev_ms, tcs.colorTemperature(), tcs.lux(), color.r, color.g, color.b,
            raw.r, raw.g, raw.b, raw.c  );
        prev_ms = millis();    
        Serial.println(buf);   

        //Update NeoPixels
        int l,r,g,b;
        l=map(tcs.lux(), 0,1000, 0,255);r=color.r;g=color.g;b=color.b;
        int c = (r << 16) | (g << 8) | b;
        
        pixels.setBrightness(l);
        pixels.fill(c);
        pixels.show(); 
        delay(500);
    }
}
