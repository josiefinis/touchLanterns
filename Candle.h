// Candle class. 
#ifndef CANDLE_H
#define CANDLE_H
#include "Arduino.h"

class Candle {
  private:
    static const uint8_t SHUFFLE[6];

    uint8_t state;            //  7-bit: is lit, 6-bit: is changing, 5-bit: is counting, 4-0-bits: counter.
    uint16_t neighbours;      // Four 4-bit indices of nearest neighbours
    Candle* watching;

    uint8_t cut(uint8_t sequence);
    
  public:
    Candle();

    uint8_t getState();
    uint16_t getNeighbours();
    Candle* getWatching();
    bool isWatching();
    bool isLit();
    bool isCounting();
    bool isChangedLastCycle();
    bool isSignaled();

    void incrementCounter();
    void toggleIsLitOnCount(uint8_t count);
    void toggleIsLit();
    void setNeighbours(uint16_t fourNeighbours);
    void setWatching(Candle* pCandle);
    void setCounterRemaining(uint8_t value);
    void setNotChangedLastCycle();
    void setLifeRemaining(uint8_t value);

    void followSuit();
    void burnDown();
};
#endif
