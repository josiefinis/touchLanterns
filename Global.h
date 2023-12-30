#ifndef GLOBAL_H
#define GLOBAL_H

#define IS_ARDUINO_ENV          1
#define PIN_SENSOR_SEND         2
#define PIN_SENSOR_RECEIVE      3
#define PIN_MUX_S0              0x10    // D4 (Pin 4)
#define PIN_MUX_S1              0x20    // D5 (Pin 5)
#define PIN_MUX_S2              0x40    // D6 (Pin 6)
#define PIN_MUX_S3              0x80    // D7 (Pin 7)

#define PIN_REGISTER_SER        0x01    // B0 (Pin 8)
#define PIN_REGISTER_NOT_OE     0x02    // B1 (Pin 9)
#define PIN_REGISTER_NOT_SRCLR  0x04    // B2 (Pin 10)
#define PIN_REGISTER_RCLK       0x08    // B3 (Pin 11)
#define PIN_REGISTER_SRCLK      0x10    // B4 (Pin 12)

#define PWM_PERIOD             9856U    // µs
#define BURN_INTERVAL       0x6000UL    // ms

#endif
