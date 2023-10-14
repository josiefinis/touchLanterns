#define USE_TIMER_1          true
#define MONITOR_ON           false
#define LIMITED_MONITOR_ON   false


#include "Sensor.h"
#include "Button.h"
#include "Candle.h"
#include "Lanterns.h"
#include "Register.h"
#include "Arduino.h"
#include <TimerInterrupt.h>
#include <ISR_Timer.h>


Register shiftRegister;
ISR_Timer ISR_timer;
Button button = Button();
Sensor sensor = Sensor();
Candle candleArray[16];
Lanterns lanterns(candleArray);
candleArray[0x0].setNeighbours(0x0000)
candleArray[0x1].setNeighbours(0x0000)
candleArray[0x2].setNeighbours(0x0000)
candleArray[0x3].setNeighbours(0x0000)
candleArray[0x4].setNeighbours(0x0000)
candleArray[0x5].setNeighbours(0x0000)
candleArray[0x6].setNeighbours(0x0000)
candleArray[0x7].setNeighbours(0x0000)
candleArray[0x8].setNeighbours(0x0000)
candleArray[0x9].setNeighbours(0x0000)
candleArray[0xA].setNeighbours(0x0000)
candleArray[0xB].setNeighbours(0x0000)
candleArray[0xC].setNeighbours(0x0000)
candleArray[0xD].setNeighbours(0x0000)
candleArray[0xE].setNeighbours(0x0000)
candleArray[0xF].setNeighbours(0x0000)


uint8_t minuteCounter = 1;
uint8_t startupCounter = 25;


void TimerHandler() {
  ISR_timer.run();
}

#define HW_TIMER_INTERVAL_MS             100L
#if MONITOR_ON
#define TIMER_INTERVAL_200MS             1000L
#else
#define TIMER_INTERVAL_200MS             200L
#endif


void shortCycle() {
  if ( Candle::busy ) { return; }

  #if MONITOR_ON
  printStates();
  #else 
  #if LIMITED_MONITOR_ON
  printLimited();
  #endif
  #endif

  uint32_t buttonOutput = button.output(sensor.output());
  if ( startupCounter > 0 ) {
    startupCounter--;
    sensor.zeroOutput();
    return;
  }
  if ( buttonOutput ) { 
    lanterns.receiveSignal(buttonOutput); 
  } 
  if ( lanterns.getActiveCounters() ) {
    lanterns.update(); }
  }
  if ( lanterns.getIsUpdatesForRegister() ) { 
    shiftRegister.writeToStorageRegister(lanterns.getLitCandles()); 
    lanterns.setUpdatesForRegister(false);
  }
  if ( lanterns.litCandles ) { minuteCounter++; }
}


#if LIMITED_MONITOR_ON
void printLimited() {
  Serial.println();
  //Serial.print("u"); Serial.print(Candle::hasUpdatesForRegister()); Serial.print("\t");
  //Serial.print("c"); Serial.print(Candle::activeCounters); Serial.print("\t");  // TODO make getActiveCounters function
  //Serial.print("l"); Serial.print(Candle::getLitCandles(candleArray), BIN); Serial.print("\n");
  Serial.print(minuteCounter); Serial.print("\t");
  for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(candleArray[i].getState(), HEX); Serial.print("\t"); }
}
#endif


#if MONITOR_ON
void printStates() {
  Serial.println();
  Serial.print("Counters: "); Serial.print(lanterns.activeCounters); Serial.print("\n");
  Serial.print("index   \t"); for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(i); Serial.print("\t"); } Serial.print("\n");
  //Serial.print("address \t"); for ( uint8_t i = 0; i < 16; i++ ) { Serial.print((long) &candleArray[i], HEX); Serial.print("\t"); } Serial.print("\n");
  Serial.print("state   \t"); for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(candleArray[i].getState(), HEX); Serial.print("\t"); }Serial.print("\n");
  Serial.print("watching\t"); for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(lanterns.addressToIndex(candleArray[i].getWatching())); Serial.print("\t"); }Serial.print("\n");
  Serial.print("state_w \t"); for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(candleArray[i].getWatching()->getState(), HEX); Serial.print("\t"); }Serial.print("\n");
}
#endif


void setup() {
  #if MONITOR_ON
  Serial.begin(9600);		
  while(!Serial) {}
  #else 
  #if LIMITED_MONITOR_ON
  Serial.begin(9600);		
  while(!Serial) {}
  #endif
  #endif

  pinMode(PIN_SENSOR_RECEIVE, INPUT);
  pinMode(PIN_SENSOR_SEND, OUTPUT);
  pinMode(PIN_REGISTER_SER, OUTPUT);
  pinMode(PIN_REGISTER_NOT_OE, OUTPUT);
  pinMode(PIN_REGISTER_RCLK, OUTPUT);
  pinMode(PIN_REGISTER_SRCLK, OUTPUT);
  pinMode(PIN_REGISTER_NOT_SRCLR, OUTPUT);
  DDRD |= 0b11111100; // set PORTD (digital 7 to 2) to outputs

  ITimer1.init();
  #if MONITOR_ON
  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {

    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
  #else
  ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler);
  #endif
  ISR_timer.setInterval(TIMER_INTERVAL_200MS, shortCycle);
  shiftRegister.reset();
}

void loop() {
  if ( not lanterns.litCandles ) { return; }
  if ( minuteCounter ) { return; }
  lanterns.burnDown(); 
  minuteCounter++;
}
