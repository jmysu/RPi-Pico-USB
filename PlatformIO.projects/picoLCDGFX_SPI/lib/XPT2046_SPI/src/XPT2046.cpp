/**
 * @file XPT2046.cpp
 * @date 19.02.2016
 * @author Markus Sattler
 *
 * Copyright (c) 2015 Markus Sattler. All rights reserved.
 * This file is part of the XPT2046 driver for Arduino.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "XPT2046.h"
#include <debug.h>

#define _BV(bit) (1 << (bit))

#define XPT2046_CFG_START   _BV(7)

#define XPT2046_CFG_MUX(v)  ((v&0b111) << (4))

#define XPT2046_CFG_8BIT    _BV(3)
#define XPT2046_CFG_12BIT   (0)

#define XPT2046_CFG_SER     _BV(2)
#define XPT2046_CFG_DFR     (0)

#define XPT2046_CFG_PWR(v)  ((v&0b11))

#define XPT2046_MUX_Y       0b101
#define XPT2046_MUX_X       0b001

#define XPT2046_MUX_Z1      0b011
#define XPT2046_MUX_Z2      0b100


XPT2046::XPT2046(int8_t CS, int8_t IRQ) {
    _pinCs = CS;
    _pinIrq = IRQ;
    _spiSettings = SPISettings(2500000, MSBFIRST, SPI_MODE0);
    _maxValue = 0x0fff;
    _width = 0;
    _height = 0;
    _rotation = 0;

    _minX = 0;
    _minY = 0;

    _maxX = _maxValue;
    _maxY = _maxValue;

    _lastX = -1;
    _lastY = -1;

    _minChange = 10;
    _pressure = 100;
    _cb = NULL;
}

XPT2046::~XPT2046() {
    pinMode(_pinIrq, INPUT);
    pinMode(_pinCs, INPUT);
}

void XPT2046::begin(uint16_t width, uint16_t height) {
    pinMode(_pinIrq, INPUT_PULLUP);
    pinMode(_pinCs, OUTPUT);

    _width = width;
    _height = height;

    // Init chip
    enableIrq();
}

void XPT2046::loop() {
    // touch on low
    if(!digitalRead(_pinIrq)) {
        uint16_t x, y, z;
        read(&x, &y, &z);
        if(_cb) {
            if(abs(x - _lastX) > _minChange || abs(y - _lastY) > _minChange) {
                _cb((z > _pressure), x, y, z);
                _lastX = x;
                _lastY = y;
            }
        }
    } else {
        if(_lastX >= 0) {
            if(_cb) {
                _cb(false, _lastX, _lastY, 0);
            }
            _lastX = -10000;
            _lastY = -10000;
        }
    }
}

void XPT2046::setRotation(uint8_t m) {
    _rotation = m % 4;
}

void XPT2046::setCalibration(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY) {
    _minX = minX;
    _minY = minY;
    _maxX = maxX;
    _maxY = maxY;
}

void XPT2046::read(uint16_t * oX, uint16_t * oY, uint16_t * oZ) {
    uint16_t x, y;
    readRaw(&x, &y, oZ);

    uint32_t cX = x;
    uint32_t cY = y;


    if(cX < _minX) {
        cX = 0;
    } else  if(cX > _maxX) {
        cX = _width;
    } else {
        cX -= _minX;
        cX = ((cX << 8) / (((_maxX - _minX) << 8) / (_width << 8)) )>> 8;
    }

    if(cY < _minY) {
        cY = 0;
    } else if(cY > _maxY) {
        cY = _height;
    } else {
        cY -= _minY;
        cY = ((cY << 8) / (((_maxY - _minY) << 8) / (_height << 8))) >> 8;
    }

    *oX = cX;
    *oY = cY;

    //char buf[64];
    //sprintf(buf, "[Touch] raw X: %d Y: %d - real: X: %d Y: %d\n", x, y, cX, cY);
    //Serial.print(buf);
}

void XPT2046::onChange(uint16_t min, uint16_t pressure, XPT204Event cb) {
    _minChange = min;
    _pressure = pressure;
    _cb = cb;
}


void XPT2046::readRaw(uint16_t * oX, uint16_t * oY, uint16_t * oZ) {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z1 = 0;
    uint32_t z2 = 0;
    uint8_t i = 0;

    spi_begin();
    spiCsLow();

    for(; i < 15; i++) {
        // SPI requirer 32bit aliment
        uint8_t buf[12] = {
                (XPT2046_CFG_START | XPT2046_CFG_12BIT | XPT2046_CFG_DFR | XPT2046_CFG_MUX(XPT2046_MUX_Y) | XPT2046_CFG_PWR(3)), 0x00, 0x00,
                (XPT2046_CFG_START | XPT2046_CFG_12BIT | XPT2046_CFG_DFR | XPT2046_CFG_MUX(XPT2046_MUX_X) | XPT2046_CFG_PWR(3)), 0x00, 0x00,
                (XPT2046_CFG_START | XPT2046_CFG_12BIT | XPT2046_CFG_DFR | XPT2046_CFG_MUX(XPT2046_MUX_Z1)| XPT2046_CFG_PWR(3)), 0x00, 0x00,
                (XPT2046_CFG_START | XPT2046_CFG_12BIT | XPT2046_CFG_DFR | XPT2046_CFG_MUX(XPT2046_MUX_Z2)| XPT2046_CFG_PWR(3)), 0x00, 0x00
        };

        //SPI.transferBytes(&buf[0], &buf[0], 12);
        SPI.transfer(&buf[0], 12);
        

        y += (buf[1] << 8 | buf[2])>>3;
        x += (buf[4] << 8 | buf[5])>>3;
        z1 += (buf[7] << 8 | buf[8])>>3;
        z2 += (buf[10] << 8 | buf[11])>>3;
    }

    enableIrq();

    if(i == 0) {
        *oX = 0;
        *oY = 0;
        *oZ = 0;
        return;
    }

    x /= i;
    y /= i;
    z1 /= i;
    z2 /= i;

    spiCsHigh();
    spi_end();



    switch(_rotation) {
        case 0:
        default:
            break;
        case 1:
            x = (_maxValue - x);
            y = (_maxValue - y);
            break;
        case 2:
            y = (_maxValue - y);
            break;
        case 3:
            x = (_maxValue - x);
            break;
    }

    int z = z1 + _maxValue - z2;

    *oX = x;
    *oY = y;
    *oZ = z;

    //hexdump(buf, 8, 4);

    //Serial1.printf("[Touch] X: %d Y: %d Z: %d (%d, %d)\n", x, y, z, z1, z2);

}

void XPT2046::enableIrq() {
    spi_begin();
    spiCsLow();
    const uint8_t buf[4] = { (XPT2046_CFG_START | XPT2046_CFG_12BIT | XPT2046_CFG_DFR | XPT2046_CFG_MUX(XPT2046_MUX_Y)), 0x00, 0x00, 0x00 };
    //SPI.writeBytes((uint8_t *) &buf[0], 3);
    SPI.transfer((uint8_t *) &buf[0], 3);
    
    spiCsHigh();
    spi_end();
}

inline void XPT2046::spiCsHigh(void) {
    digitalWrite(_pinCs, 1);
}

inline void XPT2046::spiCsLow(void) {
    digitalWrite(_pinCs, 0);
}

inline void XPT2046::spi_begin(void) {
    SPI.beginTransaction(_spiSettings);
}

inline void XPT2046::spi_end(void) {
    SPI.endTransaction();
}

