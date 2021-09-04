/**
 * @file XPT2046.h
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

#ifndef XPT2046_H_
#define XPT2046_H_

#include <Arduino.h>
#include <SPI.h>

class XPT2046 {
    public:
        typedef void (*XPT204Event)(bool press, uint16_t x, uint16_t y, uint16_t z);

        XPT2046(int8_t CS, int8_t IRQ);
        ~XPT2046();
        void begin(uint16_t width, uint16_t height);
        void loop();

        void setRotation(uint8_t m);
        void setCalibration(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY);

        void read(uint16_t * oX, uint16_t * oY, uint16_t * oZ);
        void readRaw(uint16_t * oX, uint16_t * oY, uint16_t * oZ);

        void onChange(uint16_t min, uint16_t pressure, XPT204Event cb);
    protected:
        int8_t _pinCs;
        int8_t _pinIrq;

        uint16_t _width;
        uint16_t _height;

        uint16_t _rotation;

        uint16_t _minX;
        uint16_t _minY;

        uint16_t _maxX;
        uint16_t _maxY;

        uint16_t _maxValue;

        SPISettings _spiSettings;

        int _lastX;
        int _lastY;

        uint16_t _minChange;
        uint16_t _pressure;
        XPT204Event _cb;

        void enableIrq();

        inline void spiCsHigh(void) __attribute__((always_inline));
        inline void spiCsLow(void) __attribute__((always_inline));

        inline void spi_begin(void) __attribute__((always_inline));
        inline void spi_end(void) __attribute__((always_inline));

};





#endif /* XPT2046_H_ */
