// Candle class. 
#ifndef CANDLE_H
#define CANDLE_H
#include "Arduino.h"

/* TODO handle long press while still responding to a previous long press.
           existing beacon network/tree will be destroyed when building a new one.
           possible solution: maintain multiple networks/trees with multiple 'next' pointers?
*/
class Candle {
  private:

    static uint8_t zeroAddress;
    static uint8_t addressStep;
    static bool updatesForRegister;           // New changes that require update of hardware register. 


    uint8_t state; /*  7-bit   is lit
                       6-bit   is changing
                       5-bit   is counting 
                     4-0-bits  counter        
                     */
    Candle* watching;
    Candle* next;

    static uint16_t indexToOneHot(uint8_t idx);
    void followSuit();
    void buildBeaconNetwork(Candle candleArray[16], uint8_t idx);
    Candle* findWatchBeaconAbove(Candle candleArray[16], uint8_t idx);
    Candle* findWatchBeaconBelow(Candle candleArray[16], uint8_t idx);
    bool linkWatchBeacon(Candle candleArray[16], uint8_t watcher, uint8_t watched);
    void toggleIsLit();
    bool isLit();
    bool changedLastCycle();
    void setNotChangedLastCycle();
    bool isCounting();
    void setCounterRemaining(uint8_t value);
    void setLifeRemaining(uint8_t value);

  public:
    Candle();
    static void setUpdatesForRegister(bool value);
    static void storeAddress(Candle candleArray[16]);
    static uint8_t addressToIndex(uint8_t address);
    static uint8_t activeCounters;    // Number of candles that will change on counter completion. 
    static bool busy;
    uint8_t getState();
    Candle* getWatching();
    bool isWatching();
    static bool anyLitCandles;


    static uint8_t getActiveCounters();
    static bool hasUpdatesForRegister();
    static uint16_t getLitCandles(Candle candleArray[16]);

    static void receiveSignal(Candle candleArray[16], uint32_t input);
    void periodicUpdate(uint8_t i);
    static void burnDown(Candle candleArray[16]);
};

#endif
