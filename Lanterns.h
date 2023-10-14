// Lanterns class. 
#ifndef LANTERNS_H
#define LANTERNS_H

#include "Arduino.h"
#include "Candle.h"


struct queueNode {
  Candle* pCandle;
  queueNode* next;
};

/* TODO disable interrupts when building beacon tree.
*/
class Lanterns {
  private:
    Candle* pCandleArray;
    uint16_t litCandles;
    uint8_t activeCounters;    // Number of candles that will change on counter completion. 
    bool isUpdateForRegister;           // New changes that require update of hardware register. 

    bool isLongPressContainedIn(uint32_t input);
    uint8_t longPressIndex(uint32_t input);
    void buildBeaconTree(Candle* originCandle);

  public:
    Lanterns(Candle* pArray);

    uint16_t getLitCandles();
    uint8_t getActiveCounters();
    bool getIsUpdateForRegister();  // TODO rename this mess

    void setIsUpdateForRegister(bool value);
    void update();
    void receiveSignal(uint32_t input);
    void burnDown();

};

#endif
