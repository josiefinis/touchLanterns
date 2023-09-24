#define USE_TIMER_1     true
#define MONITOR_ON      true

#include "Sensor.h"
#include "Button.h"
#include "Candle.h"
#include "Register.h"
#include "Arduino.h"
#include <TimerInterrupt.h>
#include <ISR_Timer.h>


uint8_t eventCountdown = 0;
ISR_Timer ISR_timer;
Button button = Button();
Sensor sensor = Sensor();
uint16_t Candle::litCandles = 0;          
uint16_t Candle::activeCounters = 0;
uint16_t Candle::newChanges = 0;
Candle candleArray[16] = Candle();
Register reg;


void TimerHandler() {
  ISR_timer.run();
}

#define HW_TIMER_INTERVAL_MS             100L
#define TIMER_INTERVAL_200MS             2000L
#define TIMER_INTERVAL_60S               60000L


void updateOften() {
  uint16_t sensorOutput = sensor.output();
  uint32_t buttonOutput = button.output(sensorOutput);
  if ( buttonOutput ) { 
    Serial.println();
    Serial.print("B->"); Serial.print(buttonOutput, BIN); Serial.print("\t");
    sendSignalToCandle(buttonOutput);
  } 
  Serial.println(); Candle::printCounters(); Candle::printNewChanges(); Candle::printLitCandles();
  printStates();
  if ( Candle::activeCounters ) {
    for (uint8_t i=0; i<16; i++ ) { candleArray[i].updateCycle(i); }
  }
  if ( Candle::hasNewChanges() ) { 
    Serial.println(); Candle::printCounters(); Candle::printNewChanges(); Candle::printLitCandles();
    Candle::applyNewChanges(); 
    reg.writeToStorageRegister(Candle::getLitCandles()); 
  }
}

void printStates() {
  Serial.println();
  for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(i); Serial.print("\t"); } Serial.print("\n");
  for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(candleArray[i].getState(), HEX); Serial.print("\t"); }Serial.print("\n");
}

void sendSignalToCandle(uint32_t input) {
  uint8_t i = 0;
  while ( input ) {
    candleArray[i].receiveSignal( i, (uint8_t) (input >> 30) ); 
    input <<= 2;
    i++;
  }
}


void setup() {
  #if MONITOR_ON
  Serial.begin(9600);		
  while(!Serial) {}
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
  ISR_timer.setInterval(TIMER_INTERVAL_200MS, updateOften);
}

void loop() {
}
