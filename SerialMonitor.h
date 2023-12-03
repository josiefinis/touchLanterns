#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include "Arduino.h"
#include "Sensor.h"
#include "Lantern.h"
#include "PWMSignal.h"
#include "Register.h"
#include "Stopwatch.h"



class SerialMonitor {
// Collect data and print via Arduino serial port. For troubleshooting.
  public:
    SerialMonitor( Sensor* sensor, Lantern* lantern, PWMSignal* pwm, Register* reg );
    void print();
    void storePWM( uint16_t signal, uint16_t time );
    void printIndices();
    void printState();
    void printLanternInput();
    void printBrightness();
    void printPWM();
    void printTimings();
    void printDelay();
    void printSignalList();
      
  private:
    Sensor* sensor;
    Lantern* lantern;
    PWMSignal* pwm;
    Register* reg;

    #if MONITOR_PWM_SIGNAL
      uint8_t MONITORidx = 0;
      uint16_t edgeAtMicrosMONITOR[32];
      uint16_t signalMONITOR[32];
    #endif
};
#endif
