#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include "Arduino.h"
#include "Global.h"
#include "Lantern.h"
#include "PWMSignal.h"


#define BUFFER_SIZE 32


class SerialMonitor {
// Collect data and print via Arduino serial port. For troubleshooting.
  public:
    SerialMonitor();
    void incrementIndex();
    bool isBufferFull();
    void print();
    void storePWM( uint16_t signal, uint16_t time );
    void storeState( uint8_t index, uint8_t state );
    void storeInput( uint8_t index, uint16_t input );
    void printBuffer();
    void printIndices();
    void printState( Lantern* lantern );
    void printLanternInput( Lantern* lantern );
    void printBrightness( Lantern* lantern );
    void printPWM();
    void printTimings();
    void printDelay( Lantern* lantern );
    void printSignalList( PWMSignal* pwmSignal );
      
  private:
    #if MONITOR_EVENTS
      uint8_t eventIdx = 0;
      uint8_t index[32];
      uint8_t state[32];
      uint16_t input[32];
    #endif
    #if MONITOR_PWM_SIGNAL
      uint8_t pwmIdx = 0;
      uint16_t pwmTime[32];
      uint16_t pwmSignal[32];
    #endif
};
#endif
