
/*
Raspberry Pi Pico pinout diagram

                                              ┌╌ LED (GP25)
                                              ┆ ┏━━━━┓          
                                          ┌─────┃    ┃─────┐
UART0 TX | I2C0 SDA | SPI0 RX  | GP0  | 01│●  ┆ ┗━━━━┛    ●│40 | VBUS
UART0 RX | I2C0 SCL | SPI0 CSn | GP1  | 02│●  ⏄           ●│39 | VSYS
                                 GND  | 03│■              ■│38 | GND
           I2C1 SDA | SPI0 SCK | GP2  | 04│●    ╭─╮       ●│37 | 3V3_EN
           I2C1 SCL | SPI0 TX  | GP3  | 05│●    │ │       ●│36 | 3V3(OUT)
UART1 TX | I2C0 SDA | SPI0 RX  | GP4  | 06│●    ╰─╯       ●│35 |          | ADC_VREF
UART1 RX | I2C0 SCL | SPI0 CSn | GP5  | 07│●              ●│34 | GP28     | ADC2
                                 GND  | 08│■              ■│33 | GND      | AGND
           I2C1 SDA | SPI0 SCK | GP6  | 09│●   ┌─────┐    ●│32 | GP27     | ADC1     | I2C1 SCL
           I2C1 SCL | SPI0 TX  | GP7  | 10│●   │     │    ●│31 | GP26     | ADC0     | I2C1 SDA
UART1 TX | I2C0 SDA | SPI1 RX  | GP8  | 11│●   │     │    ●│30 | RUN
UART1 RX | I2C0 SCL | SPI1 CSn | GP9  | 12│●   └─────┘    ●│29 | GP22
                                 GND  | 13│■              ■│28 | GND
           I2C1 SDA | SPI1 SCK | GP10 | 14│●              ●│27 | GP21     |          | I2C0 SCL
           I2C1 SCL | SPI1 TX  | GP11 | 15│●              ●│26 | GP20     |          | I2C0 SDA
UART0 TX | I2C0 SDA | SPI1 RX  | GP12 | 16│●              ●│25 | GP19     | SPI0 TX  | I2C1 SCL
UART0 RX | I2C0 SCL | SPI1 CSn | GP13 | 17│●              ●│24 | GP18     | SPI0 SCK | I2C1 SDA
                                 GND  | 18│■              ■│23 | GND
           I2C1 SDA | SPI1 SCK | GP14 | 19│●              ●│22 | GP17     | SPI0 CSn | I2C0 SCL | UART0 RX
           I2C1 SCL | SPI1 TX  | GP15 | 20│●     ● ■ ●    ●│21 | GP16     | SPI0 RX  | I2C0 SDA | UART0 TX
                                          └────────────────┘
                                                 ┆ ┆ ┆
                                                 ┆ ┆ └╌ SWDIO
                                                 ┆ └╌╌╌ GND
                                                 └╌╌╌╌╌ SWCLK



*/

/*
Zero2040 pinout

Raspberry Pi Pico pinout diagram

                                              ┌╌ LED/WS8212 (GP25)
                                              ┆ ┏━━━━┓          
                                          ┌─────┃    ┃─────┐
                      UART0 TX | GP0  | 01│●  ┆ ┗━━━━┛    ●│40 | VBUS
                      UART0 RX | GP1  | 02│●  ⏄          ●│39 | VSYS
                                 GND  | 03│■              ■│38 | GND
                      I2C1 SDA | GP2  | 04│●    ╭─╮       ●│37 | 3V3_EN
                      I2C1 SCL | GP3  | 05│●    │ │       ●│36 | 3V3(OUT)
                                 GP4  | 06│●    ╰─╯       ●│35 |          | ADC_VREF
                                 GP5  | 07│●              ●│34 | GP28     | ADC2
                                 GND  | 08│■              ■│33 | GND      | AGND
                                 GP6  | 09│●   ┌─────┐    ●│32 | GP27     | ADC1      
                                 GP7  | 10│●   │     │    ●│31 | GP26     | ADC0      
                      UART1 TX | GP8  | 11│●   │     │    ●│30 | RUN
                      UART1 RX | GP9  | 12│●   └─────┘    ●│29 | GP22
                                 GND  | 13│■              ■│28 | GND
                                 GP10 | 14│●              ●│27 | GP21     | I2C0 SCL
                                 GP11 | 15│●              ●│26 | GP20     | I2C0 SDA
                      SPI1 RX  | GP12 | 16│●              ●│25 | GP19     | SPI0 TX  
                      SPI1 CSn | GP13 | 17│●              ●│24 | GP18     | SPI0 SCK 
                                 GND  | 18│■              ■│23 | GND
                      SPI1 SCK | GP14 | 19│●              ●│22 | GP17     | SPI0 CSn
                      SPI1 TX  | GP15 | 20│●     ● ■ ●    ●│21 | GP16     | SPI0 RX  
                                          └────────────────┘
                                                 ┆ ┆ ┆
                                                 ┆ ┆ └╌ SWDIO
                                                 ┆ └╌╌╌ GND
                                                 └╌╌╌╌╌ SWCLK

*/