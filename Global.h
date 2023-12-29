#include "Arduino.h"
#ifndef GLOBAL_H
#define GLOBAL_H

#define MONITOR_INTERVAL                    160 // ms
#define MONITOR_QUICK_PRINT                 0
#define MONITOR_TIMINGS                     0
#define MONITOR_RAW_INPUT                   0
#define MONITOR_NORMALISED_INPUT            0
#define MONITOR_INPUT                       0
#define MONITOR_STATE                       0
#define MONITOR_BRIGHTNESS                  0
#define MONITOR_LIGHT_BEHAVIOUR             0
#define MONITOR_PARENT                      0
#define MONITOR_MAKE_TREE                   0
#define MONITOR_PWM_LIST                    0
#define MONITOR_PWM                         0
#define MONITOR_DELAY                       0
#define SERIAL_ON MONITOR_INPUT | MONITOR_PWM | MONITOR_TIMINGS | MONITOR_RAW_INPUT | MONITOR_NORMALISED_INPUT | MONITOR_STATE | MONITOR_PWM_LIST | MONITOR_BRIGHTNESS | MONITOR_LIGHT_BEHAVIOUR


#define PIN_SENSOR_SEND 2
#define PIN_SENSOR_RECEIVE 3
#define PIN_MUX_S0              0b00010000      // D4 (Pin 4)
#define PIN_MUX_S1              0b00100000      // D5 (Pin 5)
#define PIN_MUX_S2              0b01000000      // D6 (Pin 6)
#define PIN_MUX_S3              0b10000000      // D7 (Pin 7)

#define PIN_REGISTER_SER        0b00000001      // B0 (Pin 8)
#define PIN_REGISTER_NOT_OE     0b00000010      // B1 (Pin 9)
#define PIN_REGISTER_NOT_SRCLR  0b00000100      // B2 (Pin 10)
#define PIN_REGISTER_RCLK       0b00001000      // B3 (Pin 11)
#define PIN_REGISTER_SRCLK      0b00010000      // B4 (Pin 12)

#define PWM_PERIOD                   9856U // µs
#define BURN_INTERVAL             0x8000UL // ms    E[total burn time] = BURN_INTERVAL * 256 * 2

#endif
