#include "Global.h"

#include "Sensor.h"
#include "Lantern.h"
#include "PWMSignal.h"
#include "Register.h"


// Create class instances.
Register shiftRegister;         // Writes to SIPO register chip.
PWMSignal pwmSignal;            // Generates a pulse width modulation signal for each lantern corresponding to its brightness.
Sensor sensor = Sensor();       // Loops over 16 capacitive touch sensors with multiplexer. Takes raw sensor input and outputs binary 'is touched' signal.
Lantern lantern[16];            // State machine that controls lantern logic.


uint8_t idx;
uint32_t pwmPhase0 = micros();
uint32_t edgeAtMicros = ~0;
uint32_t lastBurnMillis = millis();
uint8_t startupCounter = 96;
#if SERIAL_ON
uint32_t lastMonitorMillis = millis();
#if MONITOR_REGISTER_SIGNAL
uint8_t MONITORidx = 0;
uint32_t edgeAtMicrosMONITOR[32];
uint16_t signalMONITOR[32];
//uint32_t microsMONITOR[32];
#endif
#endif


bool pollSensor() {
// Poll capacitive sensor
  if ( startupCounter > 0 ) {                       // Ignore sensors briefly after start up while they settle and calibrate themselves.
    sensor.output();
    startupCounter--;
    return 0;
  }
  return sensor.output();
}


void updateLanterns() {
  lantern[idx].pushInput( pollSensor() );
  lantern[idx].update();
  for ( uint8_t i=0; i<16; i++ ) {
    if ( lantern[i].nextBrightness() ) {
      pwmSignal.changeDuty( i, lantern[i].getBrightness() );
    }
  }
}


void updateRegister(uint16_t signal) {
// Write new signal to register.
  shiftRegister.writeToStorageRegister(signal);

  #if MONITOR_REGISTER_SIGNAL
  signalMONITOR[MONITORidx & 0x1F] = signal;
  edgeAtMicrosMONITOR[MONITORidx & 0x1F] = edgeAtMicros;
  MONITORidx++;
  #endif
}


void burnLanterns() {
// Burn down all lit lanterns. 
  for ( uint8_t i=0; i<16; i++ ) {
    lantern[i].burnDown(); 
  }
}

#if SERIAL_ON
void printIndices() {
  Serial.println();
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print(lantern[i].getIndex(), HEX); Serial.print("\t");
  }
  Serial.println(); 
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print("========");
  }
  Serial.println();
}
#endif


#if MONITOR_STATE
void printState() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print("0x"); Serial.print(lantern[i].getState(), HEX); Serial.print("\t"); 
  }
  Serial.println();
}
#endif


#if MONITOR_LANTERN_INPUT
void printLanternInput() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print(lantern[i].getInput(), BIN); Serial.print("\t");
  }
  Serial.println();
}
#endif


#if MONITOR_BRIGHTNESS
void printBrightness() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print(lantern[i].getBrightness()); Serial.print(":"); Serial.print(lantern[i].getBrightnessTarget()); Serial.print("\t"); 
  }
  Serial.println();
}
#endif


#if MONITOR_PARENT
void printParent() {
  for ( uint8_t i=0; i<16; i++ ) { 
    if ( lantern[i].getParent() ) {
      Serial.print( lantern[i].getParent()->getIndex(), HEX ); 
    }
    Serial.print( "\t" );
  }
  Serial.println();
}
#endif


#if MONITOR_DELAY
void printDelay() {
  for ( uint8_t i=0; i<16; i++ ) { Serial.print(lantern[i].getDelay()); Serial.print(" ds\t"); }
  Serial.println();
}
#endif


#if MONITOR_REGISTER_SIGNAL
void printRegisterSignal() {
  Serial.print("\nInterval\tSignal\n"); 
  for ( uint8_t i=0; i<32; i++ ) {
    //Serial.print(microsMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\t");
    Serial.print(edgeAtMicrosMONITOR[MONITORidx & 0x1F]); Serial.print("\t");
    Serial.print(signalMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\n");
    MONITORidx++;
  }
}
#endif


#if MONITOR_TIMINGS
void printTimings() {
  Serial.print("Sensor input:\t"); 
  Serial.print(inputTimer.getMean()); Serial.print("\t"); 
  Serial.println(inputTimer.getMax());
  Serial.print("Lantern update:\t"); 
  Serial.print(updateTimer.getMean()); Serial.print("\t"); 
  Serial.println(updateTimer.getMax());
  Serial.print("Brightness:\t"); 
  Serial.print(brightnessTimer.getMean()); Serial.print("\t"); 
  Serial.println(brightnessTimer.getMax());
  Serial.println();
}
#endif


void initialisePins() {
  // Set pins I/O.
  pinMode(PIN_SENSOR_RECEIVE, INPUT);
  pinMode(PIN_SENSOR_SEND, OUTPUT);
  DDRB |= PIN_REGISTER_SER | PIN_REGISTER_NOT_OE | PIN_REGISTER_NOT_SRCLR | PIN_REGISTER_RCLK | PIN_REGISTER_SRCLK;
  DDRD |= PIN_MUX_S0 | PIN_MUX_S1 | PIN_MUX_S2 | PIN_MUX_S3;
}


void assignNeighbours() { 
  #define NONE 0xFF
  uint8_t neighbourArray[16][4] = { 
    { 0x01, 0x02, 0x04, NONE }, 
    { 0x00, 0x02, 0x03, 0x04 },
    { 0x00, 0x01, 0x03, 0x05 },
    { 0x01, 0x02, 0x05, NONE },

    { 0x00, 0x01, 0x06, 0x07 },
    { 0x02, 0x03, 0x06, 0x07 },
    { 0x04, 0x05, 0x08, 0x0C },
    { 0x04, 0x05, 0x08, 0x0C },
    
    { 0x06, 0x07, 0x09, 0x0A },   
    { 0x08, 0x0A, 0x0B, 0x0D },  
    { 0x08, 0x09, 0x0B, NONE },  
    { 0x09, 0x0A, 0x0F, NONE },  
                             
    { 0x06, 0x07, 0x0D, 0x0E },  
    { 0x09, 0x0C, 0x0E, 0x0F },  
    { 0x0C, 0x0D, 0x0F, NONE },  
    { 0x0B, 0x0D, 0x0E, NONE }
  };
  uint8_t neighbourIdx;
  for ( uint8_t i=0; i<16; i++ ) {
    Lantern* neighbour[4];
    uint8_t nNeighbours = 0;
    for ( uint8_t j=0; j<4; j++ ) {
      neighbourIdx = neighbourArray[i][j];
      if ( neighbourIdx == NONE ) { continue; }
      neighbour[j] = &lantern[neighbourIdx];
      nNeighbours++;
    }
    lantern[i].setNeighbours(neighbour, nNeighbours); 
  }
}
                        

void setup() {
  for ( uint8_t i=0; i<16; i++ ) { lantern[i].setIndex(i); }
  initialisePins();
  assignNeighbours();
  shiftRegister.reset();

  #if SERIAL_ON
    Serial.begin(9600);		
    while(!Serial) {}
    #if MONITOR_REGISTER_SIGNAL
      for ( uint8_t i=0; i<16; i++ ) {
        edgeAtMicrosMONITOR[i] = 0;
        signalMONITOR[i] = 0;
        //microsMONITOR[i] = 0;
      }
    #endif
  #endif
}


void loop() {                                                   // TODO Monitor timings
  uint32_t currentMillis = millis();
  uint32_t currentMicros = micros();
  
  if ( currentMicros - pwmPhase0 >= edgeAtMicros ) {
  // Second half of PWM cycle
    updateRegister( pwmSignal.getSignal() );
    pwmSignal.nextEdge();
    edgeAtMicros = pwmSignal.getTime();
  }

  if ( currentMicros - pwmPhase0 >= PWM_PERIOD ) {
    // Mark start of PWM cycle here. For all signals with duty cycle greater than 50% set bit to 1, otherwise set to 0. 
    // The signal is phase shifted by 180 degrees when its duty cycle crosses 50%. This ensures that there is no pulse edge and therefore no need to update the register for the first half of a cycle. All sensor polling, updates and calculations are done during this time. Only register updates at pre calculated times are done during the second half of the cycle.

    pwmPhase0 = currentMicros;
    pwmSignal.periodStart();
    updateRegister( pwmSignal.getSignal() );
    updateLanterns();
    pwmSignal.nextEdge();
    edgeAtMicros = pwmSignal.getTime();
    idx = sensor.nextMuxChannel();
  }

  if ( currentMillis - lastBurnMillis >= BURN_INTERVAL ) {
    lastBurnMillis = currentMillis;
    burnLanterns();
  }


  #if SERIAL_ON
  if ( currentMillis - lastMonitorMillis >= MONITOR_INTERVAL ) {
    lastMonitorMillis = currentMillis;

    if ( millis() % (8 * MONITOR_INTERVAL) < MONITOR_INTERVAL ) {
      printIndices();
    }
    #if MONITOR_TIMINGS
    printTimings();
    #endif
    #if MONITOR_STATE
    printState();
    #endif
    #if MONITOR_LANTERN_INPUT
    printLanternInput();
    #endif
    #if MONITOR_BRIGHTNESS
    printBrightness();
    #endif
    #if MONITOR_PARENT
    printParent();
    #endif
    #if MONITOR_DELAY
    printDelay();
    #endif
    #if MONITOR_PWM_LIST
    pwmSignal.printSignalList(); Serial.print("\n");
    #endif
    #if MONITOR_REGISTER_SIGNAL
    printRegisterSignal();
    #endif
  }
  #endif
}
