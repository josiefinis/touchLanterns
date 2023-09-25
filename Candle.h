// Candle class. 
#ifndef CANDLE_H
#define CANDLE_H
#include "Arduino.h"

class Candle {
  private:
    // bits for 16 candles
    static uint16_t litCandles;          
    static bool newChanges;           // New changes that require update of hardware register. 
    static uint8_t activeCounters;    // Number of candles that will change on counter completion. 


    uint8_t state; /*  7-bit   is lit
                       6-bit   is changing
                       5-bit   is counting 
                     4-0-bits  counter        
                     */
    Candle * watching;
    Candle * next;

    static uint16_t indexToOneHot(uint8_t idx);
    void followSuit();
    void buildBeaconNetwork(Candle candleArray[16], uint8_t idx);
    Candle * findWatchBeaconAbove(Candle candleArray[16], uint8_t idx);
    Candle * findWatchBeaconBelow(Candle candleArray[16], uint8_t idx);
    bool linkWatchBeacon(Candle candleArray[16], uint8_t idx);
    void toggleIsLit();
    bool isLit();
    bool changedLastCycle();
    void setNotChangedLastCycle();
    bool isCounting();
    void setRemainingCounter(uint8_t value);
    void burn();

  public:
    Candle();
    uint8_t getState();
    Candle * getWatching();
    bool isWatching();


    static uint8_t getActiveCounters();
    static bool hasUpdatesForRegister();
    static uint16_t getLitCandles();

    static void receiveSignal(Candle candleArray[16], uint32_t input);
    void update(uint8_t i);
};

#endif
