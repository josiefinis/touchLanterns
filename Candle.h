// Candle class. 
#ifndef CANDLE_H
#define CANDLE_H
#include "Arduino.h"

class Candle {
  private:
    // bits for 16 candles
    static uint16_t litCandles;          
    static uint16_t newChanges;       // Bits to flip in hardware register.

    uint8_t state; /*  7-bit   is lit
                       6-bit   is changing
                       5-bit   is counting 
                     4-0-bits  counter        
                     */
    Candle* watching;
    Candle* next;

    static uint16_t indexToOneHot(uint8_t idx);
    bool update();
    void followSuit();
    void buildBeaconNetwork(uint8_t idx);
    Candle * findWatchBeaconAbove(uint8_t idx);
    Candle * findWatchBeaconBelow(uint8_t idx);
    bool linkWatchBeacon(uint8_t idx);
    void toggleIsLit();
    bool isLit();
    bool changedLastCycle();
    void setNotChangedLastCycle();
    bool isCounting();
    void setRemainingCounter(uint8_t value);
    void burn();

  public:
    Candle();
    static uint16_t activeCounters;    // 1: increment counter , 0: do nothing.
    void receiveSignal(uint8_t, uint8_t input);
    uint16_t updateCycle(uint8_t i);
    uint8_t getState();
    Candle * getWatching();
    bool isWatching();
    static void printCounters();
    static void printNewChanges();
    static void printLitCandles();
    static bool hasNewChanges();
    static uint16_t getLitCandles();
    static void applyNewChanges();

};

#endif
