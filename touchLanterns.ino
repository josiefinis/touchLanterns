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
#include "Global.h"

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



// Create class instances.
Register shiftRegister;             // Writes to SIPO register.
Button button = Button();           // Converts output from sensor to long and short 'button' presses.
Sensor sensor = Sensor();           // Loops over 16 capacitive touch sensors with multiplexer. Takes raw sensor input and outputs binary 'is touched' signal.
Candle candleArray[16];             // State and methods for each individual candle.
Lanterns lanterns(candleArray);     // Aggregate control of candles. Responds to button input and timer interrupts and changes the state of candles.


uint16_t lastSensorTime = millis();
uint16_t lastLanternsUpdateTime = millis();
uint16_t lastRegisterUpdateTime = millis();
uint16_t lastBurnTime = millis();
uint16_t lastMonitorTime = millis();

uint8_t startupCounter = 6;


#define SENSOR_INTERVAL                200U
#define LANTERNS_UPDATE_INTERVAL       200U
#define REGISTER_UPDATE_INTERVAL       200U
#define BURN_INTERVAL                60000U   // Expected life duration is equal to 16 * CANDLE_LIFE_COUNT * BURN_INTERVAL
#define MONITOR_INTERVAL              1000U


void pollSensors() {
// Poll capacitive sensors every SENSOR_INTERVAL ms, convert to button output and forward button ouptut to lanterns controler.
  uint32_t buttonOutput = button.output(sensor.output());
  if ( startupCounter > 0 ) {                       // Ignore sensors briefly after start up while they settle and calibrate themselves.
    startupCounter--;
    sensor.zeroOutput();
    return 0;
  }
  if ( not buttonOutput ) { return 0; }
  lanterns.receiveSignal(buttonOutput); 
}


void updateLanterns() {
// Apply counter based updates that control the delay turning candes on/off.
  if ( not lanterns.getActiveCounters() ) { return 0; }            
  lanterns.update(); 
}


void updateRegister() {
// Write changes in lit/unlit to register.
  if ( not lanterns.getIsUpdateForRegister() ) { return 0; }        
  lanterns.setIsUpdateForRegister(false);
  shiftRegister.writeToStorageRegister(lanterns.getLitCandles()); 
}


void burnCandles() {
// Burn down all lit candles. 
  if ( not lanterns.getLitCandles() ) { return 0; }
  lanterns.burnDown(); 
 
  #if MONITOR_LIFE
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print(candleArray[i].getLifeRemaining()); Serial.print("\t");
  }
  Serial.println();
  #endif
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
  DDRB |= PIN_REGISTER_SER | PIN_REGISTER_NOT_OE | PIN_REGISTER_RCLK | PIN_REGISTER_SRCLK | PIN_REGISTER_NOT_SRCLR;



  DDRD |= 0b11111100; // set PORTD (digital 7 to 2) to outputs
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
  assignCandleNeighbourhoods();
  shiftRegister.reset();
  randomSeed(analogRead(0));
}

void loop() {
  uint16_t currentTime = millis();

  #if MONITOR_STATE
  if ( currentTime - lastMonitorTime > MONITOR_INTERVAL ) {
    lastMonitorTime = currentTime;
    printState();
  }
  #endif

  if ( currentTime - lastSensorTime > SENSOR_INTERVAL ) {
    lastSensorTime = currentTime;
    pollSensors();
  }
  if ( currentTime - lastLanternsUpdateTime > LANTERNS_UPDATE_INTERVAL ) {
    lastLanternsUpdateTime = currentTime;
    updateLanterns();
  }
  if ( currentTime - lastRegisterUpdateTime > REGISTER_UPDATE_INTERVAL ) {
    lastRegisterUpdateTime = currentTime;
    updateRegister();
  }
  if ( currentTime - lastBurnTime > BURN_INTERVAL ) {
    lastBurnTime = currentTime;
    burnCandles();
  }
}
