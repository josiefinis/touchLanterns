#include "Arduino.h"
#ifndef GLOBAL_H
#define GLOBAL_H

#define MONITOR_INTERVAL                    1262 // ms
#define MONITOR_ON                          false
#define MONITOR_TIMINGS                     false
#define MONITOR_RAW_INPUT                   false
#define MONITOR_NORMALISED_INPUT            true
#define MONITOR_LANTERN_INPUT               false
#define MONITOR_STATE                       false
#define MONITOR_BRIGHTNESS                  false
#define MONITOR_BUILD_TREE                  false
#define MONITOR_PWM_LIST                    false
#define MONITOR_REGISTER_SIGNAL             false
#define MONITOR_DELAY                       false
#define SERIAL_ON MONITOR_TIMINGS | MONITOR_RAW_INPUT | MONITOR_NORMALISED_INPUT | MONITOR_BUTTON_OUTPUT | MONITOR_BUTTON_PRESS | MONITOR_ON | MONITOR_STATE | MONITOR_BRIGHTNESS | MONITOR_PWM_LIST| MONITOR_REGISTER_SIGNAL | MONITOR_LIFE | MONITOR_BUILD_TREE


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


#endif
