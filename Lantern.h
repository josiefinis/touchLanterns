// Lantern class. 
#ifndef LANTERN_H
#define LANTERN_H

#include "Arduino.h"
#include "Random.h"


class Lantern;

class LinkedList {
  public:
    LinkedList();
    uint8_t length();
    Lantern* next();
    void put(Lantern* lantern);
    void resetIndex();
    void shuffle();
    void print();

  private:
    struct Node {
      Lantern* pLantern;
      Node* pNext;
    };
    Node* p0;
    Node* pIndex;
    uint8_t nNodes;
};


class Qint {
  public:
    Qint();
    bool isEmpty();
    void enqueue(uint8_t value);
    uint8_t dequeue();
    uint8_t peek();
    void print();

  private:
    struct Node {
      uint8_t value;
      Node* pNext;
    };
    Node* pFront;
    Node* pBack;
};


class QLantern {
  public:
    QLantern();
    bool isEmpty();
    void enqueue(Lantern* lantern);
    Lantern* dequeue();
    void print();

  private:
    struct Node {
      Lantern* pLantern;
      Node* pNext;
    };
    Node* pFront;
    Node* pBack;
};


#define OUT                 0x00
#define IDLE                0x10
#define GO_OUT              0x01
#define GO_IDLE             0x11
#define INIT_DOWN           0x02
#define INIT_UP             0x12
#define FULL_DOWN           0x03
#define FULL_UP             0x13
#define AUTO_DOWN           0x04
#define AUTO_UP             0x14
#define PAUSE_DOWN          0x05
#define PAUSE_UP            0x15
#define FLICKER_DOWN        0x06
#define FLICKER_UP          0x16
#define ROOT_FULL_DOWN      0x23
#define ROOT_FULL_UP        0x33
#define ROOT_AUTO_DOWN      0x24
#define ROOT_AUTO_UP        0x34
#define ROOT_PAUSE_DOWN     0x25
#define ROOT_PAUSE_UP       0x35
#define ROOT_FLICKER_DOWN   0x26
#define ROOT_FLICKER_UP     0x36
#define WAIT                0x40
#define FOLLOW              0x80
#define WAIT_FULL_DOWN      0x43
#define WAIT_FULL_UP        0x53

#define BRIGHTNESS_MAX      0xFF
#define BRIGHTNESS_MIN      0x01
#define BRIGHTNESS_VAR      0x20
#define LONG                0xFF
#define MEDIUM              0x0F
#define EDGE                0x03
#define TOUCHED             0xFF
#define RELEASED            0x00
#define RISING              0x01
#define FALLING             0xFE

class Lantern {
  public:
    static Lantern* root;
    static uint8_t nTreeNodes;
    Lantern();

    bool update();
    void raiseBrightness(uint8_t rate=2, uint8_t ceiling=BRIGHTNESS_MAX);
    void lowerBrightness(uint8_t rate=2, uint8_t floor=BRIGHTNESS_MIN);
    bool nextBrightness();
    void flickerBrightness();
    void pulseBrightness();
    void pushInput(uint8_t value);
    void makeTree();
    void burnDown();
    void addNeighbour(Lantern* neighbour);
    Lantern* nextNeighbour();
    void printNeighbours();

    uint8_t getIndex();
    uint8_t getState();
    uint8_t getInput();
    uint8_t getBrightness();
    uint8_t getBrightnessTarget();
    uint8_t getDelay();
    Lantern* getParent();

    void setIndex(uint8_t value);
    void setState(uint8_t value);
    void setBrightness(uint8_t value);
    void setParent(Lantern* pLantern);
    void setDelay(uint8_t value);
    void setNeighbours(Lantern* neighbour[4], uint8_t nNeighbours);

  private:
    uint8_t index;
    uint8_t state;
    uint16_t input;
    uint8_t brightness;
    Qint brightnessQueue;
    uint8_t brightnessTarget;
    uint8_t delay;
    Lantern* parent;
    LinkedList neighbourList;

    void incrementCounter();
    void decrementCounter();
    bool out();
    bool idle();
    bool goOut();
    bool goIdle();
    bool initUp();
    bool initDown();
    bool fullUp();
    bool fullDown();
    bool autoUp();
    bool autoDown();
    bool pauseUp();
    bool pauseDown();
    bool flickerUp();
    bool flickerDown();
    bool rootInitUp();
    bool rootInitDown();
    bool rootFullUp();
    bool rootFullDown();
    bool rootAutoUp();
    bool rootAutoDown();
    bool rootPauseUp();
    bool rootPauseDown();
    bool rootFlickerUp();
    bool rootFlickerDown();
    bool wait();
    bool waitFullUp();
    bool waitFullDown();
    bool follow();

    bool isInput(uint8_t value, uint8_t mask=0x01); 
    bool isTouched(uint8_t duration=0x01);
    bool isReleased(uint8_t duration=0x01);
    bool isDoubleTouched();
};
#endif
