/*
 * touch.ino
 *
 *  Created on: 20.02.2016
 *
 * required librarys:
 *  - SPI (arduino core)
 *  - XPT2046 (https://github.com/Links2004/XPT2046)
 */

#include <Arduino.h>
#include <SPI.h>
#include <XPT2046.h>

// connect the SPI pins (MISO, MOSI, SCK) to the hardware SPI
#define TOUCH_IRQ 4
#define TOUCH_CS  16

XPT2046 touch = XPT2046(TOUCH_CS, TOUCH_IRQ);

void touchEvent(bool press, uint16_t x, uint16_t y, uint16_t z) {
    Serial.print("[Touch] press: ");
    Serial.print(press);
    Serial.print(" X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" Z: ");
    Serial.println(z);
}

void setup() {

    Serial.begin(115200);
    Serial.println("[setup] Start...");

    SPI.begin();

    touch.begin(320, 240); // screen resolution
    touch.setRotation(1); // match the cords to screen
    touch.setCalibration(350, 550, 3550, 3600); // Calibrat screen

    // call back for change
    // parameter:
    // 1 - minimum pixel move for new event
    // 2 - minimum z for a event
    // 3 - callback
    touch.onChange(3, 200, touchEvent);

    Serial.println("[setup] Done.");
}

void loop() {
    touch.loop();
}
