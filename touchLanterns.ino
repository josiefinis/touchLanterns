#define USE_TIMER_1         true

#define SERIAL_ON           false
#define MONITOR_ON          false
#define MONITOR_STATE       false
#define MONITOR_LIFE        false

#include "Sensor.h"
#include "Button.h"
#include "Candle.h"
#include "Lanterns.h"
#include "Register.h"
#include "Arduino.h"
#include <TimerInterrupt.h>
#include <ISR_Timer.h>

// A candle is given exactly four neighbours, represented by a sequence of four 4-bit indices.
// Less than four neighbours can be assigned by filling the remining spaces with the candle's own index.
// For example, if candle 4 has only two neighbours 3 and 6 this is represented by 0x4436. 
#define NEIGHBOURS_OF_CANDLE_0x0 0x0124UL
#define NEIGHBOURS_OF_CANDLE_0x1 0x0234UL
#define NEIGHBOURS_OF_CANDLE_0x2 0x0135UL
#define NEIGHBOURS_OF_CANDLE_0x3 0x3125UL
#define NEIGHBOURS_OF_CANDLE_0x4 0x0167UL
#define NEIGHBOURS_OF_CANDLE_0x5 0x2367UL
#define NEIGHBOURS_OF_CANDLE_0x6 0x458CUL
#define NEIGHBOURS_OF_CANDLE_0x7 0x458CUL
#define NEIGHBOURS_OF_CANDLE_0x8 0x679AUL
#define NEIGHBOURS_OF_CANDLE_0x9 0x8ABDUL
#define NEIGHBOURS_OF_CANDLE_0xA 0xA89BUL
#define NEIGHBOURS_OF_CANDLE_0xB 0xB9AFUL
#define NEIGHBOURS_OF_CANDLE_0xC 0x67DEUL
#define NEIGHBOURS_OF_CANDLE_0xD 0x9CEFUL
#define NEIGHBOURS_OF_CANDLE_0xE 0xECDFUL
#define NEIGHBOURS_OF_CANDLE_0xF 0xFBDEUL


// Expected life duration is equal to 16 * Lantern/CANDLE_LIFE_COUNT * CANDLE_LIFE_INTERVAL * the timer interrupt interval 
#define CANDLE_LIFE_INTERVAL      0x80           // NB must be a factor of 256. 

// Create class instances.
Register shiftRegister;             // Writes to SIPO register.
ISR_Timer ISR_timer;                // Handles timer interrupts.
Button button = Button();           // Converts output from sensor to long and short 'button' presses.
Sensor sensor = Sensor();           // Loops over 16 capacitive touch sensors with multiplexer. Takes raw sensor input and outputs binary 'is touched' signal.
Candle candleArray[16];             // State and methods for each individual candle.
Lanterns lanterns(candleArray);     // Aggregate control of candles. Responds to button input and timer interrupts and changes the state of candles.


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
/* Every timer interrupt:
     scan sensors,
     update candles,
     write to register.
*/
  #if MONITOR_STATE
  printState();
  #endif

  uint32_t buttonOutput = button.output(sensor.output());
  if ( startupCounter > 0 ) {                       // Ignore sensors briefly after start up while they settle and calibrate themselves.
    startupCounter--;
    sensor.zeroOutput();
    return;
  }
  if ( buttonOutput ) {                             // Forward button outut to lanterns controler.
    lanterns.receiveSignal(buttonOutput); 
  } 
  if ( lanterns.getActiveCounters() ) {             // Apply counter based updates (delay turning candes on/off.
    lanterns.update(); 
  }
  if ( lanterns.getIsUpdateForRegister() ) {        // Write changes in lit/unlit to register.
    shiftRegister.writeToStorageRegister(lanterns.getLitCandles()); 
    lanterns.setIsUpdateForRegister(false);
  }
  if ( lanterns.getLitCandles() ) { minuteCounter++; }
}


#if MONITOR_STATE
void printState() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print(candleArray[i].getState(), HEX); Serial.print("\t"); 
  }
  Serial.println();
}
#endif


void initialisePins() {
  // Set pins I/O.
  pinMode(PIN_SENSOR_RECEIVE, INPUT);
  pinMode(PIN_SENSOR_SEND, OUTPUT);
  pinMode(PIN_REGISTER_SER, OUTPUT);
  pinMode(PIN_REGISTER_NOT_OE, OUTPUT);
  pinMode(PIN_REGISTER_RCLK, OUTPUT);
  pinMode(PIN_REGISTER_SRCLK, OUTPUT);
  pinMode(PIN_REGISTER_NOT_SRCLR, OUTPUT);
  DDRD |= 0b11111100; // set PORTD (digital 7 to 2) to outputs
}


void initialiseTimerInterrupts() {
// Initialise timer interrupts using timer interrupt library.
  ITimer1.init();
  #if MONITOR_ON
  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler)) {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else {
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
  }
  #else
  ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler);
  #endif
  ISR_timer.setInterval(TIMER_INTERVAL_200MS, shortCycle);
}


void assignCandleNeighbourhoods() {
// Assign a neighbourhood of four candles for each candle (defines a graph).
  candleArray[0x0].setNeighbours(NEIGHBOURS_OF_CANDLE_0x0);
  candleArray[0x1].setNeighbours(NEIGHBOURS_OF_CANDLE_0x1);
  candleArray[0x2].setNeighbours(NEIGHBOURS_OF_CANDLE_0x2);
  candleArray[0x3].setNeighbours(NEIGHBOURS_OF_CANDLE_0x3);
  candleArray[0x4].setNeighbours(NEIGHBOURS_OF_CANDLE_0x4);
  candleArray[0x5].setNeighbours(NEIGHBOURS_OF_CANDLE_0x5);
  candleArray[0x6].setNeighbours(NEIGHBOURS_OF_CANDLE_0x6);
  candleArray[0x7].setNeighbours(NEIGHBOURS_OF_CANDLE_0x7);
  candleArray[0x8].setNeighbours(NEIGHBOURS_OF_CANDLE_0x8);
  candleArray[0x9].setNeighbours(NEIGHBOURS_OF_CANDLE_0x9);
  candleArray[0xA].setNeighbours(NEIGHBOURS_OF_CANDLE_0xA);
  candleArray[0xB].setNeighbours(NEIGHBOURS_OF_CANDLE_0xB);
  candleArray[0xC].setNeighbours(NEIGHBOURS_OF_CANDLE_0xC);
  candleArray[0xD].setNeighbours(NEIGHBOURS_OF_CANDLE_0xD);
  candleArray[0xE].setNeighbours(NEIGHBOURS_OF_CANDLE_0xE);
  candleArray[0xF].setNeighbours(NEIGHBOURS_OF_CANDLE_0xF);
}


void setup() {
  #if SERIAL_ON
  Serial.begin(9600);		
  while(!Serial) {}
  #endif
  initialisePins();
  initialiseTimerInterrupts();
  assignCandleNeighbourhoods();
  shiftRegister.reset();
  randomSeed(analogRead(0));
}

void loop() {
  if ( not lanterns.getLitCandles() ) { return; }
  if ( minuteCounter % CANDLE_LIFE_INTERVAL == 0 ) { 
    lanterns.burnDown(); 
    minuteCounter++;
    #if MONITOR_LIFE
    for ( uint8_t i=0; i<16; i++ ) {
      Serial.print(candleArray[i].getLifeRemaining()); Serial.print("\t");
    }
    Serial.println();
    #endif
  }
}
