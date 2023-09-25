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
Register reg;


void TimerHandler() {
  ISR_timer.run();
}

#define HW_TIMER_INTERVAL_MS             100L
#define TIMER_INTERVAL_200MS             200L
#define TIMER_INTERVAL_60S               60000L


void updateOften() {
  uint32_t buttonOutput = button.output(sensor.output());
  if ( buttonOutput ) { 
    sendSignalToCandle(buttonOutput);
  } 
  if ( Candle::getActiveCounters() ) {
    for (uint8_t i=0; i<16; i++ ) { Candle::candleArray[i]->update(i); }
  }
  if ( Candle::hasUpdatesForRegister() ) { 
    reg.writeToStorageRegister(Candle::getLitCandles()); 
  }
}

#if MONITOR_ON
void printStates() {
  Serial.println();
  for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(i); Serial.print("\t"); } Serial.print("\n");
  for ( uint8_t i = 0; i < 16; i++ ) { Serial.print(Candle::candleArray[i]->getState(), HEX); Serial.print("\t"); }Serial.print("\n");
}
#endif

void sendSignalToCandle(uint32_t input) {
  uint8_t i = 0;
  while ( input ) {
    Candle::candleArray[i]->receiveSignal( i, input & 0b11 ); 
    input >>= 2;
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
